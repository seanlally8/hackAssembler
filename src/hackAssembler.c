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

  char *newfilename = calloc(100, sizeof(char));
  char *filename = argv[1];

  FILE *ptr = openFile(filename);
  if (ptr == NULL) {
    printf("file could not be opened");
    return 1;
  }

  char binary_store[100][200] = {0};
  int row = 0;
  while (1){
    if (hasMoreLines(ptr) == -1) {
      printf("Invalid code");
      return 1;
    }
    
    else if (hasMoreLines(ptr) == 0){
      // heart of the program.
      char *instruction = calloc(200, sizeof(char));
      char *temp = calloc(200, sizeof(char));
      char *tempcode = calloc(200, sizeof(char));


      int error = advance(ptr, instruction);
      if (error == -1){
        printf("Undefined behavior in 'advance' function");
        return 1;
      }

      int parse = instructionType(instruction);

      if (parse == 0 || parse == 1){
        symbol(parse, temp, instruction);
        symbolBinary(tempcode, temp);
        strcpy(binary_store[row], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 200);
      }

      else if (parse == 2){
        strcpy(binary_store[row], "111");
        comp(temp, instruction);
        char *avalue = findM(temp);
        compBinary(tempcode, temp);
        strcat(binary_store[row], avalue);
        strcat(binary_store[row], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 200);

 
        dest(temp, instruction);
        destBinary(tempcode, temp);
        strcat(binary_store[row], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 200);

        jump(temp, instruction);
        jumpBinary(tempcode, temp);
        strcat(binary_store[row], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 200);
        row++;
      }
      free(instruction);
      free(temp);
      free(tempcode);
    }

    else if (hasMoreLines(ptr) == 1){
      // close file
      fclose(ptr);
      break;
    }
  }
  
  char *hack = "hack";
  strcpy(newfilename, argv[1]);
  for (int t = 0; t < (int)strlen(argv[1]); t++){
    if (newfilename[t] == '.'){
      newfilename[t + 1] = '\0';
      strcat(newfilename, hack);
      printf("%s", newfilename);
      break;
    }
  }
  FILE *hackptr = fopen(newfilename, "w");
  for (int z = 0; z < row; z++){ 
    fputs(binary_store[z], hackptr);
    fputc('\n', hackptr);
  }
  fclose(hackptr);
  return EXIT_SUCCESS;
}


