import re
from collections import Counter

# Read the content of the text file
with open('wonderland.txt', 'r') as file:
    text = file.read()

# Tokenize the text into words
words = re.findall(r'\b\w+\b', text.lower())

# Count the frequency of each word
word_freq = Counter(words)

# Sort the words by frequency in descending order
sorted_word_freq = sorted(word_freq.items(), key=lambda x: x[1], reverse=True)

# Print the word frequency list
for word, freq in sorted_word_freq:
    print(f"{word}: {freq}")
