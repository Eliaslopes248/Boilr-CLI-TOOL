#!/usr/bin/env bash
# Installer for Linux/macOS

OS=$(uname -s)
if [ "$OS" = "Darwin" ]; then
    FILE="build-mac-universal.zip"
else
    FILE="build-linux.zip"
fi

echo "Downloading $FILE..."
curl -L -o $FILE "https://github.com/Eliaslopes248/Boilr-CLI-TOOL/releases/download/v1.1.3/$FILE"

echo "Extracting br..."
unzip -j $FILE '*/br' -d .

rm -f $FILE
echo "✅ br is ready in $(pwd)"
