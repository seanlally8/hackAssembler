#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "helper.h"

int main(int argc, char *argv[]) {
  // DECLARE GLOBAL VARIABLES //
  node *hash_table[178] = {0};
  unsigned char init_sym[23][7] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", 
                                    "R9", "R10", "R11", "R12", "R13", "R14", "R15","SP", 
                                    "LCL", "ARG", "THIS", "THAT", "SCREEN", "KBD"};
  unsigned char init_num[23][7] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", 
                          "11", "12", "13", "14", "15", "0", "1", "2", "3", "4", 
                          "16384", "24567"};

  char *temp = calloc(200, sizeof(char));
  if (temp == NULL){
    return 1;
  }
  char *tempcode = calloc(17, sizeof(char));
  if (temp == NULL){
    return 1;
  }
  char *instruction = calloc(200, sizeof(char));
  if (instruction == NULL){
    return 1;
  }
  char *newfilename = calloc(100, sizeof(char));
  if (newfilename == NULL){
      return 1;
    }
  char *filename = argv[1];
  char *hack = "hack";
  char buffer[50];

  int parse;
  int line = 0;
  int hashnum;

  /////////////////////////////////////////////////////
  // Check user CLI input //
  if (argc != 2){
    printf("Two arguments please.\n");
    return 1;
  }

  //// INITIALIZE HASH TABLE TO HOLD SYMBOLS AND CORRESPONDING ADDRESSES ////
  for (int i = 0; i < 23; i++) {
    hashnum = hash(init_sym[i]) % 178;
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


  //// OPEN INPUT OUTPUT FILES ////////////////////////
  FILE *ptr = openFile(filename);
  if (ptr == NULL) {
    printf("file could not be opened");
    return 1;
  }
  strcpy(newfilename, argv[1]);
  for (int t = 0; t < (int)strlen(argv[1]); t++){
    if (newfilename[t] == '.'){
      newfilename[t + 1] = '\0';
      strcat(newfilename, hack);
      break;
    }
  }
  FILE *hackptr = fopen(newfilename, "w");
  if (hackptr == NULL) {
    printf("file could not be opened");
    return 1;
  }
  
  //// PASS 1 - LOOK FOR L INSTRUCTION, ADD TO HASH TABLE
  line = -1;
  while (hasMoreLines(ptr) != 1){

    ///// ERROR CHECK //////
    if (hasMoreLines(ptr) == -1) {
      printf("Invalid code");
      return 1;
    }
    
    /// MORE LINES TO GO ///
    else if (hasMoreLines(ptr) == 0){

      /// ADVANCE TO NEXT INSTRUCTION ///
      int error = advance(ptr, instruction);
      if (error == -1){
        printf("Undefined behavior in 'advance' function");
        return 1;
      }

      /// DETERMINE INSTRUCTION parse, EXTRACT SYMBOL, SET HASH NUMBER, CONVERT LINE NUMBER TO STRING///
      parse = instructionType(instruction);
      symbol(parse, temp, instruction);
      hashnum = hash((unsigned char *)temp) % 178;
      char line_str[12] = {0};
      sprintf(line_str, "%i", line + 1);


      /// IF A OR C INSTRUCTION TRACK LINE NUMBER ///
      if (parse != 1) {
        line++;
      }

      /// IF L INSTRUCTION: ADD TO HASH TABLE ///

      else if ( parse == 1 && hash_table[hashnum] == NULL){
        node *n = malloc(sizeof(node));
        strcpy(n->number, line_str);
        strcpy(n->symbol, temp);
        n->next = NULL;
        hash_table[hashnum] = n;
      }
      
      else if (parse == 1 && hash_table[hashnum] != NULL) {
        node *n = malloc(sizeof(node));
        strcpy(n->number, line_str);
        strcpy(n->symbol, temp);
        n->next = hash_table[hashnum];
        hash_table[hashnum] = n;
      }
      memset(temp, 0, 200);
      memset(instruction, 0, 200);
    }
  }

  /// PASS 2: FIND A INSTRUCTION (TRANSLATE DECIMAL VALUES, TRANSLATE PRE_EXISTING VALUES, STORE
  /// AND TRANSLATE NEW VALUES) AND C INSTRUCTIONS ////
  
  int var = 16;
  int button = 0;
  rewind(hackptr);
  rewind(ptr);
  while (hasMoreLines(ptr) != 1){

    ///// ERROR CHECK //////
    if (hasMoreLines(ptr) == -1) {
      printf("Invalid code");
      return 1;
    }

    /// MORE LINES TO GO ///
    else if (hasMoreLines(ptr) == 0){

      /// ADVANCE TO NEXT INSTRUCTION ///
      int error = advance(ptr, instruction);
      if (error == -1){
        printf("Undefined behavior in 'advance' function");
        return 1;
      }
      /// DETERMINE INSTRUCTION parse, EXTRACT SYMBOL, SET HASH NUMBER, CONVERT LINE NUMBER TO STRING///

      parse = instructionType(instruction);
      symbol(parse, temp, instruction);
      hashnum = hash((unsigned char *)temp) % 178;
      char *hold;
      strtol(temp, &hold, 10);

      if (parse == 0 && strcmp(temp, hold) != 0) {
        symbolBinary(tempcode, temp);
        fputs(tempcode, hackptr);
        fputc('\n', hackptr);
      }

      else if (parse == 0 && hash_table[hashnum] != NULL) {
        for (node *ptr = hash_table[hashnum]; ptr != NULL; ptr = ptr->next){
          if (strcmp(ptr->symbol, temp) == 0) {
            symbolBinary(tempcode, ptr->number);
            fputs(tempcode, hackptr);
            fputc('\n', hackptr);
            button = 1;
          } 
        }
        if (button == 0) {
          sprintf(buffer, "%i", var);
          node *n = malloc(sizeof(node));
          strcpy(n->symbol, temp);
          strcpy(n->number, buffer);
          n->next = hash_table[hashnum];
          hash_table[hashnum] = n;
          symbolBinary(tempcode, buffer);
          fputs(tempcode, hackptr);
          fputc('\n', hackptr);
          var++;
        }
      }
     
      else if (parse == 0 && hash_table[hashnum] == NULL) {
        sprintf(buffer, "%i", var);
        node *n = malloc(sizeof(node));
        strcpy(n->symbol, temp);
        strcpy(n->number, buffer);
        n->next = NULL;
        hash_table[hashnum] = n;
        symbolBinary(tempcode, buffer);
        fputs(tempcode, hackptr);
        fputc('\n', hackptr);
        var++;
        rewind(hackptr);
      }

      else if (parse == 2){
        strcpy(buffer, "111");
        comp(temp, instruction);
        char *avalue = findM(temp);
        compBinary(tempcode, temp);
        strcat(buffer, avalue);
        strcat(buffer, tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);
 
        dest(temp, instruction);
        destBinary(tempcode, temp);
        strcat(buffer, tempcode);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);

        jump(temp, instruction);
        jumpBinary(tempcode, temp);
        strcat(buffer, tempcode);
        fputs(buffer, hackptr);
        fputc('\n', hackptr);
        memset(temp, 0, 200);
        memset(tempcode, 0, 17);
      }
      memset(temp, 0, 200);
      memset(tempcode, 0, 17);
      memset(buffer, 0, 50);
    } 
  } 
  

  for (int j = 0; j < 178; j++){
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
  fclose(ptr);
  fclose(hackptr);
  return 0;
}

