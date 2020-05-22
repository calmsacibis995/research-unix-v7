CURRENT_OS := $(strip $(shell uname -s))

ifeq ($(CURRENT_OS),Darwin)
CROSS_COMPILE ?= arm-none-eabi-
else ifeq ($(CURRENT_OS),Linux)
LINARO_DIR := $(shell (cd /opt && ls -1d gcc-linaro-*arm-eabi | tail -1))
ifeq ($(LINARO_DIR),)
CROSS_COMPILE ?= arm-eabi-
else
CROSS_COMPILE ?= /opt/$(LINARO_DIR)/bin/arm-eabi-
endif
else
$(error $(CURRENT_OS) is not a supported OS)
endif

CC = $(CROSS_COMPILE)gcc
CCLD = $(CROSS_COMPILE)gcc
#CC = clang -target armv6-none-eabi
AS = $(CROSS_COMPILE)as
CCAS = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB = $(CROSS_COMPILE)ranlib
SIZE = $(CROSS_COMPILE)size
NM = $(CROSS_COMPILE)nm
MKAOUT ?= $(SUBDIR_LEVEL)/tools/mkaout.py
YACC = $(SUBDIR_LEVEL)/tools/yacc/yacc

FLOAT_ABI_FLAG ?= -mfloat-abi=hard
CPPFLAGS ?= -I$(SUBDIR_LEVEL)/usr/include
# -fstack-usage option and -fcallgraph-info
# -Wall -pedantic
CFLAGS ?= -std=c89 -ffreestanding -nostdlib -nostdinc -nostartfiles -marm -march=armv6k+vfpv2 -mabi=aapcs $(FLOAT_ABI_FLAG) -ggdb
ASFLAGS ?= -ggdb -march=armv6k
LDFLAGS ?= $(CPPFLAGS) $(CFLAGS) -Wl,--build-id=none -T$(SUBDIR_LEVEL)/tools/cinit.lds
LOADLIBES ?= -L$(SUBDIR_LEVEL)/usr/src/libc
LDLIBS ?= -lc

CPP11SPECIALFLAGS =
C11SPECIALFLAGS = -std=c11 -ffreestanding -nostdlib -nostartfiles -marm -march=armv6k+vfpv2 -mabi=aapcs $(FLOAT_ABI_FLAG) -ggdb

KERNEL_LOADLIBES =
KERNEL_CPPFLAGS =
KERNEL_LDLIBS =
KERNEL_CFLAGS = -std=c89 -ffreestanding -nostdlib -nostartfiles -marm -march=armv6k+vfpv2 -mabi=aapcs -ggdb
KERNEL_C11SPECIALFLAGS = -std=c11 -ffreestanding -nostdlib -nostartfiles -marm -march=armv6k+vfpv2 -mabi=aapcs -ggdb
KERNEL_LDFLAGS = $(KERNEL_CPPFLAGS) $(KERNEL_CFLAGS) -Wl,--build-id=none -Wl,--defsym,PROGRAM_ENTRY_OFFSET=$(PROGRAM_ENTRY_OFFSET) -Wl,--defsym,KERNVIRTADDR=$(KERNVIRTADDR) -Wl,-N
