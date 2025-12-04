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

echo "🔍 Checking if Git is installed..."

if command -v git >/dev/null 2>&1; then
    echo "✅ Git is already installed:"
    git --version
    exit 0
fi

echo "❌ Git not found. Installing..."

# =========================
# macOS
# =========================
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "🍏 macOS detected"

    if command -v brew >/dev/null 2>&1; then
        echo "✅ Homebrew found → Installing Git..."
        brew install git
    else
        echo "❌ Homebrew not found!"
        echo "➡️ Installing Homebrew first..."

        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

        echo "✅ Homebrew installed. Installing Git..."
        brew install git
    fi

# =========================
# Linux
# =========================
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "🐧 Linux detected"

    if command -v apt >/dev/null 2>&1; then
        sudo apt update
        sudo apt install -y git

    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y git

    elif command -v pacman >/dev/null 2>&1; then
        sudo pacman -S --noconfirm git

    else
        echo "❌ Unsupported Linux distribution!"
        exit 1
    fi

else
    echo "❌ Unsupported OS!"
    exit 1
fi

echo "✅ Git installation completed!"
git --version


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
