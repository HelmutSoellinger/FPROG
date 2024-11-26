# Red-Black Tree Project

This project implements a persistent Red-Black Tree in C++, adhering to functional programming principles. 
It reads the content of `war_and_peace.txt`, tokenizes the text, inserts unique words into the tree, and 
writes the sorted list of words to `output.txt`.

## Requirements
- **C++ Compiler**: A C++17-compliant compiler (e.g., g++, clang++, or MSVC).
- **Make Utility**: Ensure `make` is installed (commonly available on Linux/Unix systems or through MinGW/MSYS2 on Windows).
- **Input File**: Ensure `war_and_peace.txt` is present in the same directory as the source code.

## How to Compile
1. Open a terminal or command prompt.
2. Navigate to the project directory.
3. Run the following command:

- - - - -
make
- - - - -

## How to Run
1. After compiling the project, execute the following command:

- - - - -
./red_black_tree
- - - - -

2. The program will process the input file `war_and_peace.txt` and generate the output file `output.txt` in the same directory.

## Cleaning Up
To remove compiled files and reset the directory:
1. Run the following command:

- - - - -
make clean
- - - - -

2. This will delete the executable `red_black_tree`.

## Automation Script (Windows Only)
You can also use the provided `run.bat` script to compile, run, and test the program in one step:
1. Double-click `run.bat` or execute it in the terminal:

- - - - -
run.bat
- - - - -

The script uses `make` to compile and clean up automatically.
