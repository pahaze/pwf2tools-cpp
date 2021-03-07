#ifndef PTR2COMMON_H
#define PTR2COMMON_H

#include <string.h>
#include <stdio.h>

static inline bool streq_s(const char *s1, const char *s2) {
  return (strcmp(s1,s2) == 0) ? true : false;
}

static inline bool streq(const char *s1, const char *s2) {
  return (strcasecmp(s1,s2) == 0) ? true : false;
}

static inline int getfilesize(FILE *f) {
  long spos = ftell(f);
  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  fseek(f, 0, SEEK_SET);
  return int(len);
}

#endif

