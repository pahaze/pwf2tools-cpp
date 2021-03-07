#ifndef PTR2CMD_H
#define PTR2CMD_H

#include "pwf2common.h"

#ifndef MAX_ALIASES
  #define MAX_ALIASES 3
#endif

typedef int (*cmd_function_t)(int argc, char *args[]);

struct cmd_t {
  const char *name;
  const char *usage;
  const char *desc;
  const char *aliases[MAX_ALIASES];
  int naliases;
  cmd_function_t fn;

  bool matches(const char *s) {
    if(streq(s, name)) return true;
    for(int i = 0; i < naliases; i += 1) {
      if(streq(s, aliases[i])) return true;
    }
    return false;
  };
  void printhelp(const char *prefix) {
    printf("%s%s %s\n", prefix, this->name, this->usage);
    printf("   %s\n", this->desc);
    if(naliases > 0) {
      printf("   ALIASES: ");
      for(int i = 0 ; i < naliases; i += 1) {
        printf("%s ", aliases[i]);
      }
    }
    printf("\n");
  }
  inline int exec(int argc, char *args[]) {
    return this->fn(argc, args);
  }
};

#endif
