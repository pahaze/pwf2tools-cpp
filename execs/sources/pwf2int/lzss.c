/* Code is partially derived from Haruhiko Okumura (4/6/1989), in which they express the freedom to use, modify, and distribute their "LZSS.C" source code. */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t byte;

struct lzss_t {
  int N;
  int F;
  int THRESHOLD;
  unsigned char *textbuf;

  int matchpos;
  int matchlen;

  int *lson;
  int *rson;
  int *dad;
};

#define N (state->N)
#define F (state->F)
#define THRESHOLD (state->THRESHOLD)
#define NIL N
#define textbuf (state->textbuf)
#define matchpos (state->matchpos)
#define matchlen (state->matchlen)
#define lson (state->lson)
#define rson (state->rson)
#define dad (state->dad)

static inline void init_tree(struct lzss_t *state) {
  int i;
  for(i = (N+1); i <= (N+256); i += 1) {
    rson[i] = NIL;
  }
  for(i = 0; i < N; i+=1) {
    dad[i] = NIL;
  }
}

static inline void insert_node(struct lzss_t *state, int r) {
  int i,p,cmp;
  unsigned char *key;

  cmp = 1; key = (textbuf + r); p = N+1+key[0];
  rson[r] = lson[r] = NIL; matchlen = 0;
  for(;;) {
    if(cmp >= 0) {
      if(rson[p] != NIL) {
	p = rson[p];
      } else {
	rson[p] = r; dad[r] = p; 
	return;
      }
    } else {
      if(lson[p] != NIL) {
	p = lson[p];
      } else {
	lson[p] = r; dad[r] = p; 
	return;
      }
    }
    for(i = 1; i < F; i += 1) {
      if((cmp = (key[i] - textbuf[p+i])) != 0) {
	break;
      }
    }
    if(i > matchlen) {
      matchpos = p;
      if((matchlen = i) >= F) {
	break;
      }
    }
  }
  dad[r] = dad[p]; lson[r] = lson[p]; rson[r] = rson[p];
  dad[lson[p]] = r; dad[rson[p]] = r;
  if(rson[dad[p]] == p) {
    rson[dad[p]] = r;
  } else {
    lson[dad[p]] = r;
  }
  dad[p] = NIL;
}

static inline void delete_node(struct lzss_t *state, int p) {
  int q;

  if(dad[p] == NIL) { return; }
  if(rson[p] == NIL) {
    q = lson[p];
  } else if(NIL == lson[p]) {
    q = rson[p];
  } else {
    q = lson[p];
    if(rson[q] != NIL) {
      do {
	q = rson[q];
      } while(rson[q] != NIL);
      rson[dad[q]] = lson[q]; dad[lson[q]] = dad[q];
      lson[q] = lson[p]; dad[lson[p]] = q;
    }
    rson[q] = rson[p]; dad[rson[p]] = q;
  }
  dad[q] = dad[p];
  if(p == rson[dad[p]]) {
    rson[dad[p]] = q;
  } else {
    lson[dad[p]] = q;
  }
  dad[p] = NIL;
}

#define readsrc() if(src >= srcend) { break; } c = *src++;
#define writedst(x) if(NULL != dst) { *dst++ = (x); }

static int encode(struct lzss_t *state, int EJ, const uint8_t *src, int srclen, uint8_t *dst) {
  int i,c,len,r,s,lastmatchlen,codebufind;
  uint8_t codebuf[33];
  uint8_t mask;
  const uint8_t *srcend = src + srclen;
  int dstlen = 0;
  int NMASK = N - 1;

  init_tree(state);
  codebuf[0]=0;
  codebufind=mask=1;

  s = 0; r = N - F;

  for(len = 0; len < F; len+=1){
    readsrc();
    textbuf[r+len] = c;
  }
  if(0 == len) return 0;

  for(i = 1; i <= F; i += 1) {
    insert_node(state, r - i);
  }

  insert_node(state, r);

  do {
    if(matchlen > len) {
      matchlen = len;
    }
    if(matchlen <= THRESHOLD) {
      matchlen = 1;
      codebuf[0] |= mask;
      codebuf[codebufind++] = textbuf[r];
    } else {
      codebuf[codebufind++] = (uint8_t)(
	((matchpos >> (8 - EJ)) & ~((1 << EJ)-1)) |
        (matchlen - (THRESHOLD + 1))
      );
    }
    if((mask <<= 1) == 0) {
      for(i = 0; i < codebufind; i += 1) {
        writedst(codebuf[i]);
      }
      dstlen += codebufind;
      codebuf[0] = 0; codebufind = mask = 1;
    }
    lastmatchlen = matchlen;
    for(i = 0; i < lastmatchlen; i += 1) {
      readsrc();
      delete_node(state, s);
      textbuf[s] = c;
      if(s < F - 1) {
        textbuf[s+N] = c;
      }
      s = (s + 1) & NMASK;
      r = (r + 1) & NMASK;
      insert_node(state, r);
    }
    while(i++ < lastmatchlen) {
      delete_node(state, s);
      s = (s + 1) & NMASK;
      r = (r + 1) & NMASK;
      if(--len) { insert_node(state, r); }
    }
  } while (len > 0);
  if(codebufind > 1) {
    for(i = 0; i < codebufind; i += 1) {
      writedst(codebuf[i]);
    }
    dstlen += codebufind;
  }
  return dstlen;
}

int lzss_compress(int EI, int EJ, int P, int rless, uint8_t *buffer, const uint8_t *src, int srclen, uint8_t *dst) {
  struct lzss_t lstate;
  struct lzss_t *state = &lstate;
  int dstlen;
  N = 1 << EI;
  F = (1 << EJ) + P;
  THRESHOLD = P;
  textbuf = buffer;
  lson = malloc(sizeof(*lson) * (N+1));
  rson = malloc(sizeof(*rson) * (N+257));
  dad = malloc(sizeof(*dad) * (N+1));

  dstlen = encode(state, EJ, src, srclen, dst);

  free(lson); free(rson); free(dad);
  return dstlen;
}
#undef N
#undef F
#undef readsrc
#undef writedst

#define readsrc(x) if(src >= srcend) { break; } (x) = *src++;
#define writedst(x) if(dst >= dstend) { break; } *dst++ = (x);

void lzss_decompress(int EI, int EJ, int P, int rless, byte *buffer, const byte *srcstart, int srclen, byte *dststart, int dstlen) {
  int N = (1 << EI);
  int F = (1 << EJ);

  const byte *src = srcstart;
  const byte *srcend = srcstart + srclen;

  byte *dst = dststart;
  byte *dstend = dststart + dstlen;

  int r = (N - F) - rless;
  
  int flags;
  int c, i, j, k;
  const int NMASK = (N - 1);
  const int FMASK = (F - 1);
  for(flags = 0;; flags >>= 1) {
    if(!(flags & 0x100)) {
      readsrc(flags);
      flags |= 0xFF00;
    }
    if(flags & 1) {
      readsrc(c);
      writedst(c);
      buffer[r++] = c;
      r &= NMASK;
    } else {
      readsrc(i);
      readsrc(j);
      i |= ((j >> EJ) << 8);
      j = (j & FMASK) + P;
      for(k = 0; k <= j; k += 1) {
	c = buffer[(i+k) & NMASK];
	writedst(c);
	buffer[r++] = c;
	r &= NMASK;
      }
    }
  }
}


