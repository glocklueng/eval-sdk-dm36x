#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com).
#$L$

.PHONY: build clean

PKG_URL=http://source.ridgerun.net/packages
PKG_TARBALL=ti-flash-utils-dvsdk4.tar.gz
PKG_SHA1SUM=ab78ebd45ac820b6400e255e6a04dbca50bf62db
FETCHER_EXTRACT_DIRECTORY_NAME=src

all:
	$(V)$(ECHO) "Please specify which platform you want to build for the flasher tools"
	$(V) exit -1

include ../../../bsp/classes/rrsdk.class
include $(CLASSES)/fetcher.defs

.PHONY=common

export CROSSCOMPILE=$(CCACHE) $(TOOLCHAIN_PATH)/bin/$(TOOLCHAIN_PREFIX)-
export CROSSCOMPILE_PREFIX=$(TOOLCHAIN_PREFIX)

ifeq ($(TOOLCHAIN),linaro-4.7.1)
export BFLAGS=-marm
endif

MAKE_EXTRA += SDRAM_BASE=$(CONFIG_BSP_ARCH_SDRAM_BASE) SDRAM_SIZE=$(CONFIG_BSP_ARCH_SDRAM_SIZE)

ifeq ($(CONFIG_UBL_TEST_MEMORY),y)
MAKE_EXTRA += TEST_MEMORY=y
endif

ifeq ($(CONFIG_UBL_PRINT_CONFIGURATION),y)
MAKE_EXTRA += PRINT_CONFIGURATION=y
endif

PLL_SETTING=UNCONFIGURED

ifeq ($(CONFIG_UBL_ARM432_DDR340_OSC24),y)
PLL_SETTING=ARM432_DDR340_OSC24
endif

ifeq ($(CONFIG_UBL_ARM445_DDR351_OSC24),y)
PLL_SETTING=ARM445_DDR351_OSC24
endif

ifeq ($(CONFIG_UBL_ARM297_DDR270_OSC24),y)
PLL_SETTING=ARM297_DDR270_OSC24
endif

ifeq ($(CONFIG_UBL_ARM297_DDR277_OSC27),y)
PLL_SETTING=ARM297_DDR277_OSC27
endif

ifeq ($(CONFIG_UBL_ARM270_DDR216_OSC24),y)
PLL_SETTING=ARM270_DDR216_OSC24
endif

ifeq ($(CONFIG_UBL_ARM216_DDR173_OSC19P2),y)
PLL_SETTING=ARM216_DDR173_OSC19P2
endif

ifeq ($(CONFIG_UBL_ARM216_DDR173_OSC24),y)
PLL_SETTING=ARM216_DDR173_OSC24
endif

ifeq ($(PLL_SETTING),UNCONFIGURED)
$(warning ERROR: unknown PLL setting)
endif

MAKE_EXTRA += PLL_SETTING=$(PLL_SETTING)

common: rrfetched rrpatched

build_dm365: common
	$(V) $(MAKE) -C src/DM36x/GNU/ubl/build/ TYPE=nand $(MAKE_EXTRA) $(QOUT) $(ERRQOUT)
	$(V) $(MAKE) -C src/DM36x/GNU/ubl/build/ TYPE=sdmmc $(MAKE_EXTRA) $(QOUT) $(ERRQOUT)
	$(V) cp src/DM36x/GNU/ubl/ubl_DM36x_{nand.bin,sdmmc.bin} $(IMAGEDIR)/

build_dm644x: common
	$(V) $(MAKE) -C src/DM644x/GNU/slt ARCH_DM6446T=$(CONFIG_BSP_ARCH_DM6446T) $(QOUT) $(ERRQOUT)
	$(V) $(MAKE) -C src/DM644x/GNU/slh $(QOUT) $(ERRQOUT)
	$(V) $(MAKE) -C src/DM644x/GNU/bc $(QOUT) $(ERRQOUT)
	$(V) $(MAKE) -C src/DM644x/GNU/ubl/build/ TYPE=nand ARCH_DM6446T=$(CONFIG_BSP_ARCH_DM6446T) $(QOUT) $(ERRQOUT)
	$(V) $(MAKE) -C src/DM644x/GNU/ubl/build/ TYPE=nor  ARCH_DM6446T=$(CONFIG_BSP_ARCH_DM6446T) $(QOUT) $(ERRQOUT)
	$(V) cp src/DM644x/GNU/ubl/ubl_DM644x_nand.bin $(IMAGEDIR)
	$(V) cp src/DM644x/GNU/ubl/ubl_DM644x_nor.bin  $(IMAGEDIR)

clean: unpatch
	$(V) if [ -d src ] ; then \
	  $(MAKE) -C src/DM36x/GNU/ubl/build/ TYPE=nand clean $(QOUT) $(ERRQOUT) ; \
	  $(MAKE) -C src/DM36x/GNU/ubl/build/ TYPE=sdmmc clean $(QOUT) $(ERRQOUT) ; \
	  $(MAKE) -C src/DM644x/GNU/ubl/build/ TYPE=nand clean $(QOUT) $(ERRQOUT) ; \
	  $(MAKE) -C src/DM644x/GNU/ubl/build/ TYPE=nor clean $(QOUT) $(ERRQOUT) ; \
	  $(MAKE) -C src/DM644x/GNU/slt clean $(QOUT) $(ERRQOUT) ; \
	  $(MAKE) -C src/DM644x/GNU/slh clean $(QOUT) $(ERRQOUT) ; \
	  $(MAKE) -C src/DM644x/GNU/bc clean $(QOUT) $(ERRQOUT) ; \
	fi

distclean: clean rrfetched_clean
