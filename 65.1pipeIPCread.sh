#!/bin/bash

FIFO_FILE="myfifo"

# Check if FIFO file exists, if not create it
if [ ! -p "$FIFO_FILE" ]; then
    mkfifo "$FIFO_FILE"
fi

# Read data from the FIFO
echo "Reading data from FIFO..."

# Loop for continuously reading data from the FIFO
while true; do
    # Read from the FIFO and display the data
    if read line < "$FIFO_FILE"; then
        echo "Received: $line"
    fi
done
