#include <stddef.h>

int strcmp (const char *p1, const char *p2);
int strncmp (const char *s1, const char *s2, size_t n);

int itoa(int value, char *sp, int radix);

char *my_strrchr(const char *haystack, char needle);
char *my_strchrnul(const char *haystack, char needle);
char *my_strncpy(char * destination, const char * source, size_t num);
int my_strlen(const char *str);

int streql(const char* str1, const char* str2);
int strneql(const char *str1, const char *str2, int n);
int strstartswith(const char *prefix, const char *str);