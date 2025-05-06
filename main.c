#include "ramfb.h"

#define FB_BASE   0xfe000000
#define FB_WIDTH  1024
#define FB_HEIGHT 768
#define FB_BPP    4

void main()
{
    struct RAMFBCfg cfg = {
        .addr = bswap64(FB_BASE),
        .fourcc = bswap32(FORMAT_XRGB8888),
        .flags = bswap32(0),
        .width = bswap32(FB_WIDTH),
        .height = bswap32(FB_HEIGHT),
        .stride = bswap32(FB_WIDTH * FB_BPP),
    };
    video_setup_ramfb(&cfg);
    unsigned int *fb = (unsigned int *)FB_BASE;
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++)
        fb[i] = 0xFFFF0000;
}
