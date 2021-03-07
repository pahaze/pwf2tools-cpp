#include "gs.h"
#include "tim2.h"
#include <stdio.h>

bool tim2download(const void *tim2) {
  tim2::header_t *hdr = tim2::getheader(tim2);
  if(!tim2::checkheader(hdr->magic)) {
    return false;
  }

  int i;
  for( i = 0; i < hdr->textures_count; i += 1) {
    tim2::segment_t *segment;
    bool s = tim2::getsegmentbyindex(tim2, i, &segment);
    if(s) continue;

    gs::tex0_t tex0(segment->tex0);
    byte *pixels = tim2::getpixels(segment);

    gs::ReadTexture(
      tex0.psm,
      tex0.tb_addr, tex0.tb_width,
      0,0,
      segment->width, segment->height,
      pixels
    );
  }

  return true;
}

bool tim2upload(const void *tim2) {
    tim2::header_t *hdr = tim2::getheader(tim2);
    if(!tim2::checkheader(hdr->magic)) {
        return false;
    }

    int i;
    for( i = 0 ; i < hdr->textures_count; i += 1 ) {
        tim2::segment_t *segment;
        bool s = tim2::getsegmentbyindex(tim2, i, &segment);
        if(s) continue;

        gs::tex0_t tex0(segment->tex0);
        byte *pixels = tim2::getpixels(segment);

        gs::WriteTexture(
            tex0.psm,
            tex0.tb_addr, tex0.tb_width,
            0, 0,
            segment->width, segment->height,
            pixels
        );
    }

    return true;
}
