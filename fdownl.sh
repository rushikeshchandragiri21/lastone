#!/bin/bash

# FTP server details
FTP_SERVER="10.10.13.16"
FILE_NAME="example.txt"  # Change this to the desired file name

# Check if the file exists on the FTP server
if lftp -e "ls $FILE_NAME" "ftp://$FTP_SERVER" &> /dev/null; then
    # File exists, so download it
    lftp -e "get $FILE_NAME; bye" "ftp://$FTP_SERVER"
    echo "File '$FILE_NAME' downloaded successfully."
else
    # File does not exist
    echo "File '$FILE_NAME' does not exist on the FTP server."
fi
