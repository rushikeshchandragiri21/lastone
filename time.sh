#!/bin/bash

# Function to clear the screen
clear_screen() {
    clear
}

# Function to display the current time
display_time() {
    # Get the current time in HH:MM:SS format
    current_time=$(date +"%T")
    echo "Current Time: $current_time"
}

# Main function to update and display the clock
main() {
    while true; do
        clear_screen
        display_time
        sleep 1  # Wait for 1 second before updating the clock
    done
}

# Call the main function
main
