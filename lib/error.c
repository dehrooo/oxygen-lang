/*
    Error handling and formating
*/

#include <stdio.h>

void ErrorP(int line, char where, char *message) {
  printf("Error: %s\n\n   %d| %d\n", message, line,
          where); // todo implement where
};