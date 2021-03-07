#include "types.h"
#include "int.h"
namespace ptr2int {

const char *typenames[INT_RESOURCE_TYPE_COUNT] = {
  "END",
  "TEXTURES",
  "SOUNDS",
  "PROPS",
  "HAT_RED",
  "HAT_BLUE",
  "HAT_PINK",
  "HAT_YELL"
};

const header_t nullhdr = {
  INTHDR_MAGIC, 0, INT_RESOURCE_END, 0x20, 0, 0, {0,0}
};

}


