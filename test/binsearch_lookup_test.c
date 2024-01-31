#include <stdio.h>
#include <assert.h>
#include "binsearch_lookup.h"
#include "string_funs.h"

void test1() {
  struct LibEntry entries[] = {
    { .libname = "A", .libpath = "A-out" },
    { .libname = "C", .libpath = "C-out" },
    { .libname = "E", .libpath = "E-out" }
  };
  assert(streql("A-out", binsearch_lookup("A", entries, 3)));
  assert(streql("C-out", binsearch_lookup("C", entries, 3)));
  assert(streql("E-out", binsearch_lookup("E", entries, 3)));
  assert(NULL == binsearch_lookup("B", entries, 3));
  assert(NULL == binsearch_lookup("D", entries, 3));
  assert(NULL == binsearch_lookup("F", entries, 3));
  assert(NULL == binsearch_lookup("0", entries, 3));
}

void test2() {
  struct LibEntry entries[] = {
    { .libname = "LLVMgold.so", .libpath = "0" },
    { .libname = "libBrokenLocale.so", .libpath = "A" },
    { .libname = "libBrokenLocale.so.1", .libpath = "B" },
    { .libname = "libCGAL.so", .libpath = "C" },
    { .libname = "libCGAL.so.13", .libpath = "D" },
    { .libname = "libCGAL.so.13.0.3", .libpath = "E" },
    { .libname = "libCGAL_Core.so", .libpath = "F" },
    { .libname = "libCGAL_Core.so.13", .libpath = "G" },
    { .libname = "libCGAL_Core.so.13.0.3", .libpath = "H" },
  };
  assert(streql("0", binsearch_lookup("LLVMgold.so", entries, 9)));
  assert(NULL == binsearch_lookup("_int.so", entries, 9));
  assert(streql("A", binsearch_lookup("libBrokenLocale.so", entries, 9)));
  assert(streql("B", binsearch_lookup("libBrokenLocale.so.1", entries, 9)));
  assert(NULL == binsearch_lookup("libBrokenLocale.so.2", entries, 9));
  assert(streql("C", binsearch_lookup("libCGAL.so", entries, 9)));
  assert(streql("D", binsearch_lookup("libCGAL.so.13", entries, 9)));
  assert(streql("E", binsearch_lookup("libCGAL.so.13.0.3", entries, 9)));
  assert(NULL == binsearch_lookup("libCGAL.so.13.0.4", entries, 9));
  assert(streql("F", binsearch_lookup("libCGAL_Core.so", entries, 9)));
  assert(streql("G", binsearch_lookup("libCGAL_Core.so.13", entries, 9)));
  assert(streql("H", binsearch_lookup("libCGAL_Core.so.13.0.3", entries, 9)));
  assert(NULL == binsearch_lookup("libCGAL_Core.so.13.0.4", entries, 9));
}

int main() {
  test1();
  test2();
  printf("OK\n");
}