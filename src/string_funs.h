#include <stddef.h>

#define MAX_DECIMAL_INT_LEN 12
/*
All string functions are vendored or DIY.
*/

// Functions copied verbatim from glibc
int strcmp(const char* p1, const char* p2);
int strncmp(const char* s1, const char* s2, size_t n);

// Convert an int to a string. For display purposes.
int itoa(int value, char* sp, int radix);

// DIY versions of functions in string.h
// Didn't copy them because the code is too complicated
char* my_strrchr(const char* haystack, char needle);
char* my_strchrnul(const char* haystack, char needle);
char* my_strncpy(char* destination, const char* source, size_t num);
int my_strlen(const char* str);

// Convinience string functions
int streql(const char* str1, const char* str2);
int strneql(const char* str1, const char* str2, int n);
int strstartswith(const char* prefix, const char* str);