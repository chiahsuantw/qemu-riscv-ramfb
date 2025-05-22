/* QEMU ramfb driver implementation */

#pragma once

#include <stdint.h>

#define QEMU_PACKED __attribute__((packed))

#define bswap64(x) __builtin_bswap64(x)
#define bswap32(x) __builtin_bswap32(x)
#define bswap16(x) __builtin_bswap16(x)

#define FORMAT_RGB888   875710290
#define FORMAT_XRGB8888 875713112
#define FORMAT_RGB565   909199186

struct QEMU_PACKED RAMFBCfg {
    uint64_t addr;
    uint32_t fourcc;
    uint32_t flags;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
};

int ramfb_setup(struct RAMFBCfg *cfg);
