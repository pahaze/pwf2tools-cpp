#include "gs.h"
#include "gstable.inc"

#include <stdio.h>

#define gsargs int tbp, int tbw, int tx, int ty, int tw, int th, void *data

namespace gs {

u8 gsmem[1024*1024*4];

#define CalcGSIndices(pw, ph, tbw, block_table, bw, bh, bp, cw) \
    int pageX = x / pw; \
    int pageY = y / ph; \
    int page = (pageY * tbw) + pageX; \
    int px = x - (pageX * pw); \
    int py = y - (pageY * ph); \
    int blockX = px / bw; \
    int blockY = py / bh; \
    int block = block_table[(blockY * bp) + blockX]; \
    int bx = px - (blockX * bw); \
    int by = py - (blockY * bh); \
    int column = by / cw; \
    int cx = bx; \
    int cy = by - (column * cw);

#define CalcGSIndex(startBlockPos, page, block, column, cw) \
    ((startBlockPos + (page * 2048) + (block * 64) + (column * 16) + cw) * 4)

#define cgi(cw) (CalcGSIndex(startblockpos, page, block, column, cw))

#define gshdr(t) \
    t *src = (t*)(data);

#define calchdr int startblockpos = tbp * 64;

#define gsloop \
    for(int y = ty; y < (ty + th); y += 1) \
    for(int x = tx; x < (tx + tw); x += 1)

#define gswrite(t, p, v) *(t*)(gsmem + (p)) = (v)
#define gsread(t, p) (*(t*)(gsmem + (p)))

int CalcGSIndex32(int tbp, int tbw, int x, int y) {
    calchdr;

    CalcGSIndices(64, 32, tbw, block32, 8, 8, 8, 2);
    int cw = columnWord32[cx + (cy * 8)];
    return cgi(cw);
}

int CalcGSIndex16(int tbp, int tbw, int x, int y) {
    calchdr;

    CalcGSIndices(64, 64, tbw, block16, 16, 8, 4, 2);
    int cw = columnWord16[cx + (cy * 16)];
    int ch = columnHalf16[cx + (cy * 16)];
    return (cgi(cw)) + (ch * 2);
}

int CalcGSIndex8(int tbp, int  tbw, int x, int y) {
    tbw >>= 1;
    calchdr;

    CalcGSIndices(128,64,tbw,block8,16,16,8,4);
    int cw = columnWord8[column & 1][cx + (cy * 16)];
    int cb = columnByte8[cx + (cy * 16)];
    return (cgi(cw)) + (cb);
}

int CalcGSIndex4(int tbp, int tbw, int x, int y, int *cb) {
    tbw >>= 1;
    calchdr;

    CalcGSIndices(128,128,tbw,block4,32,16,4,4);
    int cw = columnWord4[column & 1][cx + (cy * 32)];
    *cb = columnByte4[cx + (cy * 32)];

    return (cgi(cw)) + ((*cb) >> 1);
}

#define t(p, f) case p: f(tbp, tbw, tx, ty, tw, th, data); break;

#define psmerror(name, psm) fprintf(stderr, "%s: Unknown PSM Format: 0x%x\n", #name, psm);

bool WriteTexture(int psm, gsargs) {
    switch(psm) {
    t(GS_TEX_32, WriteTexture32);
    t(GS_TEX_16, WriteTexture16);
    t(GS_TEX_8,  WriteTexture8);
    t(GS_TEX_4,  WriteTexture4);
    default:
        psmerror(WriteTexture, psm);
        return false;
    }
    return true;
}

bool ReadTexture(int psm, gsargs) {
    switch(psm) {
    t(GS_TEX_32, ReadTexture32);
    t(GS_TEX_16, ReadTexture16);
    t(GS_TEX_8,  ReadTexture8);
    t(GS_TEX_4,  ReadTexture4);
    default:
        psmerror(ReadTexture, psm);
        return false;
    }
    return true;
}

bool ReadCLUT(int psm, int cpsm, gsargs) {
    switch(psm) {
    case GS_TEX_4:
        switch(cpsm) {
        t(GS_CLUT_32, ReadCLUT32_I4);
        t(GS_TEX_24,  ReadCLUT32_I4);
        t(GS_CLUT_16, ReadCLUT16_I4);
        default:
            psmerror(ReadCLUT_cpsm, cpsm);
            return false;
        }
	break;

    case GS_TEX_8:
	switch(cpsm) {
	t(GS_CLUT_32, ReadCLUT32_I8);
	t(GS_TEX_24, ReadCLUT32_I8);
	t(GS_CLUT_16, ReadCLUT16_I8);
	default:
	  psmerror(ReadCLUT_cpsm, cpsm);
	  return false;
	}
	break;

    default:
        psmerror(ReadCLUT_psm, psm);
        return false;
    }
    return true;
}

bool WriteCLUT(int psm, int cpsm, gsargs) {
  switch(psm) {
  case GS_TEX_4:
    switch(cpsm) {
    t(GS_CLUT_32, WriteCLUT32_I4);
    t(GS_TEX_24, WriteCLUT32_I4);
    t(GS_CLUT_16, WriteCLUT16_I4);
    default:
      psmerror(WriteCLUT_cpsm, cpsm);
      return false;
    }
    break;

  case GS_TEX_8:
    switch(cpsm) {
    t(GS_CLUT_32, WriteCLUT32_I8);
    t(GS_TEX_24, WriteCLUT32_I8);
    t(GS_CLUT_16, WriteCLUT16_I8);
    default:
      psmerror(WriteCLUT_cpsm, cpsm);
      return false;
    }
    break;

  default:
    psmerror(WriteCLUT_psm, psm);
    return false;
  }
  return true;
}

void ReadCLUT32_I8(gsargs) {
  int startBlockPos = tbp * 64;
  u32 *dst = (u32*)(data);
  u32 *src = (u32*)(gsmem + (startBlockPos * 4));
  int i;

  for(i = tx; i < (tx+tw); i += 1) {
    *dst = src[clutTableT32I8[i]];
    dst++;
  }
  return;
}

void ReadCLUT16_I8(gsargs) {
  int startBlockPos = tbp * 64;
  u16 *dst = (u16*)(data);
  u16 *src = (u16*)(gsmem + (startBlockPos * 4));
  int i;

  for(i = tx; i < (tx+tw); i += 1) {
    *dst = src[clutTableT16I8[i]];
    dst++;
  }
  return;
}

void ReadCLUT32_I4(gsargs) {
	int startBlockPos = tbp * 64;
	u32 *dst = (u32*)(data);
	u32 *src = (u32*)(gsmem + (startBlockPos * 4));
	int i;

	for(i = tx; i < (tx + tw); i += 1) {
        *dst = src[clutTableT32I4[i]];
		dst++;
	}
	return;
}

void ReadCLUT16_I4(gsargs) {
	int startBlockPos = tbp * 64;
	u16 *dst = (u16*)(data);
	u16 *src = (u16*)(gsmem + (startBlockPos * 4));
	int i;

	for(i = tx; i < (tx+tw); i += 1) {
        *dst = src[clutTableT16I4[i]];
		dst++;
	}
	return;
}

void WriteCLUT32_I8(gsargs) {
  int startBlockPos = tbp * 64;
  u32 *dst = (u32*)(data);
  u32 *src = (u32*)(gsmem + (startBlockPos * 4));
  int i;

  for(i=tx; i<(tx+tw);i+=1) {
    src[clutTableT32I8[i]] = *dst;
    dst++;
  }
  return;
}

void WriteCLUT16_I8(gsargs) {
  int startBlockPos = tbp * 64;
  u16 *dst = (u16*)(data);
  u16 *src = (u16*)(gsmem + (startBlockPos * 4));
  int i;

  for(i=tx;i<(tx+tw);i+=1) {
    src[clutTableT16I8[i]] = *dst;
    dst++;
  }
  return;
}

void WriteCLUT32_I4(gsargs) {
  int startBlockPos = tbp * 64;
  u32 *dst = (u32*)(data);
  u32 *src = (u32*)(gsmem + (startBlockPos * 4));
  int i;

  for(i = tx; i < (tx + tw); i += 1) {
    src[clutTableT32I4[i]] = *dst;
    dst++;
  }
  return;
}

void WriteCLUT16_I4(gsargs) {
  int startBlockPos = tbp * 64;
  u16 *dst = (u16*)(data);
  u16 *src = (u16*)(gsmem + (startBlockPos * 4));
  int i;

  for(i = tx; i < (tx + tw); i += 1) {
    src[clutTableT16I4[i]] = *dst;
    dst ++;
  }
  return;
}

void WriteTexture32(gsargs) {
    gshdr(u32);
    gsloop {
        gswrite(u32, CalcGSIndex32(tbp, tbw, x, y), *src);
        src++;
    }
}

void ReadTexture32(gsargs) {
    gshdr(u32);
    gsloop {
        *src = gsread(u32, CalcGSIndex32(tbp, tbw, x, y));
        src++;
    }
}

void WriteTexture16(gsargs) {
    gshdr(u16);
    gsloop {
        gswrite(u16, CalcGSIndex16(tbp, tbw, x, y), *src);
        src++;
    }
}

void ReadTexture16(gsargs) {
    gshdr(u16);
    gsloop {
        *src = gsread(u16, CalcGSIndex16(tbp, tbw, x, y));
        src++;
    }
}

void WriteTexture8(gsargs) {
  gshdr(u8);
  gsloop {
    gswrite(u8, CalcGSIndex8(tbp, tbw, x, y), *src);
    src++;
  }
}

void ReadTexture8(gsargs) {
    gshdr(u8);
    gsloop {
        *src = gsread(u8, CalcGSIndex8(tbp,tbw,x,y));
        src++;
    }
}

void WriteTexture4(gsargs) {
    gshdr(u8);
    bool odd = false;
    gsloop {
        int cb;
        u8 *dst = gsmem + CalcGSIndex4(tbp, tbw, x, y, &cb);
        if(cb & 1) {
            if(odd) {
                *dst = ((*dst) & 0xF) | ((*src) & 0xF0);
            } else {
                *dst = ((*dst) & 0xF) | (((*src) << 4) & 0xF0);
            }
        } else {
            if(odd) {
                *dst = ((*dst) & 0xF0) | (((*src) >> 4) & 0xF);
            } else {
                *dst = ((*dst) & 0xF0) | ((*src) & 0xF);
            }
        }

        if(odd) src++;
        odd = !odd;
    }
}

void ReadTexture4(gsargs) {
    gshdr(u8);
    bool odd = false;
    gsloop {
        int cb;
        u8 *dst = gsmem + CalcGSIndex4(tbp, tbw, x, y, &cb);
        if(cb & 1) {
            if(odd) {
                *src = ((*src) & 0x0F) | ((*dst) & 0xF0);
            } else {
                *src = ((*src) & 0xF0) | (((*dst) >> 4) & 0x0F);
            }
        } else {
            if(odd) {
                *src = ((*src) & 0x0F) | (((*dst) << 4) & 0xF0);
            } else {
                *src = ((*src) & 0xF0) | ((*dst) & 0x0F);
            }
        }

        if(odd) src++;
        odd = !odd;
    }
}

}
