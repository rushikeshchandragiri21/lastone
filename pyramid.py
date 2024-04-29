def print_pyramid(num_lines):
    # Determine the width of the pyramid base
    width = num_lines * 2 - 1

    # Print each line of the pyramid
    for i in range(1, num_lines + 1):
        # Calculate the number of spaces before and after the stars
        spaces = (width - (2 * i - 1)) // 2

        # Print leading spaces
        print(" " * spaces, end="")

        # Print stars
        print("*" * (2 * i - 1))


def main():
    # Prompt the user for the number of lines
    num_lines = int(input("Enter the number of lines for the pyramid: "))

    # Display the pyramid
    print_pyramid(num_lines)


if __name__ == "__main__":
    main()
