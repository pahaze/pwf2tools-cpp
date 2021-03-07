#include "spm.h"

namespace spm {

#define POLYGON_KEY u64(0xEEEEEEEEEEEEEEEE)
#define SPM_MAGIC u32(0x18DF540A)

bool check_install() {
    if(sizeof(polygonheader_t) != 0x70) { return false; }
    return true;
}

bool checkheader(const void *spm) {
    u32 magic = *(u32*)(spm);
    if(SPM_MAGIC == magic) {
        return true;
    }
    return false;
}

int getpolygoncount(const void *spm, int len) {
    if(false == checkheader(spm)) return -1;
    const byte *bytes = (const byte*)(spm);
    int parsed = 8;
    int acc = 0;

    while((parsed + 8) <= len) {
        u64 v = *(u64*)(bytes + parsed);
        if(v == POLYGON_KEY) {
            acc += 1;
        }
        parsed += 0x10;
    }

    return acc;
}

bool getpolygonbyindex(const void *spm, int len, int index, polygonheader_t **out_polygon) {
    if(!checkheader(spm)) return false; //if not an spm file, go away
    const byte *bytes = (const byte*)(spm); //get the spm bytes
    int parsed = 8;
    int acc = 0;

    while((parsed + 8) <= len) {
        u64 v = *(u64*)(bytes + parsed); //get bytes + parsed
        if(v == POLYGON_KEY) { //if its 0xEEEEEEEEEEEEEEEE
            if(acc == index) { //if index matches the current iteration
                *out_polygon = (polygonheader_t*)((bytes + parsed) - 0x68); //get header from bytes + parsed and subtract from 0x68
                return true; //we found it amigos
            }
            acc += 1;	
            if(acc > index) { break; }
        }
        parsed += 0x10; //increase parsed by 0x10
    }
    *out_polygon = nullptr; //couldnt find it
    return false; //nope
}

u64 tex0frompolygon(polygonheader_t *polygon) {
    return *(u64*)(polygon + 1);
}



}
