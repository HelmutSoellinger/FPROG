@echo off
echo Starting the Red-Black Tree Project in WSL...

:: Step 1: Compile the project using make inside WSL
echo Compiling the project using Make in WSL...
wsl make

:: Check if the executable was created
if not exist red_black_tree (
    echo Compilation failed! Exiting...
    exit /b 1
)
echo Compilation successful.

:: Step 2: Run the program in WSL
echo Running the program in WSL...
wsl ./red_black_tree

:: Check if output.txt was created
if not exist output.txt (
    echo Test failed: output.txt was not generated.
    exit /b 1
)
echo Test passed: output.txt was successfully generated.

:: Step 3: Clean up build files using make clean in WSL
echo Cleaning up build files using Make in WSL...
wsl make clean

echo Done.
