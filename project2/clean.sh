#!/bin/bash

# Define the list of files and patterns to clean
FILES_TO_CLEAN=(
    "*.o"            # Object files
    "*.out"          # Executable files with .out extension (common for g++ output)
    "a.out"          # Default executable name for g++
    "output.bin"     # Specific output file
    "decoded.txt"    # Specific decoded file
    "*.tmp"          # Temporary files, if any
)

# Clean files matching patterns
for pattern in "${FILES_TO_CLEAN[@]}"; do
    echo "Cleaning up files matching pattern: $pattern"
    rm -f $pattern
done

# Optionally, remove any file without an extension (common for custom-named executables)
echo "Cleaning unnamed executables (files without extensions)"
find . -type f ! -name "*.*" -exec rm -f {} +

echo "Cleanup complete!"
