/* QEMU ramfb driver implementation */

#include "ramfb.h"

/* fw_cfg register locations */
#define FW_CFG_BASE   0x10100000
#define FW_CFG_SELECT (uint16_t *)(FW_CFG_BASE + 0x08)
#define FW_CFG_DATA   (uint64_t *)(FW_CFG_BASE + 0x00)
#define FW_CFG_DMA    (uint64_t *)(FW_CFG_BASE + 0x10)

/* fw_cfg DMA control bits */
#define FW_CFG_DMA_CTL_ERROR  0x01
#define FW_CFG_DMA_CTL_READ   0x02
#define FW_CFG_DMA_CTL_SKIP   0x04
#define FW_CFG_DMA_CTL_SELECT 0x08
#define FW_CFG_DMA_CTL_WRITE  0x10

/* selector key values */
#define FW_CFG_FILE_DIR 0x19

struct QEMU_PACKED FWCfgFile {
    uint32_t size;
    uint16_t select;
    uint16_t reserved;
    char name[56];
};

struct QEMU_PACKED FWCfgFiles {
    uint32_t count;
    struct FWCfgFile f[];
};

struct QEMU_PACKED FWCfgDmaAccess {
    uint32_t control;
    uint32_t length;
    uint64_t address;
};

static int strncmp(const char *s1, const char *s2, int n)
{
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
        return 0;
    return *s1 - *s2;
}

static void fw_cfg_dma_transfer(void *address, uint32_t length,
                                uint32_t control)
{
    struct FWCfgDmaAccess access = {
        .control = bswap32(control),
        .length = bswap32(length),
        .address = bswap64((uint64_t)address),
    };
    *FW_CFG_DMA = bswap64((uint64_t)&access);
    while (bswap32(access.control) & ~FW_CFG_DMA_CTL_ERROR)
        ;
}

static void fw_cfg_read_entry(void *buf, int e, int len)
{
    uint32_t control = (e << 16) | FW_CFG_DMA_CTL_SELECT | FW_CFG_DMA_CTL_READ;
    fw_cfg_dma_transfer(buf, len, control);
}

static void fw_cfg_write_entry(void *buf, int e, int len)
{
    uint32_t control = (e << 16) | FW_CFG_DMA_CTL_SELECT | FW_CFG_DMA_CTL_WRITE;
    fw_cfg_dma_transfer(buf, len, control);
}

static int fw_cfg_find_file(const char *name)
{
    uint32_t count;
    fw_cfg_read_entry(&count, FW_CFG_FILE_DIR, sizeof(count));
    count = bswap32(count);
    for (int i = 0; i < count; i++) {
        struct FWCfgFile file;
        fw_cfg_dma_transfer(&file, sizeof(file), FW_CFG_DMA_CTL_READ);
        if (strncmp(name, file.name, sizeof(file.name)) == 0)
            return bswap16(file.select);
    }
    return -1;
}

int ramfb_setup(struct RAMFBCfg *cfg)
{
    int e = fw_cfg_find_file("etc/ramfb");
    fw_cfg_write_entry(cfg, e, sizeof(struct RAMFBCfg));
    return 0;
}
