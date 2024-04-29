#!/bin/bash

# Check if there are at least 2 arguments
if [ "$#" -lt 2 ]; then
    echo "Error: At least two filenames are required."
    exit 1
fi

# Check if all files exist
for file in "$@"; do
    if [ ! -f "$file" ]; then
        echo "Error: File '$file' does not exist."
        exit 1
    fi
done

# Concatenate files
cat "$@"
