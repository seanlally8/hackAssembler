# hackAssembler
This is a rudimentary assembler implemented in C according to the design specifications provided by The Elements of Computing by Noam Nisan and Shimon Schocken. The code needs cleaning/optimization but it is currently offering the most basic functionality, ie it can only handle decimal values and not symbols for A and L instructions (assembly instructions assigning an address to the A register), and it can't handle errors in code or user input values.

The file system is fairly standard for a C program (as I understand it). All .c files (files containing program logic) are in /src and all header files
(files containing the program API, list of functions and global variables) are in /include. The /tests folder contains any .c files generated to run tests on the source code. 

# Usage
The program takes .asm file as input and outputs a .hack file containing the binary equivalent.


