#include <stddef.h>
#include "binsearch_lookup.h"
#include "string_funs.h"

char *binsearch_lookup(const char *libname, struct LibEntry *entries, int entriesLength) {
  int left = 0;
  int right = entriesLength - 1;
  
  while (1) {
    int mid = left + (right - left) / 2; // same as (right + left) / 2
                                         // but prevents int overflow
    int result = strcmp(libname, entries[mid].libname);
    if (result == 0) {
      // found it
      return entries[mid].libpath;
    } 
    if (right == left) {
      // interval is empty and still didn't find it,
      // report not found.
      return NULL;
    }
    if (result > 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return NULL;
}