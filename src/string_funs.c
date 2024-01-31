#include "string_funs.h"

/*
Taken from https://github.com/lattera/glibc/blob/master/string/strcmp.c
*/
int
strcmp (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;

  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
  return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}

/*
  Taken from https://github.com/lattera/glibc/blob/master/string/strncmp.c
*/
int
strncmp (const char *s1, const char *s2, size_t n)
{
  unsigned char c1 = '\0';
  unsigned char c2 = '\0';

  if (n >= 4)
    {
      size_t n4 = n >> 2;
      do
  {
    c1 = (unsigned char) *s1++;
    c2 = (unsigned char) *s2++;
    if (c1 == '\0' || c1 != c2)
      return c1 - c2;
    c1 = (unsigned char) *s1++;
    c2 = (unsigned char) *s2++;
    if (c1 == '\0' || c1 != c2)
      return c1 - c2;
    c1 = (unsigned char) *s1++;
    c2 = (unsigned char) *s2++;
    if (c1 == '\0' || c1 != c2)
      return c1 - c2;
    c1 = (unsigned char) *s1++;
    c2 = (unsigned char) *s2++;
    if (c1 == '\0' || c1 != c2)
      return c1 - c2;
  } while (--n4 > 0);
      n &= 3;
    }

  while (n > 0)
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0' || c1 != c2)
  return c1 - c2;
      n--;
    }

  return c1 - c2;
}

// Taken from https://stackoverflow.com/a/12386915
int itoa(int value, char *sp, int radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}

/*
These functions are DIY because the glibc versions are too complicated
*/

// https://linux.die.net/man/3/strrchr
char *my_strrchr(const char *haystack, char needle) {
  int len = my_strlen(haystack);
  for (int i = len - 1; i >= 0; i--) {
    char chr = haystack[i];
    if (chr == needle) {
      return (char *)&haystack[i];
    }
  }
  return NULL;
}

// https://linux.die.net/man/3/strchrnul
char *my_strchrnul(const char *haystack, char needle) {
  for (int i = 0; ; i++) {
    char chr = haystack[i];
    if (chr == needle) {
      return (char *)&haystack[i];
    }
    if (chr == '\0') {
      return (char *)&haystack[i];
    }
  }
}

// https://linux.die.net/man/3/strncpy
char *my_strncpy(char * destination, const char * source, size_t num) {
  int ended = 0;
  for (int i = 0; i < num; i++) {
    char chr = source[i];
    if (ended) {
      destination[i] = '\0';
      continue;
    }
    destination[i] = chr;
    if (chr == '\0') {
      ended = 1;
    }
  }

  return destination;
}

int my_strlen(const char *str) {
  int i = 0;
  while (str[i] != '\0') {
    i++;
  }
  return i;
}

/*
These are just conveniences
*/

int streql(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0;
}

int strneql(const char *str1, const char *str2, int n) {
  return strncmp(str1, str2, n) == 0;
}

int strstartswith(const char *prefix, const char *str) {
  return strneql(prefix, str, my_strlen(prefix));
}