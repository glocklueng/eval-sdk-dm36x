# Copyright (C) 2007 RidgeRun (http://www.ridgerun.com)
#
# A license for this software is granted for evaluation purposes only.
# Commercial use and/or redistribution is strictly forbidden without 
# written consent from RidgeRun.      
# A free license may be available for educational or personal use.  
# Contact RidgeRun for permission


.PHONY: build chkconfig preconfig buildfs install clean

ifeq ($(DEVDIR),)
$(error ====== DEVDIR variable is empty, invoke this Makefile from the BSP root, or provide the path to it =====)
endif
include $(DEVDIR)/bsp/mach/Make.conf

build:
	# Install basic /etc files
	$(V)cp -R etc $(FSROOT)/
	$(V)find $(FSROOT)/etc/ -name .svn | xargs rm -Rf

ifeq ($(CONFIG_BSP_VIDEO_OUTPUT_COMPONENT),y)
DISPLAY_OUTPUT=COMPONENT
 ifeq ($(CONFIG_BSP_VIDEO_MODE_720P_60),y)
    DISPLAY_MODE=720P-60
 endif
 ifeq ($(CONFIG_BSP_VIDEO_MODE_1080I_30),y)
    DISPLAY_MODE=1080I-30
 endif
endif
ifeq ($(CONFIG_BSP_VIDEO_OUTPUT_COMPOSITE),y)
DISPLAY_OUTPUT=COMPOSITE
DISPLAY_MODE=NTSC
endif
ifeq ($(CONFIG_BSP_VIDEO_OUTPUT_PRGB),y)
DISPLAY_OUTPUT=PRGB
DISPLAY_MODE=PRGB
endif

ifeq ($(CONFIG_BSP_VIDEO_OUTPUT_SIZE_480P),y)
DISPLAY_BUFSIZE=3145728
OSD_SPEC=720x480x16,1350K
endif
ifeq ($(CONFIG_BSP_VIDEO_OUTPUT_SIZE_720P),y)
DISPLAY_BUFSIZE=6291456
OSD_SPEC=1280x720x16,1800K
endif
ifeq ($(CONFIG_BSP_VIDEO_OUTPUT_SIZE_1080I),y)
DISPLAY_BUFSIZE=13631488
OSD_SPEC=1920x1080x16,4050K
endif
ifeq ($(CONFIG_BSP_VIDEO_OUTPUT_SIZE_OTHER),y)
DISPLAY_BUFSIZE=$(CONFIG_BSP_VIDEO_DISPLAY_BUFFER_SIZE)
OSD_SPEC=$(subst ",,$(CONFIG_BSP_VIDEO_OUTPUT_OSD0_SPEC))
endif
ifeq ($(CONFIG_BSP_VIDEO_INPUT_SIZE_480P),y)
CAPTURE_BUFSIZE=3145728
endif
ifeq ($(CONFIG_BSP_VIDEO_INPUT_SIZE_720P),y)
CAPTURE_BUFSIZE=6291456
endif
ifeq ($(CONFIG_BSP_VIDEO_INPUT_SIZE_1080P),y)
CAPTURE_BUFSIZE=12582912
endif
ifeq ($(CONFIG_BSP_VIDEO_INPUT_SIZE_OTHER),y)
CAPTURE_BUFSIZE=$(CONFIG_BSP_VIDEO_CAPTURE_BUFFER_SIZE)
endif

cmdline:
	$(V)echo -n " davinci_enc_mngr.ch0_output=$(DISPLAY_OUTPUT) davinci_enc_mngr.ch0_mode=$(DISPLAY_MODE) ">>$(CMDLINEFILE)
	$(V)echo -n " davinci_display.cont2_bufsize=$(DISPLAY_BUFSIZE) vpfe_capture.cont_bufoffset=$(DISPLAY_BUFSIZE) vpfe_capture.cont_bufsize=$(CAPTURE_BUFSIZE)">>$(CMDLINEFILE)
	$(V)echo -n " video=davincifb:osd1=0x0x8:osd0=$(OSD_SPEC)@0,0:vid0=off:vid1=off ">>$(CMDLINEFILE)

chkconfig:

preconfig:

buildfs:

install:

clean:
