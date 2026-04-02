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
  char binary_store[32000][200] = {0};
  char symbol_store[32000][200] = {0};
  int row = 0;
  int line = 0;

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
      printf("%s\n", instruction);

      if (parse == 0) {
        // Keep a row in the final instruction set for this A Instruction; we will translate later using
        // symbol table
        row++;
        line++;
      }

      else if (parse == 1) {
        row++;
      }

      else if (parse == 2){
        strcpy(binary_store[line], "111");
        comp(temp, instruction);
        char *avalue = findM(temp);
        compBinary(tempcode, temp);
        strcat(binary_store[line], avalue);
        strcat(binary_store[line], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);

 
        dest(temp, instruction);
        destBinary(tempcode, temp);
        strcat(binary_store[line], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);

        jump(temp, instruction);
        jumpBinary(tempcode, temp);
        strcat(binary_store[line], tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);
        row++;
        line++;
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
  // POPULATE SYMBOL TABLE
  node *hash_table[1000] = {0};
  unsigned char init_sym[23][7] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", 
                                    "R9", "R10", "R11", "R12", "R13", "R14", "R15","SP", 
                                    "LCL", "ARG", "THIS", "THAT", "SCREEN", "KBD"};
  unsigned char init_num[23][7] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", 
                          "11", "12", "13", "14", "15", "0", "1", "2", "3", "4", 
                          "16384", "24567"};
  int hashnum;
  
  for (int i = 0; i < 23; i++) {
    hashnum = hash(init_sym[i]) % 1000;
    if (hash_table[hashnum] == NULL){
      node *n = malloc(sizeof(node));
      strcpy(n->number, (const char *)init_num[i]);
      strcpy(n->symbol, (const char *)init_sym[i]);
      n->next = NULL;
      hash_table[hashnum] = n;
    }
    else {
      node *f = malloc(sizeof(node));
      strcpy(f->number, (const char *)init_num[i]);
      strcpy(f->symbol, (const char *)init_sym[i]);
      f->next = hash_table[hashnum];
      hash_table[hashnum] = f;
    }
  }

  ////////////////// PREPARATORY TESTS////////////////////
 /* int c = 0;8
  for (int i = 0; i < row; i++){
    if (instructionType(symbol_store[i]) != 1) { 
      printf("%i symbol_store = %s\n", c, symbol_store[i]);
      c++;
    }
    else {
      printf("symbol_store = %s\n", symbol_store[i]);
    }
  }
  for (int i = 0; i < line; i++){
    printf("%i binary_store = %s\n", i, binary_store[i]);
  }
  for (int i = 0; i < 100; i++) {
    if (hash_table[i] != NULL){
      for (node *ptr = hash_table[i]; ptr != NULL; ptr = ptr->next){
        printf("hash number: %i             hash symbol: %s\n", i, ptr->symbol);
      }
    }
  }*/
  /////////////////////////////////////////////////////////

  // FIRST PASS TO FIND L INSTRUCTIONS
  int line_num = -1;
  for (int i = 0; i < row; i++) {

    int type = instructionType(symbol_store[i]);
    symbol(type, temp, symbol_store[i]);
    hashnum = hash((unsigned char *)temp) % 1000;
    char line_num_str[12] = {0};
    sprintf(line_num_str, "%i", line_num + 1);

    if (type != 1) {
      line_num++;
    }

    else if (type == 1 && hash_table[hashnum] == NULL){
      node *n = malloc(sizeof(node));
      strcpy(n->number, line_num_str);
      strcpy(n->symbol, temp);
      n->next = NULL;
      hash_table[hashnum] = n;
    }
    
    else if (type == 1 && hash_table[hashnum] != NULL) {
      node *n = malloc(sizeof(node));
      strcpy(n->number, line_num_str);
      strcpy(n->symbol, temp);
      n->next = hash_table[hashnum];
      hash_table[hashnum] = n;
    }
    memset(temp, 0, 200);
  }



  /*for (int g = 0; g < 100; g++){
    for (node *ptr = hash_table[g]; ptr != NULL; ptr = ptr->next){
      printf("Hash Number is %ul,             Address is %s,             Symbol is %s\n", g, ptr->number, ptr->symbol);
    }
  }*/



  // SECOND PASS TO FIND A INSTRUCTIONS AND TRANSLATE

  line_num = 0;
  int var = 16;
  for (int i = 0; i < row; i++) {

    int type = instructionType(symbol_store[i]);
    symbol(type, temp, symbol_store[i]);
    hashnum = hash((unsigned char *)temp) % 1000;
    char *hold;
    strtol(temp, &hold, 10);

    if (type == 0 && strcmp(temp, hold) != 0) {
      symbolBinary(tempcode, temp);
      strcpy(binary_store[line_num], tempcode);
      line_num++;
    }

    else if (type == 0 && hash_table[hashnum] != NULL) {
      for (node *ptr = hash_table[hashnum]; ptr != NULL; ptr = ptr->next){
        if (strcmp(ptr->symbol, temp) == 0) {
          symbolBinary(tempcode, ptr->number);
          strcpy(binary_store[line_num], tempcode);
        }
      }
      line_num++;
    }
   
    else if (type == 0 && hash_table[hashnum] == NULL) {
      printf("var value %i          ", var);
      char buffer[12] = {0};
      sprintf(buffer, "%i", var);
      node *n = malloc(sizeof(node));
      strcpy(n->symbol, temp);
      strcpy(n->number, buffer);
      n->next = NULL;
      hash_table[hashnum] = n;
      symbolBinary(tempcode, buffer);
      strcpy(binary_store[line_num], tempcode);
      line_num++;
      var++;
    }

    else if (type != 1) {
      line_num++;
    }
    memset(temp, 0, 200);
    memset(tempcode, 0, 17);
  }
  for (int g = 0; g < line; g++) {
    printf("%s\n", binary_store[g]);
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
  for (int i = 0; i < row; i++){
    if (hash_table[i] !=NULL){
      for (node *ptr = hash_table[1]; ptr != NULL; ptr = ptr->next) {
        printf("%s, %s", hash_table[i]->number, hash_table[i]->symbol);
      }
    }
  }
  for (int z = 0; z < row; z++){ 
    fputs(binary_store[z], hackptr);
    fputc('\n', hackptr);
  }

  fclose(hackptr);
  for (int j = 0; j < 100; j++){
    if (hash_table[j] != NULL){
      node *ptr = hash_table[j];
      while (ptr != NULL){
        node *next = ptr->next;
        free(ptr);
        ptr = next;
      }
    }
  }
  free(instruction);
  free(temp);
  free(tempcode);
  free(newfilename);
  return EXIT_SUCCESS;
}


