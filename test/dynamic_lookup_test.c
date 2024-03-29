#include "dynamic_lookup.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Please provide libname and ld_library_path\n");
    exit(1);
  }

  const char* result = dynamic_lookup(argv[1], argv[2]);
  if (result != NULL) {
    printf("%s", result);
  }
}
