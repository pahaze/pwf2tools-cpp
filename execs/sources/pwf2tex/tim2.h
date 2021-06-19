#ifndef PS2_TIM2_H
#define PS2_TIM2_H

#include "types.h"

namespace tim2 {

struct segment_t {
  u32 totalsize;
  u32 palettesize;
  u32 imagesize;
  u16 offset_imagedata;  // From start of segment
  u16 color_entries;     // ??? only for 8 bpp or less???
  u8 format;
  u8 mipmap_count;
  u8 clutformat;
  u8 depth;
  u16 width;
  u16 height;

  /* These are PS2 Registers */
  u64 tex0;
  u64 tex1;
  u32 gsRegs;   // I Really don't know
  u32 texclut;  // lower 32 bits of GsTexClut I guess...

  /* userdata and pixel data comes after */
};

struct header_t {
  byte magic[4];
  u16 version;
  u16 textures_count;
  u32 idk[2];
};

bool check_install();
bool checkheader(const byte magic[4]);
header_t *getheader(const void *tim2);
byte *getpixels(segment_t *segment);
int getsegmentbyindex(const void *tim2, int index, segment_t **out_segment);

}  // namespace tim2

#endif  // PS2_TIM2_H
