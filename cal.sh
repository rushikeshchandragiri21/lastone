#!/bin/bash

# Function to perform addition
add() {
    echo "Result: $(($1 + $2))"
}

# Function to perform subtraction
subtract() {
    echo "Result: $(($1 - $2))"
}

# Function to perform multiplication
multiply() {
    echo "Result: $(($1 * $2))"
}

# Function to perform division
divide() {
    if [ $2 -eq 0 ]; then
        echo "Error: Division by zero!"
    else
        echo "Result: $(($1 / $2))"
    fi
}

# Function to perform exponentiation
power() {
    echo "Result: $(($1 ** $2))"
}

# Function to perform modulus
modulus() {
    echo "Result: $(($1 % $2))"
}

# Function to perform square root
square_root() {
    echo "Result: $(echo "scale=2; sqrt($1)" | bc)"
}

# Main function to parse user input and perform calculations
main() {
    if [ $# -ne 3 ]; then
        echo "Usage: $0 <operand1> <operator> <operand2>"
        echo "Example: $0 5 + 3"
        exit 1
    fi

    operand1=$1
    operator=$2
    operand2=$3

    case $operator in
        +) add $operand1 $operand2 ;;
        -) subtract $operand1 $operand2 ;;
        \*) multiply $operand1 $operand2 ;;
        /) divide $operand1 $operand2 ;;
        **) power $operand1 $operand2 ;;
        %) modulus $operand1 $operand2 ;;
        sqrt) square_root $operand1 ;;
        *) echo "Invalid operator: $operator"; exit 1 ;;
    esac
}

# Call the main function with command line arguments
main "$@"

