#ifndef PTR2_INT_H
#define PTR2_INT_H
#include "types.h"

#define INTHDR_MAGIC 0x44332211

#define INT_RESOURCE_END 0
#define INT_RESOURCE_TM0 1
#define INT_RESOURCE_SOUNDS 2
#define INT_RESOURCE_STAGE 3
#define INT_RESOURCE_HATCOLORBASE 4
#define HATCOLOR_RED 0
#define HATCOLOR_BLUE 1
#define HATCOLOR_PINK 2
#define HATCOLOR_YELLOW 3
#define INT_RESOURCE_TYPE_COUNT 8

namespace ptr2int {
extern const char *typenames[INT_RESOURCE_TYPE_COUNT];

struct header_t {
  u32 magic;
  u32 filecount;
  u32 resourcetype;
  u32 fntableoffset;
  u32 fntablesizeinbytes;
  u32 lzss_section_size;
  u32 unk[2];
};

extern const header_t nullhdr;

struct filename_entry_t {
  u32 offset;
  u32 sizeof_file;
};

struct lzss_header_t {
  u32 uncompressed_size;
  u32 compressed_size;
  byte data[0];
};

typedef header_t *header_pt;

inline bool checkinstall() {
  return (sizeof(header_t) == 0x20) || (sizeof(lzss_header_t) == 0x8);
}
bool checkheader(const void *mint);
header_t *getheader(const void *mint);
u32 *getfiledataoffsets(const void *mint);
filename_entry_t *getfilenameentries(const void *mint);
lzss_header_t *getlzssheader(const void *mint);
char *getfilenames(const void *mint);
header_t *getnextheader(const void *mint);

inline bool checkheader(const void *mint) {
  header_t *inthdr = header_pt(mint);
  return (inthdr->magic == INTHDR_MAGIC) ? true : false;
}

inline header_t *getheader(const void *mint) { return header_pt(mint); }

inline u32 *getfiledataoffsets(const void *mint) {
  header_t *hdr = getheader(mint);
  return (u32 *)(hdr + 1);
}

inline filename_entry_t *getfilenameentries(const void *mint) {
  header_t *hdr = getheader(mint);
  byte *bytes = (byte *)(hdr);
  return (filename_entry_t *)(bytes + hdr->fntableoffset);
}

inline lzss_header_t *getlzssheader(const void *mint) {
  header_t *hdr = getheader(mint);
  byte *bytes = (byte *)(hdr);
  return (lzss_header_t *)(bytes +
                           (hdr->fntableoffset + hdr->fntablesizeinbytes));
}

inline char *getfilenames(const void *mint) {
  header_t *hdr = getheader(mint);
  return (char *)(getfilenameentries(mint) + hdr->filecount);
}

inline header_t *getnextheader(const void *mint) {
  header_t *hdr = getheader(mint);
  byte *b = (byte *)(getlzssheader(mint));
  return header_pt(b + hdr->lzss_section_size);
}

};  // namespace ptr2int

#endif
