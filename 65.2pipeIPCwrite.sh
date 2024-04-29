#!/bin/bash

FIFO_FILE="myfifo"

# Check if FIFO file exists, if not create it
if [ ! -p "$FIFO_FILE" ]; then
    mkfifo "$FIFO_FILE"
fi

# Prompt the user to enter data
echo "Enter data to write to FIFO. Press Ctrl+D to exit."

# Loop for continuously writing data to the FIFO
while true; do
    read input
    # Write input to the FIFO
    echo "$input" > "$FIFO_FILE"
done
