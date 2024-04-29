#!/bin/bash

# Check if file name argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <file_name>"
    exit 1
fi

# Get file name from command line argument
FILE_NAME=$1

# Search for the file using the find command
echo "Searching for $FILE_NAME in the system..."
found_files=$(find / -name "$FILE_NAME" 2>/dev/null)

# Check if file is found
if [ -z "$found_files" ]; then
    echo "$FILE_NAME not found in the system."
else
    echo "$FILE_NAME found at:"
    echo "$found_files"
fi
# #!/bin/bash

# # Check if the user provided the file name and directory/drive
# if [ $# -ne 2 ]; then
#     echo "Usage: $0 <file_name> <directory/drive>"
#     exit 1
# fi

# # Store the file name and directory/drive
# file_name=$1
# directory_drive=$2

# # Check if the specified directory/drive exists
# if [ ! -d "$directory_drive" ]; then
#     echo "Directory/drive '$directory_drive' does not exist."
#     exit 1
# fi

# # Navigate to the specified directory/drive
# cd "$directory_drive" || exit

# # Check if the file exists
# if [ -e "$file_name" ]; then
#     echo "File '$file_name' exists in directory/drive '$directory_drive'."
# else
#     echo "File '$file_name' does not exist in directory/drive '$directory_drive'."
# fi
# chmod +x check_file_existence.sh
# ./check_file_existence.sh <file_name> <directory/drive>
