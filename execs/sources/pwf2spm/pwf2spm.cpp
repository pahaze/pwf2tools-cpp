#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <vector>
#include <algorithm>
#include "spm.h"

using namespace std;

#ifdef _WIN32
  #define PRIx64 "llx"
  #define PRIx8 "hhx"
#endif

#define ERROR(s,...) fprintf(stderr, s, __VA_ARGS__)

int getfilesize(FILE *f) {
  int savepos = ftell(f);
  int ret;
  fseek(f, 0, SEEK_END);
  ret = ftell(f);
  fseek(f, savepos, SEEK_SET);
  return ret;
}

void loadfile(FILE *f, void *out) {
  int len = getfilesize(f);
  fread(out, 1, len, f);
  return;
}

void printhelp() {
  printf("gtex0 [spmfile] [outfile]\n");
  printf("   Extracts tex0 from an SPM model.\n");
}

bool streq(const char *s1, const char *s2) {
  if(0 == strcasecmp(s1, s2)) {
    return true;
  }
  return false;
}

vector<u64> tex0listfromspm(const void *spm, int spmlen, bool tex0) {
  int npoly = spm::getpolygoncount(spm, spmlen); //get the plygons from the spm
  int i;
  spm::polygonheader_t *poly; //get array of polygons
  vector<u64> tex0list; //vector of u64s
  vector<u64> formats;

  for(i = 0; i < npoly; i += 1) { //for each poly
    if(spm::getpolygonbyindex(spm, spmlen, i, &poly)) { //if the poly is sucessfully output
      u8 fmt = poly->format >> 24; //get format but bitshift right 24 bits
      if(fmt != 0x20) { //if format bitshifted is not 0x20
		u64 tex0v = spm::tex0frompolygon(poly);	

		if (tex0list.end() == find(tex0list.begin(), tex0list.end(), tex0v)) {
			tex0list.push_back(tex0v);
			formats.push_back(fmt);
			//tex0list.push_back((u64)fmt);
			//printf("TEX0: %016" PRIx64 "\n", tex0v);
			//printf("FORMAT: %02" PRIx8 "\n\n", fmt);

		}
      }
    }
  }
  return tex0 ? tex0list : formats;
}

void gtex0(FILE *spmfile, FILE *outfile, FILE *fmtfile) {
  int spmlen = getfilesize(spmfile);
  void *spm = malloc(spmlen);
  fread(spm, 1, spmlen, spmfile);

  vector<u64> tex0list = tex0listfromspm(spm, spmlen, true);
  vector<u64> formats = tex0listfromspm(spm, spmlen, false);

  for(size_t i = 0; i < tex0list.size(); i += 1) {
    fprintf(outfile, "%016" PRIx64 "\n", tex0list[i]);
  }

  for(size_t i = 0; i < formats.size(); i += 1) {
	fprintf(fmtfile, "%02" PRIx8 "\n", formats[i]);
  }

  printf("Found %d tex0\n", int(tex0list.size()));

  return;
}

#define CMD(a) if(true == streq(#a,args[1]))

int main(int argc, char *args[]) {
  if(argc <= 1) {
    printhelp();
  } else {
    CMD(gtex0) {
      if(argc <= 3) {
		printf("gtex0 <spmfile> <outfile>\n");
		return 1;
      } else {
		FILE *spmfile = fopen(args[2], "rb");
		if(NULL == spmfile) {
		  ERROR("Couldn't open SPM file %s\n", args[2]);
		  return 1;
		}
		FILE *outfile = fopen(args[3], "w");
		if(NULL == outfile) {
		  ERROR("Couldn't open output file %s\n", args[3]);
		  return 1;
		}
		char f[strlen(args[3]) + 3];
		sprintf(f, "%s.fmt", args[3]);
		FILE *fmtfile = fopen(f, "w");
		if (NULL == fmtfile) {
			ERROR("Couldn't open format output file %s\n", f);
			return 1;
		}//*/
		gtex0(spmfile, outfile, fmtfile//*/
		);
		fclose(spmfile);
		fclose(outfile);
		fclose(fmtfile);
      }
    } else {
      ERROR("pwf2spm:  Unknown command %s\n", args[1]);
      return 1;
    }
  }
  return 0;
}
