#!/bin/bash

# Detect OS
OS=$(uname)

# Function to check and install Python
check_and_install_python() {
    if ! command -v python3 &> /dev/null; then
        echo "Python3 not found. Installing..."
        if [ "$OS" == "Linux" ]; then
            sudo apt update && sudo apt install -y python3
        elif [ "$OS" == "Darwin" ]; then
            brew install python3
        fi
    else
        echo "Python3 is already installed."
    fi
}

# macOS: Check and install Homebrew
if [ "$OS" == "Darwin" ]; then
    if ! command -v brew &> /dev/null; then
        echo "Homebrew not found. Installing..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        export PATH="/opt/homebrew/bin:$PATH"
    else
        echo "Homebrew is already installed."
    fi
fi

# Check and install Python
check_and_install_python

# Run Python setup script
echo "Running Python setup script..."
python3 setup.py
