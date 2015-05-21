#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

PKG_URL=git://git.ti.com/ipc/ludev.git
# hash for tag 4.10.00.01
PKG_REVISION=0d2fac7187ca1f6af62a98dee5b3621908cf5831

MOD_PATH=`ls -d $(FSROOT)/lib/modules/2.6.* | tail -n 1`
INSTALL_MOD_PATH=$(MOD_PATH)/ti
KERNELDIR="$(DEVDIR)/kernel/$(KERNEL)"
MODULE_SRC_DIR=$(PWD)/src/src/cmem/module
FETCHER_EXTRACT_DIRECTORY_NAME=src

include ../../../bsp/classes/rrsdk.class
include $(CLASSES)/fetcher.defs
# can not use linuxmodule.class because it uses MODULE_SRC_DIR as the extraction directory
#include $(CLASSES)/linuxmodule.class

build: rrbuilt

rrbuilt: rrfetched $(RRSDK_PREPATCH_TARGETS) rrpatched
	$(V)$(MAKE) -C $(KERNELDIR) M=$(MODULE_SRC_DIR) $(KFLAGS) EXTRA_CFLAGS=-I$(PWD)/src/include V=1 modules
	$(V) touch $@

install: rrinstalled

rrinstalled: rrbuilt
	$(V) mkdir -p $(INSTALL_MOD_PATH)
	$(V) ( cd  $(MODULE_SRC_DIR) ; find ./ | grep '\.ko$$' | xargs tar cf - | ( cd $(INSTALL_MOD_PATH) ; tar xf - ) )
	#rebuild the module database so it includes the compat-wireless modules
	$(MAKE) -j $(BSP_NCPU) -C $(KERNELDIR) modules_install $(KFLAGS) INSTALL_MOD_PATH="$(FSROOT)" $(QOUT)
	$(V) touch $@

clean: unpatch
	$(V)if [ -r $(MODULE_SRC_DIR)/Makefile ] ; then \
	    $(MAKE) -C $(MODULE_SRC_DIR) $(KFLAGS) clean $(QOUT) ; \
	fi
	$(V) rm -f rrbuilt rrinstalled

distclean: rrfetched_clean
	$(V) rm -rf rrpatched $(RRSDK_PREPATCH_TARGETS) rrbuilt rrinstalled .pc series
