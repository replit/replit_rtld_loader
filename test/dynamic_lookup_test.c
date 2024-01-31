#include <stdio.h>
#include <stdlib.h>
#include "dynamic_lookup.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Please provide libname and ld_library_path\n");
    exit(1);
  }

  char *result = dynamic_lookup(argv[1], argv[2]);
  if (result != NULL) {
    printf("%s", dynamic_lookup(argv[1], argv[2]));
  }
}