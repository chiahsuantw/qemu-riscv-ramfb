#include "ramfb.h"

#define FB_BASE   0xfe000000
#define FB_WIDTH  1024
#define FB_HEIGHT 768
#define FB_BPP    4

enum { VIDEO_ALIGN_TOP_LEFT, VIDEO_ALIGN_CENTER };

void video_bmp_display(struct RAMFBCfg ramfb_cfg, unsigned int *bmp_image,
                       int width, int height, int align);

void main()
{
    /* Initialize the framebuffer */
    struct RAMFBCfg cfg = {
        .addr = bswap64(FB_BASE),
        .fourcc = bswap32(FORMAT_XRGB8888),
        .flags = bswap32(0),
        .width = bswap32(FB_WIDTH),
        .height = bswap32(FB_HEIGHT),
        .stride = bswap32(FB_WIDTH * FB_BPP),
    };
    ramfb_setup(&cfg);

    /* Generate a blue 100x50 image */
    int bmp_width = 100;
    int bmp_height = 50;
    unsigned int bmp_image[bmp_width * bmp_height];
    for (int i = 0; i < bmp_width * bmp_height; i++)
        bmp_image[i] = 0xFF0000FF;

    /* Display the image at the center */
    video_bmp_display(cfg, bmp_image, bmp_width, bmp_height,
                      VIDEO_ALIGN_CENTER);
}

void video_bmp_display(struct RAMFBCfg ramfb_cfg, unsigned int *bmp_image,
                       int width, int height, int align)
{
    unsigned int *fb = (unsigned int *)bswap64(ramfb_cfg.addr);
    int fb_width = bswap32(ramfb_cfg.width);
    int fb_height = bswap32(ramfb_cfg.height);

    int start_x = 0;
    int start_y = 0;

    if (align == VIDEO_ALIGN_CENTER) {
        start_x = (fb_width - width) / 2;
        start_y = (fb_height - height) / 2;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int fb_x = start_x + x;
            int fb_y = start_y + y;
            if (fb_x >= 0 && fb_x < fb_width && fb_y >= 0 && fb_y < fb_height) {
                fb[fb_y * fb_width + fb_x] = bmp_image[y * width + x];
            }
        }
    }
}
