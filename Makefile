# Makefile for Pocketbook Companion application

# Application name
TARGET = PocketbookCompanion.app

# SDK paths
SDK_PATH ?= /usr/arm-obreey-linux-gnueabi/sysroot
TOOLCHAIN_PATH ?= /usr/bin

# Compiler settings
CC = $(TOOLCHAIN_PATH)/arm-obreey-linux-gnueabi-gcc
CXX = $(TOOLCHAIN_PATH)/arm-obreey-linux-gnueabi-g++
STRIP = $(TOOLCHAIN_PATH)/arm-obreey-linux-gnueabi-strip

# Compiler flags
CFLAGS = -Wall -O2 -I$(SDK_PATH)/usr/include
LDFLAGS = -L$(SDK_PATH)/usr/lib -linkview -lfreetype -lz -lpthread

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
	@echo "Copy $(TARGET) to your PocketBook device"

.PHONY: all clean install
