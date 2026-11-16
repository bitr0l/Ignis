#
# EFI platform build script.
# Copyright (c) 2026, Ian Moffett, et al.
# Provided under the BSD-3 clause.
#

include ../../mk/common.mk

CFILES = $(shell find . -name "*.c")
CFILES += $(shell find $(GLOBAL_ROOT)/sdk/crt/ -name "*.c")
OFILES = $(CFILES:.c=.o)
DFILES = $(CFILES:.c=.d)

LDFLAGS =                               \
        $(COMMON_LDFLAGS)           	\
        -Wl,-subsystem:efi_application  \
        -Wl,-entry:efi_main

CFLAGS =                                 \
        $(COMMON_CFLAGS)             	 \
		-I$(GLOBAL_ROOT)/sdk/inc		 \
		-I$(GLOBAL_ROOT)/sdk/inc/crt	 \
		-I$(GLOBAL_ROOT)/platform/inc	 \
        -O2                              \
		-MMD                             \
		-Iinc							 \
		-Itarget						 \
		-fms-extensions

.PHONY: all
all: $(OFILES) $(GLOBAL_ROOT)/out/BOOTX64.EFI

-include $(DFILES)
%.o: %.c
	$(PROMPT) "CC" $<
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: $(GLOBAL_ROOT)/out/BOOTX64.EFI
$(GLOBAL_ROOT)/out/BOOTX64.EFI:
	$(CC) $(LDFLAGS) $(OFILES) -o $@
