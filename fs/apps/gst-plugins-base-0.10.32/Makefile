#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

include ../../../bsp/classes/rrsdk.class


PKG_URL=http://gstreamer.freedesktop.org/src/gst-plugins-base/
PKG_TARBALL=gst-plugins-base-0.10.32.tar.bz2
PKG_SHA1SUM=c1c149272b7ab6cbe5f648532f74525c6541eea5

APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib

AUTOTOOLS_PARAMS= --disable-rpath --disable-valgrind --disable-nls --disable-schemas-install \
--disable-examples --disable-gtk-doc LDFLAGS="$(APPS_LDFLAGS)" \
--disable-x

ifneq ($(CONFIG_FS_APPS_LIBOGG),y)
AUTOTOOLS_PARAMS+=--disable-ogg
endif

ifneq ($(CONFIG_FS_APPS_LIBVORBIS),y)
AUTOTOOLS_PARAMS+=--disable-vorbis
endif

ifneq ($(CONFIG_FS_APPS_LIBTHEORA),y)
AUTOTOOLS_PARAMS+=--disable-theora
endif

AUTOTOOLS_PRECONFIG_TARGETS=force_autogen

force_autogen:
	cd $(AUTOTOOLS_SRC_DIR) && autoreconf -f

BINARIES= /usr/bin/{gst-discoverer-0.10,gst-visualise-0.10}
LIBRARIES= /usr/lib/{libgstapp-0.10.so.0.23.0,libgstaudio-0.10.so.0.23.0,libgstcdda-0.10.so.0.23.0} \
	/usr/lib/{libgstfft-0.10.so.0.23.0,libgstinterfaces-0.10.so.0.23.0,libgstnetbuffer-0.10.so.0.23.0} \
	/usr/lib/{libgstpbutils-0.10.so.0.23.0,libgstriff-0.10.so.0.23.0,libgstrtp-0.10.so.0.23.0}\
	/usr/lib/{libgstsdp-0.10.so.0.23.0,libgsttag-0.10.so.0.23.0,libgstvideo-0.10.so.0.23.0,libgstrtsp-0.10.so.0.23.0}

include $(CLASSES)/gstreamer-plugin.class