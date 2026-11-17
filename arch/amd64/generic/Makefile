#
# AMD64 build script.
# Copyright (c) 2026, Ian Moffett, et al.
# Provided under the BSD-3 clause.
#

.PHONY: all
all: target
	cd ../../../platform/efi; $(MAKE)

.PHONY: target
target:
	mkdir -p $(GLOBAL_ROOT)/platform/efi/target/machine
	rsync -avr $(GLOBAL_ROOT)/platform/inc/arch/$(CONFIG_ARCH)/$(CONFIG_CHIP)/*.h \
		$(GLOBAL_ROOT)/platform/efi/target/machine
