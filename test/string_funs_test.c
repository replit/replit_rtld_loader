#include "string_funs.h"
#include <assert.h>
#include <stdio.h>

void test_my_strrchr() {
  char str[] = "zbcabc";
  assert(&str[5] == my_strrchr(str, 'c'));
  assert(NULL == my_strrchr(str, 'e'));
  assert(&str[4] == my_strrchr(str, 'b'));
  assert(&str[3] == my_strrchr(str, 'a'));
  assert(&str[0] == my_strrchr(str, 'z'));
}

void test_my_strchrnul() {
  char str[] = "abc\0de";
  assert(&str[0] == my_strchrnul(str, 'a'));
  assert(&str[1] == my_strchrnul(str, 'b'));
  assert(&str[2] == my_strchrnul(str, 'c'));
  assert(&str[3] == my_strchrnul(str, 'd'));
  assert(&str[3] == my_strchrnul(str, 'e'));
  assert(&str[3] == my_strchrnul(str, 'f'));
}

void test_my_strncpy() {
  char dest1[6] = "XXXXX";
  char dest2[6] = "XXXXX";
  char dest3[6] = "XXXXX";
  char str[] = "abc";

  my_strncpy(dest1, str, 3);
  assert(streql(dest1, "abcXX"));

  my_strncpy(dest2, str, 5);
  assert(streql(dest2, "abc"));
  assert(dest2[3] == '\0');
  assert(dest2[4] == '\0');

  my_strncpy(dest3, str, 2);
  assert(streql(dest3, "abXXX"));
}

void test_my_strlen() {
  assert(3 == my_strlen("abc"));
  assert(5 == my_strlen("abcde"));
  assert(0 == my_strlen(""));
}

int main() {
  test_my_strrchr();
  test_my_strchrnul();
  test_my_strncpy();
  test_my_strlen();
  printf("OK\n");
}