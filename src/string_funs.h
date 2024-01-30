#include <stddef.h>

int streql(const char* one, const char* other);
int strneql(const char *one, const char *other, int n);
int strstartswith(const char *str, const char *prefix);

int my_strcmp (const char *p1, const char *p2);
char *my_strrchr(const char *haystack, char needle);
char *my_strchrnul(const char *haystack, char needle);
char *my_strncpy(char * destination, const char * source, size_t num);
int my_strlen(const char *str);
int itoa(int value, char *sp, int radix);