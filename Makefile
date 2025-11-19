# Makefile for Pocketbook Companion application

# Application name
TARGET = calibre-companion.app

# SDK paths - укажите путь к вашему SDK
SDK_ROOT ?= $(HOME)/SDK_6.3.0/SDK-B288
TOOLCHAIN_PREFIX = arm-obreey-linux-gnueabi

# Check if SDK exists
ifeq ($(wildcard $(SDK_ROOT)),)
    $(error SDK not found at $(SDK_ROOT). Please clone it: git clone --depth 1 --branch 5.19 https://github.com/pocketbook/SDK_6.3.0.git $(HOME)/SDK_6.3.0)
endif

# Compiler settings
CC = $(SDK_ROOT)/bin/$(TOOLCHAIN_PREFIX)-gcc
STRIP = $(SDK_ROOT)/bin/$(TOOLCHAIN_PREFIX)-strip
SYSROOT = $(SDK_ROOT)/$(TOOLCHAIN_PREFIX)/sysroot

# Compiler flags
CFLAGS = -Wall -O2 --sysroot=$(SYSROOT) -I$(SYSROOT)/usr/include
LDFLAGS = --sysroot=$(SYSROOT) -L$(SYSROOT)/usr/lib -linkview

# Source files
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	$(STRIP) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	@echo "Copy $(TARGET) to your PocketBook device's /applications folder"

.PHONY: all clean install
