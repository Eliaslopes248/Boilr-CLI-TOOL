#!/bin/bash

# Script to zip all build-* directories in the releases folder
# Creates zip files for distribution

set -e  # Exit on error

for dir in build-*; do
  if [ -d "$dir" ]; then
    zip_file="${dir}.zip"
    echo "Creating ${zip_file} from ${dir}/..."
    
    # Remove existing zip if it exists
    if [ -f "$zip_file" ]; then
      rm -f "$zip_file"
    fi
    
    # Create zip file
    if zip -r "$zip_file" "$dir" > /dev/null 2>&1; then
      # Verify the zip was created and has content
      if [ -f "$zip_file" ] && [ -s "$zip_file" ]; then
        size=$(ls -lh "$zip_file" | awk '{print $5}')
        echo "✓ Created ${zip_file} (${size})"
      else
        echo "✗ Error: ${zip_file} was created but is empty!"
        exit 1
      fi
    else
      echo "✗ Error: Failed to create ${zip_file}"
      exit 1
    fi
  fi
done

echo ""
echo "All zip files created successfully!"
