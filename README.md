# hackAssembler
This is a rudimentary assembler implemented in C according to the design specifications provided by [The Elements of Computing](http://f.javier.io/rep/books/The%20Elements%20of%20Computing%20Systems.pdf) by Noam Nisan and Shimon Schocken. The code needs cleaning/optimization but it is currently offering the most basic functionality, ie it can only handle decimal values and not symbols for A and L instructions (assembly instructions assigning an address to the A register), and it can't handle errors in code or user input values.

# Files and Directories
The file system is fairly standard for a C program (as I understand it). All .c files (files containing program logic) are in /src and all header files
(files containing the program API) are in /include. The /tests folder contains any .c files generated to run tests on the source code.

The file (compile_flags.txt) was generated to support in-line error checking on my lvim text editor.

# Usage
The program takes .asm file as input and outputs a .hack file containing the binary equivalent.

# Installation
1. Open terminal
2. Navigate to the directory where you'd like to install the hackAssembler.
```bash
cd file/path/to/directory
```
3. Clone the github repository
```bash
git clone <https://github.com/seanlally8/hackAssembler.git> .
```
4. 3. Navigate to the project root directory
```bash
cd assembler
```
5. Build hackAssembler
```bash
make hackAssembler
```
6. Run program (if you have .asm file written in the hackAssembly language, you can replace "Fill.asm" with the file path to your code.
```bash
./build/hackAssembler Fill.asm
```






