
u32 color32from16(u16 c) {
	u32 a16 = (c >> 15) & 1;
	u32 r16 = (c >> 10) & 0x1F;
	u32 g16 = (c >> 5) & 0x1F;
	u32 b16 = c & 0x1F;

	u32 a32 = a16 ? 0xFF : 0x0;
	u32 r32 = u32(r16) * 8;
	u32 g32 = u32(g16) * 8;
	u32 b32 = u32(b16) * 8;

	return (
		(a32 << 24) |
		(r32 << 16) |
		(g32 << 8) |
		(b32)
	);
}

u16 color16from32(u32 c) {
  u32 a32 = (c >> 24) & 0xFF;
  u32 r32 = (c >> 16) & 0xFF;
  u32 g32 = (c >> 8) & 0xFF;
  u32 b32 = c & 0xFF;

  u32 a16 = (a32 ? 1 : 0);
  u32 r16 = r32 / 8;
  u32 g16 = g32 / 8;
  u32 b16 = b32 / 8;

  return (
    (a16 << 15) |
    (r16 << 10) |
    (g16 << 5) |
    (b16 & 0x1F)
  );
}

int calc_pixel_stride(int gstype) {
  switch(gstype) {
  case GS_TEX_4:
  case GS_TEX_8:
    return 1;
  case GS_TEX_16:
    return 2;
  case GS_TEX_24:
  case GS_TEX_32:
    return 4;
  default:
    fprintf(stderr, "calc_pixel_stride: Unknown pixel type: %d\n", gstype);
    return -1;
  }
}

int calc_clut_stride(int cpsm) {
  switch(cpsm) {
  case GS_CLUT_16:
    return 2;
  case GS_TEX_24:
  case GS_CLUT_32:
    return 4;
  default:
    fprintf(stderr, "calc_clut_stride: Unknown CLUT type: %d\n", cpsm);
    return -1;
  }
}

bool ispalettetype(int psm) {
  switch(psm) {
  case GS_TEX_4:
  case GS_TEX_8:
    return true;
  default:
    return false;
  }
}

int calc_ncolors(int psm) {
  switch(psm) {
  case GS_TEX_4:
    return 16;
  case GS_TEX_8:
    return 256;
  default:
    fprintf(stderr, "calc_ncolors: Unknown Palette type: %d\n", psm);
    return -1;
  }
}

int calc_clut_size(int psm, int cpsm) {
  return calc_clut_stride(cpsm) * calc_ncolors(psm);
}

#define CALCIND ((y*width)+x)

byte getpixel4(const byte *src, int width, int x, int y) {
  int ind = CALCIND;
  byte pixels = src[ind>>1];
  byte pixel = (ind & 1) ? ((pixels & 0xF0) >> 4) : (pixels & 0xF);
  return pixel;
}

template <typename T>
inline T getpixel(const T *src, int width, int x, int y) {
  return src[CALCIND];
}

u32 getpixel_as32(int srctype, const void *src, int width, int x, int y) {
  switch(srctype) {
  case GS_TEX_4:
    return u32(getpixel4((const byte*)(src), width, x, y));
  case GS_TEX_8:
    return u32(getpixel<byte>((const byte*)(src), width, x, y));
  case GS_TEX_16:
    return color32from16(u32(getpixel<u16>((const u16*)(src),width,x,y)));
  case GS_TEX_24:
    return u32(getpixel<u32>((const u32*)(src),width,x,y)) | 0xFF000000; //Enable Alpha
  case GS_TEX_32:
    return u32(getpixel<u32>((const u32*)(src),width,x,y));
  default:
    fprintf(stderr, "getpixel_as32: Unknown pixel type: %d\n", srctype);
    return 0xFFFFFFFF;
  }
}

u32 getclut_as32(int cluttype, const void *clut, int index) {
  const u16 *clut16 = (const u16*)clut;
  const u32 *clut32 = (const u32*)clut;
  switch(cluttype) {
  case GS_CLUT_16:
    return color32from16(clut16[index]);
  case GS_TEX_24:
    return clut32[index] | 0xFF000000;
  case GS_CLUT_32:
    return clut32[index];
  default:
    fprintf(stderr, "getclut_as32: Unknown clut type: %d\n", cluttype);
    return 0xFFFFFFFF;
  }
}

u32 convert_pixel_in_image_to_rgba(int srctype, const void *src, int width, int x, int y, int cluttype, const void *clut) {
  u32 pixel = getpixel_as32(srctype, src, width, x, y);
  if(clut == NULL) {
    return pixel;
  }
  u32 color = getclut_as32(cluttype, clut, pixel);
  return color;
}

void convert_pixels_to_rgba(int srctype, const void *src, int width, int height, int cluttype, const void *clut, u32 *outpixels) {
  /*
  int i;
  for(i = 0; i < 16; i += 1 ){
    printf("clut[i] = %x\n", u32(((u16*)(clut))[i]));
  }
  */
  for(int y = 0; y < height; y += 1)
  for(int x = 0; x < width; x += 1) {
    int index = (y * width) + x;
    u32 color = convert_pixel_in_image_to_rgba(srctype, src, width, x, y, cluttype, clut);
    outpixels[index] = color;
  }
}

template <typename T>
bool clut_has_color(const T *clut, T x, int ncolors) {
  for(int i = 0; i < ncolors; i += 1) {
    if(clut[i] == x) {
      return true;
    }
  }
  return false;
}

template<typename T>
bool insert_clut(T *clut, T x, int &ncolors, int maxcolors) {
  if(false == clut_has_color(clut, x, ncolors)) {
    //printf("Putting color %x\n", x);
    if(ncolors >= maxcolors) {
      return false;
    }
    clut[ncolors] = x;
    ncolors += 1;
    return true;
  } else {
    return true;
  }
}

#define build_image_clut_insert(T, clut, color) \
  if(false == insert_clut<T>(clut, color, ncolors, maxcolors)) { \
    fprintf(stderr, "CLUT needs too many colors\n"); \
    return -1; \
  }

void zero_clut(void *clut, int stride, int ncolors) {
  memset(clut, 0, ncolors * stride);
}

int build_image_clut(const u32 *in_rgba, int width, int height, int cluttype, void *out_clut, int maxcolors) {
  u16 *clut16 = (u16*)(out_clut);
  u32 *clut32 = (u32*)(out_clut);
  int ncolors = 0;
  for(int y = 0; y < height; y+=1)
  for(int x = 0; x < width; x+=1) {
    int index = (y * width) + x;
    switch(cluttype) {
    case GS_CLUT_16:
      build_image_clut_insert(u16, clut16, color16from32(in_rgba[index]));
      break;
    case GS_TEX_24:
      build_image_clut_insert(u32, clut32, in_rgba[index] | 0xFF000000);
      break;
    case GS_CLUT_32:
      build_image_clut_insert(u32, clut32, in_rgba[index]);
      break;
    default:
      fprintf(stderr, "build_image_clut: Unsupported CLUT Type %d\n", cluttype);
      break;
    }
  }
  return ncolors;
}

u32 convert_pixel_in_image_to_tex0(int srctype, const u32 *inpixels, int width, int x, int y, const u32 *inclut, int clutsize) {
  int index = (y * width) + x;
  u32 c = inpixels[index];

  if(ispalettetype(srctype)) {
    for(int i = 0; i < clutsize; i += 1) {
      if(inclut[i] == c) return i;
    }
    fprintf(stderr, "Couldn't find CLUT entry\n");
    return 0;
  } else {
    switch(srctype) {
    case GS_TEX_16:
      return color16from32(c);
    case GS_TEX_24:
    case GS_TEX_32:
      return c;
    default:
      fprintf(stderr, "convert_pixel_in_image_to_tex0: Unknown TEX type %d\n", c);
      return 0;
    }
  }
}

void wh_from_tex0(gs::tex0_t tex0, int &width, int &height) {
  width = 1 << tex0.tex_width;
  height = 1 << tex0.tex_height;
}
byte sethighnibble(byte b, byte v) {
  return (b & 0xF) | ((v & 0xF) << 4);
}

byte setlownibble(byte b, byte v) {
  return (b & 0xF0) | (v & 0xF);
}

void write_typed_pixel(int srctype, void *outpixels, int width, int x, int y, u32 pixel) {
  int index = (y * width) + x;
  int stride = calc_pixel_stride(srctype);
  byte *outpixels_asbyte = (byte*)(outpixels);
  byte *l = outpixels_asbyte + (index * stride);
  switch(srctype) {
  case GS_TEX_4:
    l = outpixels_asbyte + (index >> 1);
    if(index & 1) {
      *l = sethighnibble(*l, byte(pixel));
    } else {
      *l = setlownibble(*l, byte(pixel));
    }
    break;
  case GS_TEX_8:
    *l = byte(pixel);
    break;
  case GS_TEX_16:
    *(u16*)(l) = u16(pixel);
    break;
  case GS_TEX_24:
  case GS_TEX_32:
    *(u32*)(l) = u32(pixel);
    break;
  default:
    fprintf(stderr, "write_typed_pixel: Unknown Pixel Type %d\n", srctype);
    break;
  }
}

void convert_pixels_to_tex0(int srctype, const u32 *inpixels, int width, int height, int cluttype, void *out_clut, void *out_pixels) {
  u32 clut32[256] = {0};
  int clutsize = 0;
  bool haspalette = ispalettetype(srctype);
  if(haspalette) {
    clutsize = calc_ncolors(srctype);
    build_image_clut(inpixels, width, height, GS_CLUT_32, (void*)(clut32), clutsize);
  }

  for(int y = 0; y < height; y += 1)
  for(int x = 0; x < width; x += 1) {
    u32 pixel = convert_pixel_in_image_to_tex0(srctype, inpixels, width, x, y, clut32, clutsize);
    write_typed_pixel(srctype, out_pixels, width, x, y, pixel);
  }

  if(haspalette) {
    build_image_clut(inpixels, width, height, cluttype, out_clut, clutsize);
  }
}
