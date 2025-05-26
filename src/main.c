/*
  This file contains the entry point for the Oxygen Interpreter.

  Its a basic shell for the backend of the language with helpful commands and
  basic argument processing.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");

  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *contents = malloc(fsize + 1);
  fread(contents, fsize, 1, file);
  fclose(file);

  contents[fsize] = 0;

  printf("%s", contents);

  free(contents);
  return 0;
}