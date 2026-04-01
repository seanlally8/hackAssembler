// parser.h
#ifndef PARSER_H // Check if MYHEADER_H is NOT defined
#define PARSER_H // If not defined, define it now

#include <stdio.h>

// FUNCTIONS
FILE *openFile(char *ptr);
int hasMoreLines(FILE *file);
int advance(FILE *ptr, char *inst_string);
int instructionType(char *inst_string);
void symbol(int inst_type, char *temp, char *inst_string);
void dest(char *temp, char *current_inst);
void comp(char *temp, char *inst_string);
char *jump(char *temp, char *inst_string);


#endif // Ends the #ifndef block
