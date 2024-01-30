#include "string_funs.h"

int streql(const char* one, const char* other) {
  while (1) {
    char oneChr = *one;
    char otherChr = *other;
    if (oneChr != otherChr) {
      return 0;
    }
    if (oneChr == '\0') {
      return 1;
    }
    one++;
    other++;
  }
}

int strneql(const char *one, const char *other, int n) {
  for (int i = 0; i < n; i++) {
    char oneChr = one[i];
    char otherChr = other[i];
    if (oneChr != otherChr) {
      return 0;
    }
    if (oneChr == '\0') {
      return 1;
    }
  }

  return 1;
}

/*
stolen from https://codebrowser.dev/glibc/glibc/string/strcmp.c.html
*/
int my_strcmp (const char *p1, const char *p2)
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


char *my_strrchr(const char *haystack, char needle) {
  int len = my_strlen(haystack);
  for (int i = len - 1; i > 0; i--) {
    char chr = haystack[i];
    if (chr == needle) {
      return (char *)&haystack[i];
    }
  }
  return NULL;
}

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

int strstartswith(const char *prefix, const char *str) {
  while (1) {
    char strCh = *str;
    char prefixCh = *prefix;
    if (prefixCh == '\0') {
      return 1;
    }
    if (strCh != prefixCh) {
      return 0;
    }
    if (strCh == '\0') {
      return 1;
    }
    str++;
    prefix++;
  }
}

// from https://stackoverflow.com/a/12386915
// Yet, another good itoa implementation
// returns: the length of the number string
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