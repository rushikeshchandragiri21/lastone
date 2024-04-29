import os
import stat

def check_executable(filename):
    if not os.path.exists(filename):
        print(f"Error: File '{filename}' does not exist.")
        return
    
    if os.access(filename, os.X_OK):
        print(f"'{filename}' is executable.")
        # Remove execute permissions
        os.chmod(filename, os.stat(filename).st_mode & ~stat.S_IEXEC)
        print(f"Execute permissions removed from '{filename}'.")
    else:
        print(f"'{filename}' is not executable.")

if __name__ == "__main__":
    filename = input("Enter the filename: ")
    check_executable(filename)
