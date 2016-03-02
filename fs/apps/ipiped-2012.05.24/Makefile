#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com).
##$L$

# Note: The following package revision has no official release,
# for that reason we use the date of the last change as the version number 

PKG_URL=git://github.com/RidgeRun/ipiped.git
PKG_TYPE=git
PKG_REVISION=e54fcb1871bc16d3009c8a15580bf7d32aa33ad1

include ../../../bsp/classes/rrsdk.class

# The gvariant on ARM has a bug with double floating values if the compiler has optimizations
CFLAGS:=$(patsubst -O%,,$(CFLAGS)) 
CPPFLAGS:=$(patsubst -O%,,$(CPPFLAGS)) 

# Custom Autotools parameters
APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib
CFLAGS+=-I$(DEVDIR)/kernel/$(KERNEL)/include -I$(FSDEVROOT)/usr/include

AUTOTOOLS_PARAMS += CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(APPS_LDFLAGS)"

# When doing development for the application is very tedius to be doing make clean every time
# we do a change on the code, just because we need to re-copy it because doesn't build fine outside
# the src dir. So we just clone the code for the simulator (since we mostly develop for target)
ifeq ($(MAKECMDGOALS),sim)
AUTOTOOLS_CLONE_SOURCE=yes
else
#For vala sources we need to build over the src dir
AUTOTOOLS_BUILD_DIR=src
endif


CORRECT_DBUS_SERVICE=/usr/share/dbus-1/system-services/com.ridgerun.ipiped.service
BINARIES=/usr/bin/ipiped /usr/bin/ipipe-client
OTHERS=/usr/etc/ipiped.conf \
  /etc/dbus-1/system.d/com.ridgerun.ipiped.conf \
  /usr/share/ipiped/{dm365_mt9p031_config,dm365_mt9p011_config} \
  /usr/share/dbus-1/system-services/com.ridgerun.ipiped.service
INIT_SCRIPT=ipiped.init
INIT_SCRIPT_LEVEL=50

include $(CLASSES)/autotools.class
