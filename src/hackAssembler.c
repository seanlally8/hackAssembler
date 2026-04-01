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
        strcpy(symbol_store[row], instruction);
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
    }

    else if (hasMoreLines(ptr) == 1){
      // close file
      for (int g = 0; g < row; g++){
        printf("%s\n", symbol_store[g]);
      }
      fclose(ptr);
      break;
    }
  }

  // POPULATE SYMBOL TABLE
  node *hash_table[100] = {0};
  int new_var = 16;
  unsigned char init_sym[23][7] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", 
                                    "R9", "R10", "R11", "R12", "R13", "R14", "R15","SP", 
                                    "LCL", "ARG", "THIS", "THAT", "SCREEN", "KBD"};
  unsigned char init_num[23][7] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", 
                          "11", "12", "13", "14", "15", "0", "1", "2", "3", "4", 
                          "16384", "24567"};
  int hashnum;
  int line_cntr = -1;
  char buffer[12] = {0};
  
  for (int i = 0; i < 23; i++) {
    hashnum = hash(init_sym[i]) % 100;
    hash_table[hashnum] = NULL;
    node *n = malloc(sizeof(node));
    strcpy(n->number, (const char *)init_num[i]);
    strcpy(n->symbol, (const char *)init_sym[i]);
    hash_table[hashnum] = n;
  }

  // FIRST PASS TO FIND L INSTRUCTIONS
  for (int k = 0; k < row; k++){
    int type = instructionType(symbol_store[k]);
    symbol(type, temp, symbol_store[k]);
    hashnum = hash((unsigned char *)temp) % 100;
    sprintf(buffer, "%i", line_cntr + 1); 
    if (type == 1 && hash_table[hashnum] == NULL){
      node *n = malloc(sizeof(node));
      strcpy(n->number, buffer); 
      strcpy(n->symbol, temp);
      n->next = NULL;
      hash_table[hashnum] = n;
      memset(buffer, 0, 12);
      memset(temp, 0, 200);
    }
    else if(type == 1 && hash_table[hashnum] != NULL){
      node *n  = malloc(sizeof(node));
      strcpy(n->number, buffer);
      strcpy(n->symbol, temp);
      n->next = hash_table[hashnum];
      hash_table[hashnum] = n;
    }
    else if (type == 0 || type == 2) {
      line_cntr++;
    }
  }

  // SECOND PASS TO FIND A INSTRUCTIONS
  for (int p = 0; p < row ; p++){
    int type = instructionType(symbol_store[p]);
    symbol(type, temp, symbol_store[p]);
    hashnum = hash((unsigned char *)temp) % 100;
    if (type == 0 && hash_table[hashnum] != NULL){
      for (node *ptr = hash_table[hashnum]; ptr != NULL; ptr = ptr->next){
        if (strcmp(ptr->symbol, temp) == 0) {
          symbolBinary(tempcode, ptr->number);
          strcpy(binary_store[p], tempcode);
        }
      }
      memset(tempcode, 0, 17);
    }
    else if (type == 0 && hash_table[hashnum] == NULL) {
      sprintf(buffer, "%i", new_var);
      node *n = malloc(sizeof(node));
      strcpy(n->number, buffer);
      strcpy(n->symbol, temp);
      n->next = NULL;
      hash_table[hashnum] = n;
      symbolBinary(tempcode, hash_table[hashnum]->number);
      strcpy(binary_store[p], tempcode);
      new_var++;
      memset(tempcode, 0, 17);
      memset(buffer, 0, 12);
      free(n);
    }
    memset(temp, 0, 200);
  }





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


