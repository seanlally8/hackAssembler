#include <ctype.h>
#include <stdio.h>
#include <math.h>

// An attempt at writing a hash function
unsigned long hash(unsigned char *str);

int main(void){
  unsigned char *string = (unsigned char *)"KBD";
  printf("%lu", hash(string) % 40);
  return 0;
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
