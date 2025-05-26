#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX

PLUGIN=TanDist

include dpf/Makefile.base.mk

all: plugins gen

# --------------------------------------------------------------

plugins:
	$(MAKE) all -C plugins/$(PLUGIN)

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/$(PLUGIN)
	rm -rf bin build

# --------------------------------------------------------------

.PHONY: plugins