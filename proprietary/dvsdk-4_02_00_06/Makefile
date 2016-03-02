#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com).
##$L$

include ../../bsp/classes/rrsdk.class

# Fetcher definitions
PKG_TARBALL=$(DVSDK_NAME)-trimmed.tar.gz

ifeq ($(CONFIG_PROPRIETARY_TI_CODECENGINE_DM365),y)
DVSDK_PLATFORM=dm365
DVSDK_NAME=ti-dvsdk_dm365-evm_4_02_00_06
# This is the file we test to see if we have the right DVSDK
# Particular useful as you may have installed something on /usr/local/dvsdk that doesn't match your arch
DVSDK_VERIFICATION_FILE=codecs-dm365_4_02_00_00
endif

ifeq ($(CONFIG_PROPRIETARY_TI_CODECENGINE_DM368),y)
DVSDK_PLATFORM=dm368
DVSDK_NAME=ti-dvsdk_dm368-evm_4_02_00_06
# This is the file we test to see if we have the right DVSDK
# Particular useful as you may have installed something on /usr/local/dvsdk that doesn't match your arch
DVSDK_VERIFICATION_FILE=codecs-dm365_4_02_00_00
endif

ifeq ($(CONFIG_PROPRIETARY_TI_CODECENGINE_OMAP35x),y)
DVSDK_PLATFORM=omap35x
DVSDK_NAME=ti-dvsdk_omap3530-evm_4_02_00_06
# This is the file we test to see if we have the right DVSDK
# Particular useful as you may have installed something on /usr/local/dvsdk that doesn't match your arch
DVSDK_VERIFICATION_FILE=codecs-omap3530_4_02_00_00
endif

ifeq ($(CONFIG_PROPRIETARY_TI_CODECENGINE_DM373x),y)
DVSDK_PLATFORM=dm3730
DVSDK_NAME=ti-dvsdk_dm3730-evm_4_02_00_06
# This is the file we test to see if we have the right DVSDK
# Particular useful as you may have installed something on /usr/local/dvsdk that doesn't match your arch
DVSDK_VERIFICATION_FILE=codecs-omap3530_4_02_00_00
endif

ifeq ($(CONFIG_PROPRIETARY_TI_CODECENGINE_OMAPL138),y)
DVSDK_PLATFORM=omapl138
DVSDK_NAME=ti-dvsdk_omapl138-evm_4_02_00_06
# This is the file we test to see if we have the right DVSDK
# Particular useful as you may have installed something on /usr/local/dvsdk that doesn't match your arch
DVSDK_VERIFICATION_FILE=codecs-omapl138_4_02_00_00
endif

INIT_SCRIPT=coprocessors.init
INIT_SCRIPT_LEVEL=90

# We only generate command line when we are enabled
ifeq ($(CONFIG_PROPRIETARY_TI_CODECENGINE),y)
# We are a dvsdk 4
include $(CLASSES)/dvsdk4.class

cmdline: dvsdk_cmdline
else
cmdline:
endif
