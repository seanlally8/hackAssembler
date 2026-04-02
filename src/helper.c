#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


char *findM(char *comp_string){
  for (int k = 0; k < (int)strlen(comp_string); k++){
    if (comp_string[k] == 'M'){
      return "1";
    }
    else{
      return "0";
    }
  }
  return "0";
}
  
//http://www.cse.yorku.ca/~oz/hash.html -- the djb2 hash function developed by Dan Bernstein
unsigned long hash(unsigned char *str){
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}
