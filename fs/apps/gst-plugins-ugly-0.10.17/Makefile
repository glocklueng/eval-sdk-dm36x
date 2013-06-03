#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

include ../../../bsp/classes/rrsdk.class
include $(CLASSES)/gstreamer-plugin.class


PKG_URL=http://gstreamer.freedesktop.org/src/gst-plugins-ugly/
PKG_TARBALL=gst-plugins-ugly-0.10.17.tar.bz2
PKG_SHA1SUM=6e637d8e7927f7fb8d4c4ffe8b0120257dfb073c

APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib

AUTOTOOLS_PARAMS = --disable-rpath --disable-valgrind --disable-nls \
--disable-examples  --disable-gtk-doc --disable-a52dec --disable-amrnb --disable-dvdread \
--disable-lame --disable-mpeg2dec --disable-sidplay LDFLAGS="$(APPS_LDFLAGS)"


