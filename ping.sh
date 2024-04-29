#!/bin/bash

# Define the IP address or domain name to ping
target="8.8.8.8"  # Google's public DNS server

# Ping the target with a single packet and suppress output
ping -c 1 "$target" > /dev/null 2>&1

# Check the exit status of the ping command
if [ $? -eq 0 ]; then
    echo "Network connection is available."
else
    echo "No network connection."
fi
