// This code conversts the symbolic fiels of comp into their binary equivalents
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "code.h"

void symbolBinary(char *temp, char *sym_string){
  int adder[16] = {0};
  int input = strtod(sym_string, NULL);
  int exponent = 0;

  for (int i = 15; i > -1 ; i--){
    adder[i] = pow(2, exponent);
    exponent++;
  }

  for (int g = 0; g < 16; g++){
    temp[g] = '0';
  }
  
  for (int j = 15; j > -1; j--){
    if (adder[j] > input) {
      for (int k = j + 1; k < 16; k++) {
        if (adder[k] < input) {
          temp[k] = '1';
          input = input - adder[k];
        }
        else if (adder[k] == input){
          temp[k] = '1';
          return;
        }
      }
    }
  }
  
}

void destBinary(char *temp, char* dest_string){
  temp[0] = '0'; temp[1] = '0'; temp[2] = '0'; temp[3] = 0;
  for (int m = 0; m < (int)strlen(dest_string); m++){
    if (dest_string[m] == 'A'){
      temp[0] = '1';
    }
    else if (dest_string[m] == 'D'){
      temp[1] = '1';
    }
    else if (dest_string[m] == 'M'){
      temp[2] = '1';
    }
  }
}

void compBinary(char *temp, char* comp_string){
  static comp_dict c = {
        .binary =  {"101010", "111111", "111010", "001100", "110000", "001101", 
                    "110001", "001111", "110011", "011111", "110111", "001110", 
                    "110010", "000010", "010011", "000111", "000000", "010101"},
        .symbolic = {"0", "1", "-1", "D", "A", "!D", "!A", "-D", "-A", "D+1", "A+1", 
                      "D-1", "A-1", "D+A", "D-A", "A-D", "D&A", "D|A"}
        };
  for (int k = 0; k < (int)strlen(comp_string); k++){
    if (comp_string[k] == 'M'){
      comp_string[k] = 'A';
      break;
    }
  }
  int m = 0;
  for (m = 0; m < 18; m++){
    if (strcmp(comp_string, c.symbolic[m]) == 0){
      strcpy(temp, c.binary[m]);
      return;
    }
  }
  printf("No such computation");
}

void jumpBinary(char *temp, char *jump_string){
// CREATE ANOTHER DICT FOR THE JUMP BINARY TO SYMBOLIC TABLE IN THE SPECIFICATION
  // REPEAT LOGIC FROM COMP. I wrote the logic for this but I cleaned up the code after
  // some AI queries.
  jump_dict j = {
    .binary = {"000", "001", "010", "011", "100", "101", "110", "111"},
    .symbolic ={"", "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP"}
  };

  for (int m = 0; m < 8; m++){
      if (strcmp(jump_string, j.symbolic[m]) == 0){
        strcpy(temp, j.binary[m]);
      }
  }
}



