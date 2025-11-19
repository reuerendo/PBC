#!/bin/bash

# PocketBook SDK Setup Script
# This script automates the installation of PocketBook SDK and toolchain

set -e

echo "=========================================="
echo "PocketBook SDK Setup"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default paths
DEFAULT_SDK_DIR="$HOME/pocketbook/sdk"
DEFAULT_TOOLCHAIN_DIR="$HOME/pocketbook/toolchain"

# Function to print colored messages
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if running on Linux
if [[ "$OSTYPE" != "linux-gnu"* ]]; then
    print_warning "This script is designed for Linux. For other systems, please install manually."
fi

# Create directories
print_info "Creating directories..."
mkdir -p "$DEFAULT_SDK_DIR"
mkdir -p "$DEFAULT_TOOLCHAIN_DIR"

# Install dependencies
print_info "Installing system dependencies..."
if command -v apt-get &> /dev/null; then
    sudo apt-get update
    sudo apt-get install -y build-essential git wget unzip tar
elif command -v dnf &> /dev/null; then
    sudo dnf install -y gcc gcc-c++ git wget unzip tar
elif command -v yum &> /dev/null; then
    sudo yum install -y gcc gcc-c++ git wget unzip tar
else
    print_warning "Could not detect package manager. Please install: build-essential, git, wget, unzip, tar"
fi

# Download SDK
print_info "Downloading PocketBook SDK..."
cd "$DEFAULT_SDK_DIR"

if [ ! -d "SDK_6.3.0" ]; then
    print_info "Cloning SDK repository..."
    git clone https://github.com/pocketbook/SDK_6.3.0.git || {
        print_warning "Git clone failed. Trying wget..."
        wget https://github.com/pocketbook/SDK_6.3.0/archive/refs/tags/SDK-B288.tar.gz
        tar xzf SDK-B288.tar.gz
        mv SDK_6.3.0-SDK-B288 SDK_6.3.0
    }
else
    print_info "SDK already exists, skipping download."
fi

# Download toolchain
print_info "Downloading ARM toolchain..."
cd "$DEFAULT_TOOLCHAIN_DIR"

if [ ! -f "SDK_6.3.0.zip" ]; then
    wget http://ftp.obreey.com/SDK/SDK_6.3.0.zip || {
        print_error "Failed to download toolchain. Please download manually from:"
        print_error "http://ftp.obreey.com/SDK/SDK_6.3.0.zip"
        exit 1
    }
    
    print_info "Extracting toolchain..."
    unzip -q SDK_6.3.0.zip
else
    print_info "Toolchain already exists, skipping download."
fi

# Setup environment variables
print_info "Setting up environment variables..."

ENV_FILE="$HOME/.pocketbook_env"
cat > "$ENV_FILE" << EOF
# PocketBook SDK environment variables
export POCKETBOOK_SDK_PATH=$DEFAULT_SDK_DIR/SDK_6.3.0
export ARM_SDK=$DEFAULT_TOOLCHAIN_DIR/SDK_6.3.0
export PATH=\$ARM_SDK/bin:\$PATH
EOF

# Add to shell configuration
SHELL_RC="$HOME/.bashrc"
if [ -f "$HOME/.zshrc" ]; then
    SHELL_RC="$HOME/.zshrc"
fi

if ! grep -q "pocketbook_env" "$SHELL_RC"; then
    echo "" >> "$SHELL_RC"
    echo "# PocketBook SDK" >> "$SHELL_RC"
    echo "source $ENV_FILE" >> "$SHELL_RC"
    print_info "Added PocketBook SDK to $SHELL_RC"
else
    print_info "PocketBook SDK already configured in $SHELL_RC"
fi

# Source environment
source "$ENV_FILE"

# Verify installation
print_info "Verifying installation..."

if command -v arm-obreey-linux-gnueabi-gcc &> /dev/null; then
    print_info "ARM compiler found:"
    arm-obreey-linux-gnueabi-gcc --version | head -n 1
else
    print_error "ARM compiler not found. Installation may have failed."
    print_info "Please check paths manually:"
    print_info "  SDK: $DEFAULT_SDK_DIR/SDK_6.3.0"
    print_info "  Toolchain: $DEFAULT_TOOLCHAIN_DIR/SDK_6.3.0"
    exit 1
fi

# Check if inkview.h exists
if [ -f "$DEFAULT_SDK_DIR/SDK_6.3.0/SDK-B288/usr/include/inkview.h" ]; then
    print_info "SDK headers found."
else
    print_warning "inkview.h not found. SDK may be incomplete."
fi

echo ""
echo "=========================================="
print_info "Setup complete!"
echo "=========================================="
echo ""
print_info "To start using the SDK in this terminal, run:"
echo "  source $ENV_FILE"
echo ""
print_info "Or restart your terminal to apply changes automatically."
echo ""
print_info "To build PocketBook Companion:"
echo "  cd /path/to/pocketbook-companion"
echo "  make"
echo ""
print_info "SDK installed at: $DEFAULT_SDK_DIR/SDK_6.3.0"
print_info "Toolchain installed at: $DEFAULT_TOOLCHAIN_DIR/SDK_6.3.0"
echo ""