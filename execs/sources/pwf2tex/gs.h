#ifndef PTR2TEX_GS_H
#define PTR2TEX_GS_H

#include "types.h"

namespace gs {
extern u8 gsmem[1024*1024*4];

#define GS_TEX_32			 0
#define GS_TEX_24			 1
#define GS_TEX_16			 2
#define GS_TEX_16S			10
#define GS_TEX_8			19
#define GS_TEX_4			20
#define GS_TEX_8H			27
#define GS_TEX_4HL			36
#define GS_TEX_4HH			44

#define GS_CLUT_32			 0
#define GS_CLUT_16			 2
#define GS_CLUT_16S			10

#define gsargs int tbp, int tbw, int tx, int ty, int tw, int th, void *data
bool WriteTexture(int psm, gsargs);
void WriteTexture32(gsargs);
void WriteTexture16(gsargs);
void WriteTexture8(gsargs);
void WriteTexture4(gsargs);

bool ReadTexture(int psm, gsargs);
void ReadTexture32(gsargs);
void ReadTexture16(gsargs);
void ReadTexture8(gsargs);
void ReadTexture4(gsargs);

bool ReadCLUT(int psm, int cpsm, gsargs);
void ReadCLUT32_I8(gsargs);
void ReadCLUT16_I8(gsargs);
void ReadCLUT32_I4(gsargs);
void ReadCLUT16_I4(gsargs);

bool WriteCLUT(int psm, int cpsm, gsargs);
void WriteCLUT32_I8(gsargs);
void WriteCLUT16_I8(gsargs);
void WriteCLUT32_I4(gsargs);
void WriteCLUT16_I4(gsargs);

#undef gsargs

union tex0_t {
    tex0_t() {}
    tex0_t(u64 v) {
        this->value = v;
    }
    struct {
        u64 tb_addr:14;
        u64 tb_width:6;
        u64 psm:6;
        u64 tex_width:4; //exponent, (2 ^ tex_width) = texture_width
        u64 tex_height:4;
        u64 tex_cc:1;
        u64 tex_function:2;
        u64 cb_addr:14;
        u64 clut_pixmode:4;
        u64 clut_smode:1;
        u64 clut_offset:5;
        u64 clut_loadmode:3;
    };
    u64 value;

};

};


#endif // PTR2TEX_GS_H
