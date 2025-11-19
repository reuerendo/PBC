# Makefile for PocketBook Companion Application

# Application name
TARGET = PBCompanion.app

# SDK paths (will be set by environment or GitHub Actions)
SDK_PATH ?= $(POCKETBOOK_SDK_PATH)
ARM_SDK ?= /usr/arm-obreey-linux-gnueabi

# Compiler and flags
CC = $(ARM_SDK)/bin/arm-obreey-linux-gnueabi-gcc
CFLAGS = -Wall -O2 -I$(SDK_PATH)/SDK-B288/usr/include
LDFLAGS = -L$(SDK_PATH)/SDK-B288/usr/lib -linkview -lfreetype -lz -lpng

# Source files
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link the application
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete!"

# Compile source files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning..."
	rm -f $(OBJECTS) $(TARGET)

# Install (copy to device or emulator)
install: $(TARGET)
	@echo "Installing $(TARGET)..."
	@echo "Copy $(TARGET) to /mnt/ext1/applications/ on your PocketBook device"

# Help
help:
	@echo "PocketBook Companion - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Build the application (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Show installation instructions"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Environment variables:"
	@echo "  SDK_PATH - Path to PocketBook SDK"
	@echo "  ARM_SDK  - Path to ARM cross-compiler"

.PHONY: all clean install help