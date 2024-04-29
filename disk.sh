#!/bin/bash

# Show disk partitions and their sizes
echo "Disk partitions and their sizes:"
df -h

# Show disk usage (free space)
echo -e "\nDisk usage (free space):"
df -h --output=source,size,avail
