#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H
#include <stdint.h>
#include <stdbool.h>

#define CONFIG_FLASH_ADDR  0x080E0000UL
#define CONFIG_FLASH_SIZE  (128 * 1024UL)
#define CONFIG_MAX_RECORDS (CONFIG_FLASH_SIZE / sizeof(Configuration))
#define CONFIG_MAGIC       0x434F4E46UL

typedef struct
{
    uint32_t magic;

    uint8_t brightness;
    uint8_t volume;
    bool scanlines;
    uint8_t reserved;

} Configuration;
extern Configuration activeConfig;


void load_config();
void save_config();


#endif