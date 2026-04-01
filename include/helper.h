// helper.h
#ifndef HELPER_H // check if myheader_h is not defined
#define HELPER_H // if not defined, define it now



// all your header file code (declarations, etc.) goes here
int stringCleaner(char *dirty_string);
char *findM(char *comp_string);
void memAlloc(char **list, char *string, int index, int increment);
void memFree(char **list, int index);
unsigned long hash(unsigned char *str);

#endif // ends the #ifndef block
