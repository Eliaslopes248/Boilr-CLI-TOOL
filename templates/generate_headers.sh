#!/bin/bash

# Script to convert .zip template files to .h header files
# This allows the repository to store smaller .zip files instead of large .h files
# Usage: ./generate_headers.sh [template_name.zip]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=========================================="
echo "Generating .h files from .zip templates"
echo "=========================================="
echo ""

# If a specific file is provided, process only that file
if [ $# -eq 1 ]; then
    ZIP_FILE="$1"
    if [ ! -f "$ZIP_FILE" ]; then
        echo "Error: File '$ZIP_FILE' not found"
        exit 1
    fi
    
    # Extract base name (without .zip extension)
    BASE_NAME=$(basename "$ZIP_FILE" .zip)
    OUTPUT_FILE="${BASE_NAME}.h"
    
    echo "Converting: $ZIP_FILE -> $OUTPUT_FILE"
    xxd -i "$ZIP_FILE" > "$OUTPUT_FILE"
    echo "✓ Created $OUTPUT_FILE"
    exit 0
fi

# Otherwise, process all .zip files in the directory
ZIP_COUNT=$(find . -maxdepth 1 -name "*.zip" | wc -l | tr -d ' ')

if [ "$ZIP_COUNT" -eq 0 ]; then
    echo "No .zip files found in templates directory"
    echo ""
    echo "To create a template:"
    echo "  1. Create your project template"
    echo "  2. Zip it: zip -r template_name.zip template_folder/"
    echo "  3. Place the .zip file in the templates/ directory"
    echo "  4. Run this script to generate the .h file"
    exit 0
fi

echo "Found $ZIP_COUNT .zip file(s)"
echo ""

for zip_file in *.zip; do
    if [ -f "$zip_file" ]; then
        BASE_NAME=$(basename "$zip_file" .zip)
        OUTPUT_FILE="${BASE_NAME}.h"
        
        echo "Converting: $zip_file -> $OUTPUT_FILE"
        xxd -i "$zip_file" > "$OUTPUT_FILE"
        
        if [ $? -eq 0 ]; then
            echo "✓ Created $OUTPUT_FILE"
        else
            echo "✗ Failed to create $OUTPUT_FILE"
        fi
        echo ""
    fi
done

echo "=========================================="
echo "Done! Rebuild the project to use the new headers."
echo "=========================================="
