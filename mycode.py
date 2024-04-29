def count_word_frequencies(filename):
    word_freq = {}
    
    # Open the file and read its contents
    with open(filename, 'r') as file:
        for line in file:
            # Split each line into words
            words = line.split()
            for word in words:
                # Remove punctuation marks and convert to lowercase
                word = word.strip().lower().strip(",.!?")
                # Update word frequency dictionary
                if word:
                    word_freq[word] = word_freq.get(word, 0) + 1
    
    return word_freq

def main():
    filename = input("Enter the name of the text file: ")
    word_freq = count_word_frequencies(filename)
    
    # Print word frequencies
    for word, freq in word_freq.items():
        print(f"{word}: {freq}")

if __name__ == "__main__":
    main()
