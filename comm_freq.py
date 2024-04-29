import os

def get_command_frequency():
    history_file = os.path.expanduser("~/.bash_history")  # Path to the user's bash history file
    command_freq = {}
    
    # Read the command history file
    with open(history_file, "r") as file:
        for line in file:
            command = line.strip()
            command_freq[command] = command_freq.get(command, 0) + 1
    
    # Sort the commands by frequency in descending order
    sorted_commands = sorted(command_freq.items(), key=lambda x: x[1], reverse=True)
    
    return sorted_commands[:5]  # Return the top 5 commands with their frequencies

if __name__ == "__main__":
    top_commands = get_command_frequency()
    print("Top 5 most frequently used commands:")
    for command, count in top_commands:
        print(f"{command}: {count}")
