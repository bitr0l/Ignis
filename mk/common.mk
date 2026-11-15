#
# Bootloader build variables.
# Copyright (c) 2026, Ian Moffett, et al.
# Provided under the BSD-3 clause.
#

COMMON_CFLAGS =							\
	-target x86_64-unknown-windows		\
	-ffreestanding						\
	-fshort-wchar						\
	-nostdinc							\
	-nostdlib

ifeq ($(CONFIG_ARCH),amd64)
	COMMON_CFLAGS +=					\
		-mno-red-zone					\
		-mgeneral-regs-only				\
		-mno-sse						\
		-mno-sse2						\
		-mno-avx						\
		-mno-avx2						\
		-mno-3dnow
endif

COMMON_LDFLAGS =				\
	-target x86_64-unknown-windows  \
	-fuse-ld=lld					\
	-nostdlib

CC = clang
LD = lld-link
