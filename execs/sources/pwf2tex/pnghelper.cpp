#include <png.h>

#include "types.h"

void pngread(FILE *f, u32 *colors) {
  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);

  png_init_io(png, f);

  png_read_info(png, info);

  int width, height, colortype, bpc;

  width = png_get_image_width(png, info);
  height = png_get_image_height(png, info);
  colortype = png_get_color_type(png, info);
  bpc = png_get_bit_depth(png, info);

  if (16 == bpc) {
    png_set_strip_16(png);
  }

  if (PNG_COLOR_TYPE_PALETTE == colortype) {
    png_set_palette_to_rgb(png);
  }

  if (PNG_COLOR_TYPE_GRAY == colortype && bpc < 8) {
    png_set_expand_gray_1_2_4_to_8(png);
  }

  if (png_get_valid(png, info, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png);
  }

  switch (colortype) {
    case PNG_COLOR_TYPE_RGB:
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_PALETTE:
      png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
      break;
    default:
      break;
  }

  switch (colortype) {
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      png_set_gray_to_rgb(png);
      break;
    default:
      break;
  }

  png_read_update_info(png, info);

  for (int y = 0; y < height; y += 1) {
    png_read_row(png, (png_bytep)(colors + (y * width)), NULL);
  }

  png_read_end(png, info);
}

void pngwrite(FILE *f, int width, int height, const void *colors) {
  const byte *data = (const byte *)(colors);
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);

  png_init_io(png, f);

  png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);

  for (int y = 0; y < height; y += 1) {
    png_write_row(png, data + (y * width * sizeof(u32)));
  }
  png_write_end(png, NULL);
}
