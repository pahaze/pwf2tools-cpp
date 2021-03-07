#ifndef PNGHELPER_H
#define PNGHELPER_H

#include <stdio.h>
#include "types.h"

void pngread(FILE *f, u32 *colors);
void pngwrite(FILE *f, int width, int height, const void *colors);

#endif

