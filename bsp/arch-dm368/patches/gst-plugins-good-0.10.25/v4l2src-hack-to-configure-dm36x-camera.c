Index: gst-plugins-good-0.10.25/src/sys/v4l2/gstv4l2src.c
===================================================================
--- gst-plugins-good-0.10.25.orig/src/sys/v4l2/gstv4l2src.c	2014-04-16 15:07:55.963583378 -0600
+++ gst-plugins-good-0.10.25/src/sys/v4l2/gstv4l2src.c	2014-04-21 08:43:56.469648732 -0600
@@ -67,6 +67,8 @@
 
 #define DEFAULT_PROP_DEVICE   "/dev/video0"
 #define DEFAULT_CROP_AREA     NULL
+#define PROP_DEF_CAMERA_WIDTH       0
+#define PROP_DEF_CAMERA_HEIGHT      0
 
 enum
 {
@@ -81,7 +83,9 @@
   PROP_CHAIN_IPIPE,
 #endif
   PROP_INPUT_SRC,
-  PROP_CROP_AREA
+  PROP_CROP_AREA,
+  PROP_CAMERA_WIDTH,
+  PROP_CAMERA_HEIGHT,
 };
 
 GST_IMPLEMENT_V4L2_PROBE_METHODS (GstV4l2SrcClass, gst_v4l2src);
@@ -281,6 +285,16 @@
       g_param_spec_string ("crop-area", "Select the crop area.",
       "Selects the crop area using the format left:top:width:height", 
       NULL, G_PARAM_READWRITE));
+  g_object_class_install_property (gobject_class, PROP_CAMERA_WIDTH,
+      g_param_spec_uint ("camera-width", "Set camera width",
+      "Defines the camera input image's width, the output image's width "
+      "is defined with the src pad caps",
+      0, G_MAXUINT, PROP_DEF_CAMERA_WIDTH, G_PARAM_READWRITE));
+  g_object_class_install_property (gobject_class, PROP_CAMERA_HEIGHT,
+      g_param_spec_uint ("camera-height", "Set camera height.",
+      "Defines the camera input image's height, the output image's height "
+      "is defined with the src pad caps",
+      0, G_MAXUINT, PROP_DEF_CAMERA_HEIGHT, G_PARAM_READWRITE));
 #if defined(USE_DM365_IPIPE)
   g_object_class_install_property (gobject_class, PROP_CHAIN_IPIPE,
       g_param_spec_boolean ("chain-ipipe", "Chain the IPIPE",
@@ -319,6 +333,8 @@
   v4l2src->num_buffers = PROP_DEF_QUEUE_SIZE;
 
   v4l2src->always_copy = PROP_DEF_ALWAYS_COPY;
+  v4l2src->camera_width = PROP_DEF_CAMERA_WIDTH;
+  v4l2src->camera_height = PROP_DEF_CAMERA_HEIGHT;
 
   v4l2src->is_capturing = FALSE;
   v4l2src->v4l2object->input_src = NULL;
@@ -407,6 +423,12 @@
         if(v4l2src->is_capturing)
            gst_v4l2src_change_crop(v4l2src);
         break;
+      case PROP_CAMERA_WIDTH:
+        v4l2src->camera_width = g_value_get_uint (value);
+        break;
+      case PROP_CAMERA_HEIGHT:
+        v4l2src->camera_height = g_value_get_uint (value);
+        break;
       default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
         break;
@@ -446,6 +468,12 @@
       case PROP_CROP_AREA:
         g_value_set_string(value, v4l2src->v4l2object->crop_area);
         break;
+      case PROP_CAMERA_WIDTH:
+        g_value_set_uint (value, v4l2src->camera_width);
+        break;
+      case PROP_CAMERA_HEIGHT:
+        g_value_set_uint (value, v4l2src->camera_height);
+        break;
       default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
         break;
@@ -619,28 +647,16 @@
 }
 
 static GstCaps *
-gst_v4l2src_get_caps (GstBaseSrc * src)
+gst_v4l2src_get_caps_for_formats (GstV4l2Src *v4l2src)
 {
-  GstV4l2Src *v4l2src = GST_V4L2SRC (src);
-  GstCaps *ret;
-  GSList *walk;
   GSList *formats;
-
-  if (!GST_V4L2_IS_OPEN (v4l2src->v4l2object)) {
-    /* FIXME: copy? */
-    return
-        gst_caps_copy (gst_pad_get_pad_template_caps (GST_BASE_SRC_PAD
-            (v4l2src)));
-  }
-
-  if (v4l2src->probed_caps)
-    return gst_caps_ref (v4l2src->probed_caps);
-
+  GSList *walk;
+  GstCaps *caps;
+
+  caps = gst_caps_new_empty ();
   formats = gst_v4l2_object_get_format_list (v4l2src->v4l2object);
-
-  ret = gst_caps_new_empty ();
-
-  for (walk = v4l2src->v4l2object->formats; walk; walk = walk->next) {
+
+  for (walk = formats; walk; walk = walk->next) {
     struct v4l2_fmtdesc *format;
 
     GstStructure *template;
@@ -651,24 +667,83 @@
 
     if (template) {
       GstCaps *tmp;
-
       tmp =
           gst_v4l2_object_probe_caps_for_format (v4l2src->v4l2object,
           format->pixelformat, template);
       if (tmp)
-        gst_caps_append (ret, tmp);
+        gst_caps_append (caps, tmp);
 
       gst_structure_free (template);
     } else {
       GST_DEBUG_OBJECT (v4l2src, "unknown format %u", format->pixelformat);
     }
   }
+
+  return caps;
+}
+
+static GstCaps *
+gst_v4l2src_get_caps (GstBaseSrc * src)
+{
+  GstV4l2Src *v4l2src = GST_V4L2SRC (src);
+  GstCaps *ret;
+  GSList *walk;
+
+
+  if (!GST_V4L2_IS_OPEN (v4l2src->v4l2object)) {
+    /* FIXME: copy? */
+    return
+        gst_caps_copy (gst_pad_get_pad_template_caps (GST_BASE_SRC_PAD
+            (v4l2src)));
+  }
+
+  if (v4l2src->camera_width && v4l2src->camera_height) {
+    GstCaps *cam_caps;
+
+    GST_DEBUG_OBJECT (v4l2src, "Probe camera resolution");
+    ret = gst_v4l2src_get_caps_for_formats (v4l2src);
+    cam_caps = gst_caps_new_simple ("video/x-raw-yuv",
+     "width", G_TYPE_INT, v4l2src->camera_width,
+     "height", G_TYPE_INT, v4l2src->camera_height,
+     NULL);
+
+    if (!gst_caps_can_intersect (cam_caps, ret)) {
+      gst_caps_unref (ret);
+      gst_caps_unref (cam_caps);
+      goto invalid_fmt;
+    }
+
+    gst_caps_unref (cam_caps);
+    GST_DEBUG_OBJECT (v4l2src, "Setting camera to %dx%d format %"
+      GST_FOURCC_FORMAT,v4l2src->camera_width, v4l2src->camera_height,
+      GST_FOURCC_ARGS (V4L2_PIX_FMT_UYVY));
+
+    v4l2src->v4l2object->type = V4L2_BUF_TYPE_PRIVATE;
+    gst_v4l2_object_set_format (v4l2src->v4l2object,
+        V4L2_PIX_FMT_UYVY, v4l2src->camera_width, v4l2src->camera_height);
+    v4l2src->v4l2object->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
+  }
+
+  if (v4l2src->probed_caps)
+    return gst_caps_ref (v4l2src->probed_caps);
+
+  ret = gst_v4l2src_get_caps_for_formats (v4l2src);
 
   v4l2src->probed_caps = gst_caps_ref (ret);
 
   GST_INFO_OBJECT (v4l2src, "probed caps: %" GST_PTR_FORMAT, ret);
 
   return ret;
+
+invalid_fmt:
+  {
+    GST_ELEMENT_ERROR (v4l2src, RESOURCE, SETTINGS,
+        (_("Device '%s' cannot capture at %dx%d"),
+            v4l2src->v4l2object->videodev, v4l2src->camera_width, v4l2src->camera_height),
+        NULL
+        );
+    return gst_caps_new_empty();
+  }
 }
 
 static gboolean
Index: gst-plugins-good-0.10.25/src/sys/v4l2/gstv4l2src.h
===================================================================
--- gst-plugins-good-0.10.25.orig/src/sys/v4l2/gstv4l2src.h	2014-04-16 15:07:55.979583378 -0600
+++ gst-plugins-good-0.10.25/src/sys/v4l2/gstv4l2src.h	2014-04-16 15:07:58.251583387 -0600
@@ -83,6 +83,10 @@
   GstClockTime duration;       /* duration of one frame */
   
   GstV4l2SrcGetFunc get_frame;
+
+  /* DM36x camera properties */
+  guint32 camera_width;
+  guint32 camera_height;
 };
 
 struct _GstV4l2SrcClass
