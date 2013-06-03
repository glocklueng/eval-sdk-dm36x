#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

PKG_URL=http://gstreamer.freedesktop.org/src/gst-ffmpeg/
PKG_TARBALL=gst-ffmpeg-0.10.11.tar.bz2
PKG_SHA1SUM=76e3cab4604ca276bc9d9fa94236d1339b4bda33

space :=
space +=
APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib
AUTOTOOLS_PARAMS = LDFLAGS="$(APPS_LDFLAGS)" --with-ffmpeg-extra-configure="$(subst $(space),\
		 --extra-cflags=,$(subst ",, $(CONFIG_TOOLCHAIN_ARCH_FLAGS)))"

include ../../../bsp/classes/rrsdk.class
include $(CLASSES)/gstreamer-plugin.class

