#include "gs.h"
#include "tim2.h"
#include "tim2upload.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <dirent.h>
#include "pnghelper.h"
#include "gsutil.inc"
#include <vector>
#include <string>
#include "pwf2common.h"

//For ptr2cmd: 
#define MAX_ALIASES 4
#include "pwf2cmd.h"

#ifdef _WIN32
  #define PRIx64 "llx"
#endif

#define INFO(s,...) printf(s, __VA_ARGS__);
#define ERROR(s,...) fprintf(stderr, s, __VA_ARGS__);
#define MAX_PATH 256

static int cmd_extract(int argc, char *args[]);
static int cmd_extract_list(int argc, char *args[]);
static int cmd_inject(int argc, char *args[]);
static int cmd_inject_list(int argc, char *args[]);

static cmd_t commands[2] = {
  { "extract", "[tm0-folder] [in-listfile] <png-folder>",
    "Batch extract textures from tm0-folder using listfile and output as RGBA PNG.\n\
     If png-folder is not specified, the current directory is used for output.",
    {"e", "x", "get", "g"}, 4, cmd_extract_list
  },

  { "inject", "[tm0-folder] [in-listfile] <png-folder>",
    "Batch inject textures into tm0-folder using listfile with 32-bit RGBA PNGs.\n\
     If png-folder is not specified, the current directory is used as input.",
    {"i", "put", "p"}, 3, cmd_inject_list
  }
};

struct tm0info_t {
  std::string name;
  gs::tex0_t tex0;
  void setfilename(const char *newname);
};

void tm0info_t::setfilename(const char *newname) {
  this->name = std::string(newname);
}

struct tm0env_t {
  const char *path;
  std::vector<tm0info_t> tm0s;
  bool load(const char *path);
  bool save(const char *path);
};

const char *extof(const char *filename) {
  return (strrchr(filename, '.'));
}

bool tm0env_t::save(const char *path) {
  void *tm0;
  char lbuf[MAX_PATH];

  for(tm0info_t &tm0info : this->tm0s) {
    const char *tm0filename = tm0info.name.c_str();
    snprintf(lbuf, sizeof(lbuf), "%s/%s", path, tm0filename);
    printf("TIM2_SAVE: %s\n", lbuf);
    FILE *f = fopen(lbuf, "rb");
    if(NULL == f) {
      ERROR("   Could not open %s for reading\n", lbuf);
      continue;
    }
    int len = getfilesize(f);
    tm0 = malloc(len);
    fread(tm0, 1, len, f);
    fclose(f);

    if(false == tim2download(tm0)) {
      ERROR("   Failed to download TIM2 %s\n", lbuf);
      free(tm0);
      continue;
    }

    f = fopen(lbuf, "wb");
    if (NULL == f) {
      ERROR("   Couldn't open %s for writing\n", lbuf);
      free(tm0);
      continue;
    }
    fwrite(tm0, 1, len, f);
    fclose(f);
    free(tm0);
  }

  return true;
}

bool tm0env_t::load(const char *path) {
  void *tm0;
  struct dirent *de;
  DIR *dir = opendir(path);
  char lbuf[MAX_PATH];

  if(NULL == dir) {
    fprintf(stderr, "Failed to open tm0 folder %s\n", path);
    return false;
  }

  while((de = readdir(dir)) != NULL) {
    const char *filename = de->d_name;
    snprintf(lbuf, sizeof(lbuf), "%s/%s", path, filename);
    /*
    struct stat st;
    if(stat(lbuf, &st) == -1) {
      fprintf(stderr, "Couldn't stat file: %s\n", filename);
      continue;
    }*/

    if(streq(extof(filename), ".tm0")) {
      tm0info_t tm0info;
      FILE *tm0file = fopen(lbuf, "rb");
      if(NULL == tm0file) {
	ERROR("Could not open TIM2 %s\n", lbuf);
	continue;
      }
      printf("TIM2_LOAD: %s\n", lbuf);
      int len = getfilesize(tm0file);
      tm0 = malloc(len);
      fread(tm0, 1, len, tm0file);
      if(false == tim2upload(tm0)) {
	ERROR("Failed to upload TIM2 %s\n", lbuf);
      }
      free(tm0);
      fclose(tm0file);
      tm0info.setfilename(filename);
      this->tm0s.push_back(tm0info);
    }
  }
  return true;
}


void extract_from_tex0(gs::tex0_t tex0, u32 *outpixels) {
  int tw,th; wh_from_tex0(tex0, tw, th);
  int npixels = tw * th;
  int pixstride = calc_pixel_stride(tex0.psm);
  void *pixels = (void*)(malloc(npixels * pixstride));
  int clutsize;
  void *clut = NULL;

  gs::ReadTexture(tex0.psm, tex0.tb_addr, tex0.tb_width, 0, 0, tw, th, pixels);

  if(ispalettetype(tex0.psm)) {
    clutsize = calc_clut_size(tex0.psm, tex0.clut_pixmode);
    clut = (void*)(malloc(clutsize));
    gs::ReadCLUT(tex0.psm, tex0.clut_pixmode, tex0.cb_addr, 1, 0, 0, calc_ncolors(tex0.psm), 1, clut);
  }

  convert_pixels_to_rgba(tex0.psm, pixels, tw, th, tex0.clut_pixmode, clut, outpixels);

  free(clut);
  free(pixels);
  return;
}


void inject_tex(gs::tex0_t tex0, u32 *inpixels) {
  int tw,th; wh_from_tex0(tex0, tw, th);
  int npixels = tw * th;
  u32 clut32[256];
  void *clut = (void*)(clut32);
  void *pixels = malloc(npixels * calc_pixel_stride(tex0.psm));

  convert_pixels_to_tex0(tex0.psm, inpixels, tw, th, tex0.clut_pixmode, clut, pixels);

  if(ispalettetype(tex0.psm)) {
    gs::WriteCLUT(tex0.psm, tex0.clut_pixmode, tex0.cb_addr, 1, 0, 0, calc_ncolors(tex0.psm), 1, clut);
  }

  gs::WriteTexture(tex0.psm, tex0.tb_addr, tex0.tb_width, 0, 0, tw, th, pixels);

  free(pixels);

  return;
}

void ptr2tex_alloc_out_image_from_tex0(gs::tex0_t tex0, u32 **outpixels) {
  int tw,th; wh_from_tex0(tex0, tw, th);
  int npixels = tw * th;
  *outpixels = (u32*)(malloc(npixels * sizeof(**outpixels)));
}

#define REQUIRE(x, c) if(argc < x) { commands[c].printhelp(""); return 1; }


static int cmd_extract_list(int argc, char *args[]) {
  REQUIRE(2, 2);
  const char *tm0path = args[0];
  const char *listfilename = args[1];
  const char *pngfolder = (argc > 2) ? args[2] : NULL;
  gs::tex0_t tex0;
  tm0env_t tm0env;
  if(false == tm0env.load(tm0path)) {
    return 1;
  }
  char lbuf[64];
  FILE *listfile = fopen(listfilename, "r");
  if(NULL == listfile) {
    ERROR("Could not open list file %s\n", listfilename);
    return 1;
  }
  int ntex = 0;
  while(!feof(listfile)) {
    int err = fscanf(listfile, "%016" PRIx64 "\n", &tex0.value);
    if(err < 1) continue; //Did not return tex0 value
    int tw,th; wh_from_tex0(tex0, tw, th);
    int npixels = tw * th;
    u32 *outpixels = (u32*)(malloc(npixels * sizeof(*outpixels)));

    if(pngfolder != NULL) {
      snprintf(lbuf, sizeof(lbuf), "%s/%d.png", pngfolder, ntex);
    } else {
      snprintf(lbuf, sizeof(lbuf), "%d.png", ntex);
    }

    printf("EXTRACT: %016" PRIx64 " -> %s\n", tex0.value, lbuf);
    extract_from_tex0(tex0, outpixels);
	
    ntex += 1;
    FILE *outfile = fopen(lbuf, "wb");
    if(outfile == NULL) {
      ERROR("Could not open PNG file %s\n", lbuf);
      free(outpixels);
      continue;
    }
    pngwrite(outfile, tw, th, outpixels);
    fclose(outfile);

    free(outpixels);
  }
  printf(" %d textures\n", ntex);
  return 0;
}


static int cmd_inject_list(int argc, char *args[]) {
  REQUIRE(2, 3);
  const char *tm0foldername = args[0];
  const char *listfilename = args[1];
  const char *pngfolder = (argc > 2) ? args[2] : NULL;
  gs::tex0_t tex0;
  tm0env_t tm0env;
  if(false == tm0env.load(tm0foldername)) {
    return 1;
  }
  char lbuf[64];
  FILE *listfile = fopen(listfilename, "r");
  if(NULL == listfile) {
    ERROR("Could not open list file %s\n", listfilename);
    return 1;
  }
  int ntex = 0;
  while(!feof(listfile)) {
    int err = fscanf(listfile, "%016" PRIx64 "\n", &tex0.value);
    if(err < 1) continue;
    int tw,th; wh_from_tex0(tex0,tw,th);
    int npixels = tw * th;
    u32 *inpixels = (u32*)(malloc(npixels * sizeof(*inpixels)));

    if(pngfolder == NULL) {
      snprintf(lbuf, sizeof(lbuf), "%d.png", ntex);
    } else {
      snprintf(lbuf, sizeof(lbuf), "%s/%d.png", pngfolder, ntex);
    }
    FILE *infile = fopen(lbuf, "rb");
    if(NULL == infile) {
      fprintf(stderr, "Couldn't open file %s\n", lbuf);
      free(inpixels);
      ntex++;
      continue;
    }
    pngread(infile, inpixels);
    fclose(infile);

    printf("INJECT: %016" PRIx64 " [%s]\n", tex0.value, lbuf);

    inject_tex(tex0, inpixels);

    free(inpixels);
    ntex++;
  }
  tm0env.save(tm0foldername);
  printf("Uploaded %d textures\n", ntex);
  return 0;
}

int main(int argc, char *args[]) {
  if(!tim2::check_install()) {
    printf("Bad compile\n");
    return 1;
  }

  if(argc > 1) {
    for(cmd_t &cmd : commands) {
      if(cmd.matches(args[1])) {
	return cmd.exec(argc-2, args+2);
      }
    }
    printf("pwf2tex:  Unknown command: %s\n", args[1]);
    return 1;
  }
  for(cmd_t &cmd : commands) {
    printf("---\n");
    cmd.printhelp("");
  }
  return 1;
}
