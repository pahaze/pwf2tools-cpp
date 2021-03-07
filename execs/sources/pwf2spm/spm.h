#ifndef PTR2_SPM_H
#define PTR2_SPM_H

#include "types.h"

namespace spm {
struct polygonheader_t {
    u32 unk1[11];
    u32 count_vertices;
    u32 unk2[13];
    u32 format;
    u64 key;
};

bool check_install();
bool checkheader(const void *spm);
int getpolygoncount(const void *spm, int len);
bool getpolygonbyindex(const void *spm, int len, int index, polygonheader_t **out_polygon);
u64 tex0frompolygon(polygonheader_t *polygon);

}

#endif // PTR2_SPM_H
