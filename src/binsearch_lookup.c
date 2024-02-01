#include "binsearch_lookup.h"
#include <stddef.h>
#include "string_funs.h"

const char* binsearch_lookup(const char* libname,
                             const struct lib_entry* entries,
                             size_t entriesLength) {
  int left = 0;
  int right = entriesLength - 1;

  while (right >= left) {
    int mid = left + (right - left) / 2;  // same as (right + left) / 2
                                          // but prevents int overflow
    int result = strcmp(libname, entries[mid].libname);
    if (result == 0) {
      // found it
      return entries[mid].libpath;
    }
    if (result > 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return NULL;
}
