// code.h
#ifndef CODE_H // check if myheader_h is not defined
#define CODE_H // if not defined, define it now


// STRUCTS
typedef struct {
  char binary[18][7];
  char symbolic[18][4];
} comp_dict;

typedef struct {
  char binary[8][4];
  char symbolic[8][4];
} jump_dict;


// FUNCTIONS
void symbolBinary(char *temp, char * sym_string);
void destBinary(char *temp, char* dest_string);
void compBinary(char *temp, char* comp_string);
void jumpBinary(char *temp, char* jump_string);
#endif // ends the #ifndef block
