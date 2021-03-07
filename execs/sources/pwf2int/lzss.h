#ifndef LZSS_H
#define LZSS_H

int lzss_compress(
  int EI, int EJ, int P, int rless, unsigned char *g_ring_buffer,
  unsigned char *g_infile, int srclen, unsigned char *g_outfile
);

void lzss_decompress(
  int EI, int EJ, int P, int rless, unsigned char *buffer,
  unsigned char *src, int srclen, unsigned char *dst, int dstlen
);

#endif

