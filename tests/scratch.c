#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "helper.h"
#include "code.h"

int main() {
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
    if (hash_table[hashnum] == NULL){
      node *n = malloc(sizeof(node));
      strcpy(n->number, (const char *)init_num[i]);
      strcpy(n->symbol, (const char *)init_sym[i]);
      n->next = NULL;
      hash_table[hashnum] = n;
    }
    else {
      node *n = malloc(sizeof(node));
      strcpy(n->number, (const char *)init_num[i]);
      strcpy(n->symbol, (const char *)init_sym[i]);
      n->next = hash_table[hashnum];
      hash_table[hashnum] = n;
    }
  }
  for (int i = 0; i < 100; i++) {
    if (hash_table[i] != NULL){
      for (node *ptr = hash_table[i]; ptr != NULL; ptr = ptr->next){
        printf("%s %s\n", ptr->number, ptr->symbol);
      }
    }
  }
}

