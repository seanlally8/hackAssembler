// Parser for HackAssembler. Finding instructions and organizing them according to type. Then parsing fields within instructions.
// The current version assumes a perfect user who never veers from the Hack Assembly Language specification. CL0command for specification:
// okular --page 152 '/home/sean/Documents/Personal/Study/ComputerArchitecture/nand2tetris/Noam Nisan, Shimon Schocken - The Elements of Computing Systems, second edition_ Building a Modern Computer from First Principles (2021, The MIT Press) - libgen.li.pdf'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "helper.h"

// Conctructor/initializer. Open the file provided by the 2nd command line argument

FILE *openFile(char *filename) {
  FILE *ptr = fopen(filename, "r");
  return ptr;
}

// hasMoreLines. Check to see if there are more lines in document. Function returns non zero number if
// EOF reached. RETURNS -1 if error.
int hasMoreLines(FILE *ptr){
  fpos_t pos;
  fgetpos(ptr, &pos);
  int bool = 0;
  char buffer[200] = {0};
  while (bool == 0){
    fgets(buffer, 200, ptr);
    if (feof(ptr)){
      return 1;
    }
    for (int i = 0; i < (int)strlen(buffer); i++){
      if (buffer[i] == ' '){
        while (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\r'){
          buffer[i] = fgetc(ptr);
        }
      }
      else if (buffer[i] == '/'){
        if (buffer[i] == '/'){
          while (buffer[i] != '\n' && buffer[i] != '\r'){
            buffer[i] = fgetc(ptr);
          }
        }
        else{ 
          printf("Invalid character: single backslash not permissible\n");
          return -1;
        }
      }
      else if (buffer[i] =='@' || buffer[i] == '(' || buffer[i] == '0' || buffer[i] == '1' || 
              buffer[i] == '-' || buffer[i] == 'D' || buffer[i] == 'A' || buffer[i] == '!' || buffer[i] == 'M'){
        fsetpos(ptr, &pos);
        return 0;
      }
    }
  }
  fsetpos(ptr, &pos);
  return 0;
}

// advance. Skip over white space and comments. Reads next instruction from input and makes it 
// current instruction. called only if hasMoreLines is true. 
// RETURNS -1 when error occurs; 1 when EOF; and 0 when instruction is found;
int advance(FILE *ptr, char *inst_string){
  int m = 0;
  int bool = 0;
  char buffer[200];
  while (bool == 0){
    fgets(buffer, 200, ptr);
    if (feof(ptr)){
      return 1;
    }
    for (int i = 0; i < (int)strlen(buffer); i++){
      if (buffer[i] == '/'){
        if (buffer[i + 1] != '/'){
          printf("Must have 2 backslashes");
        }
        else{
          break;
        }
      }
      else if (buffer[i] == ' ') {        
        continue;
      }
      else if (buffer[i] == '\n'){
        break;
      }
      else if (buffer[i]=='@' || buffer[i]== '(' || buffer[i]== '0' || buffer[i]== '1' || buffer[i]== '-' || 
                                  buffer[i]== 'D' || buffer[i]== 'A' || buffer[i]== '!' || buffer[i]== 'M'){
        for (int k = 0; k < (int)strlen(buffer) + 1; k++){
          if (buffer[k] == ' ' || buffer[k] == (char)13 || buffer[k] == (char)10){
            continue;
          }
          else{
            inst_string[m] = buffer[k];
            m++;
          }
        }
      }
      return 0;
    }
  }
  printf("Undefined behavior");
  return -1;
}

// instructionType. Returns 3 constants refering to A instructions buffer[i]instructions and L instructions 
int instructionType(char *inst_string){
  int inst_type = 9;
  if (inst_string[0] == '@')
  {
    inst_type = 0;
  }
  else if (inst_string[0] == '(')
  {
     inst_type = 1;
  }
  else if (inst_string[0] == '0' || inst_string[0] == '1' || 
            inst_string[0] == '-' || inst_string[0] == 'D' || 
            inst_string[0] == 'A' || inst_string[0] == '!' || inst_string[0] == 'M')
  {
    inst_type = 2;
  }
  return inst_type;
}

// symbol. if current instruction is (xxx) returns xxx, as a string. If @xxx, returns xxx as a string. Called if L or A instruction type.
void symbol(int inst_type, char *temp, char *inst_string){
  int length = strcspn(inst_string, ")");
  int size = strlen(inst_string); 
  if (inst_type == 0){
    for (int k = 0; k < size; k++){
      temp[k] = inst_string[k + 1];
    }
    temp[size] = '\0'; 
  }
  else if (inst_type == 1){
    for (int k = 0; k < length; k++){  
      temp[k] = inst_string[k + 1];
    }
    temp[length - 1] = '\0'; 
  }
}

// dest. Returns the symbolic dest part of C-instruction. Returns string
void dest(char *temp, char *inst_string){
  int length = strlen(inst_string);
  for (int k = 0; k < length; k++){
    if (inst_string[k] == '='){
      for (int m = 0; m < k; m++){
        temp[m] = inst_string[m];
      }
    }
    else{
      continue;
    }
  }
}

// comp Same but for comp. Returns String.
void comp(char *temp, char *inst_string){
  int length = strlen(inst_string);
  int eq = 0;
  int count = 0;
  for (int k = 0; k < length; k++){
    count = 0;
    // FIRST CONDITION
    if (inst_string[k] == '='){
      eq = k;
      for (int m = k + 1; m < length; m++){
        if (inst_string[m] == ';'){
          temp[count + 1] = '\0';
          return;
        }
        else{
          temp[count] = inst_string[m];
          count++;
        }
      }
    }
    // SECOND CONDITION
    else if (inst_string[k] == ';'){
      if (eq != 0) {
        for (int m = eq + 1; m < k; m++){
          temp[count] = inst_string[m];
          count++;
        }
        temp[count + 1] = '\0';
        return;
      }
      else {
        for (int f = 0; f < k; f++){
          temp[count] = inst_string[f];
          count++;
        }
        temp[count + 1] = '\0';
        return;
      }
    }
    // THIRD CONDITION
    else if (k == length - 1){
      if (eq != 0){
        for (int m = eq + 1; m < k; m++){
          temp[count] = inst_string[m];
          count++;
        }
      }
      else{
        for (int m = 0; m < length; m++){
          temp[count] = inst_string[m];
          count++;
        }
        temp[count + 1] = '\0';
        return;
      }
    }
  }
}

// jump. Same but for jump. Returns string.
char *jump(char *temp, char* inst_string){
  int util = 0;
  int length = strlen(inst_string);
  for (int k = 0; k < length; k++){
    int count = 0;
    if (inst_string[k] == ';'){
      util = 1;
      for (int m = k + 1; m < length; m++){
        temp[count] = inst_string[m];
        count++;
      }
    }
  }
  if (util == 0){
    return NULL;
  }
  else{
    return temp;
  }
}
