#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stringCleaner(char *dirty_string){
  for (int m = 0; m < 100; m++){
    if ((int)dirty_string[m] == 13 || (int)dirty_string[m] == 10){
      dirty_string[m] = 0;
    }
  }
  return 0;
}

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

void memAlloc(char **list, char *string, int index, int increment){
  list[index] = malloc(((int)strlen(string) + 1) * sizeof(char));
  sprintf(list[index], "%s", string);
  list = realloc(list, (increment + 1) * sizeof(char *));
}

void memFree(char **list, int index){
  for (int m = 0; m <= index; m++){
    free(list[m]);
  }
  free(list);
}
