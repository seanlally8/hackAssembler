#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "helper.h"
#include "code.h"


int main(int argc, char *argv[]){

  if (argc != 2){
    printf("Two arguments please.\n");
    exit(1);
  }

  int bool = 0;
  int memory_increment = 1;
  int main_count = 0;
  int sym_count = 0;
  int binary_count = 0;

  char **main_list =calloc(memory_increment, sizeof(char *));
  char **sym_list =calloc(memory_increment, sizeof(char *));
  char **binary_list =calloc(memory_increment, sizeof(char *));

  char *temp =calloc(50, sizeof(char));
  char *inst_string =calloc(50, sizeof(char));
  char *sym_string =calloc(50, sizeof(char));

  FILE *ptr = openFile(argv[1]);

  while (bool == 0){
    if (hasMoreLines(ptr) == 1){
      printf("No more instructions");
      exit(1);
    }
    else {
      memory_increment++;
      advance(ptr, inst_string);
      memAlloc(main_list, inst_string, main_count, memory_increment);
      main_count++;

      int inst_type = instructionType(inst_string);

      if (inst_type == 0 || inst_type == 1){
        sym_string = symbol(inst_type, temp, inst_string);
        memAlloc(sym_list, sym_string, sym_count, memory_increment);
        sym_count++;
        printf("sym_string: %s\n", sym_string);
      }
      
      printf("main string: %s\n", inst_string);

      if (inst_type == 2){
        char *dest_string = dest(temp, inst_string);
        printf("dest: %s\n", dest_string);


        char *dest_binary = destBinary(temp, dest_string);
        printf("dest_binary: %s\n", dest_binary);
        memAlloc(binary_list, dest_binary, binary_count, memory_increment);
        binary_count++;

        char *comp_string = comp(temp, inst_string);
        printf("comp: %s\n", comp_string);
        
        char *jump_string = jump(temp, inst_string);
        printf("jump: %s\n", jump_string);
        
      }
    }
  }
  fclose(ptr);
  memFree(main_list, main_count);
  memFree(sym_list, sym_count);
  memFree(binary_list, binary_count);
  free(inst_string);
  free(temp);
}
