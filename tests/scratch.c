#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "helper.h"
#include "code.h"

int main() {
  FILE *file = fopen("../Fill.asm", "r");
  int c;
  while ((c = fgetc(file)) != EOF){
    printf("%i ", c);
    if (c == '\n'){
      printf("\n");
    }
  }
}

