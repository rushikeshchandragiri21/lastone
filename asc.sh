#!/bin/bash

# Read 10 numbers from the user
echo "Enter 10 numbers:"
read -r num1 num2 num3 num4 num5 num6 num7 num8 num9 num10

# Sort the numbers in ascending order
sorted_numbers=$(echo -e "$num1\n$num2\n$num3\n$num4\n$num5\n$num6\n$num7\n$num8\n$num9\n$num10" | sort -n)

# Print the sorted numbers
echo "Sorted numbers in ascending order:"
echo "$sorted_numbers"
