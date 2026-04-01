#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "helper.h"

int main(int argc, char *argv[])
{
  
  if (argc != 2){
    printf("Two arguments please.\n");
    return 1;
  }

  char *instruction = calloc(200, sizeof(char));
  if (instruction == NULL){
    return 1;
  }
  char *temp = calloc(200, sizeof(char));
  if (temp == NULL){
    return 1;
  }
  char *tempcode = calloc(17, sizeof(char));
  if (temp == NULL){
    return 1;
  }
  char *newfilename = calloc(100, sizeof(char));
  if (newfilename == NULL){
    return 1;
  }

  char *filename = argv[1];
  int parse;
  char binary_store[100][200] = {0};
  char symbol_store[100][200] = {0};
  int row = 0;

  FILE *ptr = openFile(filename);
  if (ptr == NULL) {
    printf("file could not be opened");
    return 1;
  }

  while (1){
    if (hasMoreLines(ptr) == -1) {
      printf("Invalid code");
      return 1;
    }
    
    else if (hasMoreLines(ptr) == 0){
      // heart of the program.


      int error = advance(ptr, instruction);
      if (error == -1){
        printf("Undefined behavior in 'advance' function");
        return 1;
      }

      parse = instructionType(instruction);
      strcpy(symbol_store[row], instruction);

      if (parse == 0){
        // Keep a row in the final instruction set for this A Instruction; we will translate later using
        // symbol table
        row++;
      }

      else if (parse == 2){
        strcpy(binary_store[row], "111");
        comp(temp, instruction);
        char *avalue = findM(temp);
        compBinary(tempcode, temp);
        strcat(binary_store[row], avalue);
        strcat(binary_store[row], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);

 
        dest(temp, instruction);
        destBinary(tempcode, temp);
        strcat(binary_store[row], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);

        jump(temp, instruction);
        jumpBinary(tempcode, temp);
        strcat(binary_store[row], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);
        row++;
      }
      memset(instruction, 0, 200);
    }

    else if (hasMoreLines(ptr) == 1){
      // close file
      fclose(ptr);
      break;
    }
  }

  // POPULATE SYMBOL TABLE





  // FINAL STEP. Generate new file name then write the binary code to file.
  char *hack = "hack";
  strcpy(newfilename, argv[1]);
  for (int t = 0; t < (int)strlen(argv[1]); t++){
    if (newfilename[t] == '.'){
      newfilename[t + 1] = '\0';
      strcat(newfilename, hack);
      printf("%s\n", newfilename);
      break;
    }
  }
  FILE *hackptr = fopen(newfilename, "w");
  for (int h = 0; h < row; h++){
    printf("%s ", binary_store[h]);
    printf("%i\n", h);
  }
  for (int z = 0; z < row; z++){ 
    fputs(binary_store[z], hackptr);
    fputc('\n', hackptr);
  }

  fclose(hackptr);
  free(instruction);
  free(temp);
  free(tempcode);
  free(newfilename);
  return EXIT_SUCCESS;
}


