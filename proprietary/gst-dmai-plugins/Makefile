# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
#

.PHONY: build install clean

include ../../bsp/classes/rrsdk.class
# Apps.defs only needed when building
-include $(DEVDIR)/fs/Apps.defs

ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_TRUNK),y)
PKG_URL=https://gstreamer.ti.com/svn/gstreamer_ti/trunk/gstreamer_ti/ti_build/ticodecplugin
PKG_REVISION=916
PKG_NAME=gst-dmai-plugins_trunk
else
PKG_URL=https://gstreamer.ti.com/svn/gstreamer_ti/branches/BRANCH_DDOMPE/gstreamer_ti/ti_build/ticodecplugin
PKG_REVISION=1160
PKG_NAME=gst-dmai-plugins_ddompe
endif
PKG_TYPE=svn
SVN_AUTH_ARGUMENTS=--username anonymous --password anonymous --no-auth-cache

APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib
APPS_CFLAGS=-I$(DEVDIR)/kernel/$(KERNEL)/include -I$(BIOS_INSTALL_DIR)/packages
ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_TRUNK),y)
APPS_CFLAGS+=-DPlatform_$(PLATFORM)
endif

APPS_CFLAGS+=-D__EXPORTED_HEADERS__
AUTOTOOLS_PARAMS=LDFLAGS="$(APPS_LDFLAGS)" CFLAGS="$(APPS_CFLAGS)" CPPFLAGS="" --prefix=$(FSDEVROOT)/usr

ifeq ($(TOOLCHAIN),linaro-4.7.1)
AUTOTOOLS_PARAMS+=CFLAGS="$(APPS_CFLAGS) -Wno-unused-but-set-variable"
endif

AUTOTOOLS_AUTOGEN_PARAMS=--noconfigure
LIBRARIES=/usr/lib/gstreamer-0.10/libgstticodecplugin.so

# Newer XDC tools may define flags that conflict with the ones provided by the CFLAGS
# so we skip the config.site definitions
export CONFIG_SITE=

include $(CLASSES)/autotools.class

# This definitions below are variables required for the plugins to build properly
-include $(DVSDK)/Rules.make

ifeq ($(CONFIG_PROPRIETARY_TI_DVSDK4),y)
 CSTOOL_DIR  := $(CSTOOL_DIR)/bin
endif


ifeq ($(CONFIG_ARCH_DAVINCI_DM644x),y)
export PLATFORM=dm6446
export XDC_TARGET   = gnu.targets.arm.GCArmv5T
export XDC_PLATFORM = ti.platforms.evmDM6446
AUTOTOOLS_PARAMS+=--with-preset=dm6446
endif

ifeq ($(CONFIG_ARCH_DAVINCI_DM355),y)
export PLATFORM=dm355
ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_DM355S),y)
AUTOTOOLS_PARAMS+=--with-preset=dm355s
else
AUTOTOOLS_PARAMS+=--with-preset=dm355
endif
export XDC_USER_PATH:=$(DEVDIR)/proprietary/dm355_codecs/packages/;$(strip $(XDC_USER_PATH))
endif

ifeq ($(CONFIG_ARCH_DAVINCI_DM365),y)
ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_TRUNK),y)
export PLATFORM=dm365
export XDC_TARGET   = gnu.targets.arm.GCArmv5T
export XDC_PLATFORM = ti.platforms.evmDM365
else
AUTOTOOLS_PARAMS+=--with-preset=dm365
endif
export XDC_USER_PATH:=$(DEVDIR)/proprietary/dm365_mp3dec_4_8_01_production/mp3_dec_4_8_01_production_dm365_mvl/packages-production/;$(DEVDIR)/proprietary/dm365_mp3enc_3_5_00_production/mp3_enc_3_5_00_production_dm365_mvl/packages-production;$(strip $(XDC_USER_PATH))
CROSS_COMPILE=$(TOOLCHAIN_PREFIX)-
MVTOOL_PREFIX = $(CSTOOL_PREFIX)
 MVTOOL_DIR    = $(CSTOOL_DIR)/

endif

ifeq ($(CONFIG_ARCH_DAVINCI_DM646x),y)
ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_TRUNK),y)
export PLATFORM=dm6467t
export XDC_TARGET   = gnu.targets.MVArm9
export XDC_PLATFORM = ti.platforms.evmDM6467T
else
AUTOTOOLS_PARAMS+=--with-preset=dm646x
endif
CROSS_COMPILE=$(TOOLCHAIN_PREFIX)-
MVTOOL_PREFIX = $(CSTOOL_PREFIX)
MVTOOL_DIR    = $(CSTOOL_DIR)
LINK_XDC_ROOT = $(LINK_INSTALL_DIR)
endif

ifeq ($(CONFIG_ARCH_OMAP3),y)
 ifeq ($(CONFIG_BSP_ARCH_DM373x),y)
  ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_TRUNK),y)
   export XDC_TARGET    = gnu.targets.arm.GCArmv5T
   export XDC_PLATFORM  = ti.platforms.evm3530
   export PLATFORM=dm3730
  else
   AUTOTOOLS_PARAMS+=--with-preset=dm37x 
  endif
 else
  ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_TRUNK),y)
   export XDC_TARGET    = gnu.targets.arm.GCArmv5T
   export XDC_PLATFORM  = ti.platforms.evm3530
   export PLATFORM=omap3530
  else
   AUTOTOOLS_PARAMS+=--with-preset=omap35x 
  endif
 endif
CROSS_COMPILE=$(TOOLCHAIN_PREFIX)-
MVTOOL_PREFIX = $(CSTOOL_PREFIX)
MVTOOL_DIR    = $(CSTOOL_DIR)/
endif

ifeq ($(CONFIG_ARCH_DAVINCI_DA850),y)
ifeq ($(CONFIG_PROPRIETARY_TI_GST_CODECS_TRUNK),y)
export XDC_TARGET    = gnu.targets.arm.GCArmv5T
export XDC_PLATFORM  = ti.platforms.evmOMAPL138
export PLATFORM= omapl138
else
AUTOTOOLS_PARAMS+=--with-preset=omapl138
endif
MVTOOL_PREFIX = $(CSTOOL_PREFIX)
MVTOOL_DIR    = $(CSTOOL_DIR)
XDC_USER_PATH=$(LINK_INSTALL_DIR);
endif

ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE=bin/$(TOOLCHAIN_PREFIX)-
endif

# Needed by trunk plugins
export GST_TI_PLATFORM=$(PLATFORM)

export PLATFORM_XDC=$(PLATFORM)
export BIOS_INSTALL_DIR
export CE_INSTALL_DIR
export CMEM_INSTALL_DIR
export CODEC_INSTALL_DIR
export DMAI_INSTALL_DIR
export FC_INSTALL_DIR
export LINK_INSTALL_DIR
LINK_XDC_ROOT?= $(LINK_INSTALL_DIR)/packages
export LINK_XDC_ROOT
export LINUXKERNEL_INSTALL_DIR
export LPM_INSTALL_DIR
export MVTOOL_DIR
export MVTOOL_PREFIX
export XDAIS_INSTALL_DIR
export XDC_INSTALL_DIR
export CROSS_COMPILE
export C6ACCEL_INSTALL_DIR
export XDC_USER_PATH:=$(DEVDIR)/proprietary/codecs/;$(DEVDIR)/CodecServer/;$(strip $(XDC_USER_PATH))

#Config option that allows cmem debugging
export CONFIG_PROPRIETARY_TI_GST_CMEM_DEBUG
