#include "tim2.h"

namespace tim2 {

bool check_install() {
    if(sizeof(header_t) != 0x10) {
        return false;
    }
    if(sizeof(segment_t) != 0x30) {
        return false;
    }
    return true;
}

bool checkheader(const byte magic[4]) {
    if(*(const u32*)(magic) != 0x324D4954) {
        return false;
    }
    return true;
}

header_t *getheader(const void *tim2) {
    return (header_t*)(tim2);
}

byte *getpixels(segment_t *segment) {
    return (byte*)(segment) + segment->offset_imagedata;
}

int getsegmentbyindex(const void *tim2, int index, segment_t **out_segment) {
    byte *bytes = (byte*)(tim2);
    header_t *hdr = (header_t*)(bytes);

    if(false == checkheader(hdr->magic)) {
        return 1;
    }

    if(index >= hdr->textures_count) {
        return 1;
    }

    segment_t *segment = (segment_t*)(hdr+1);
    byte *caddr = (byte*)(segment);
    for(int i = 0; i < index; i += 1) {
        caddr += segment->totalsize;
        segment = (segment_t*)(caddr);
    }

    *out_segment = segment;
    return 0;
}


}
