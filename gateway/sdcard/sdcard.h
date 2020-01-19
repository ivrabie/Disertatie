
#ifndef SDCARD_SDCARD_H_
#define SDCARD_SDCARD_H_
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
namespace SDCARD
{
#define SDCARD_TAG "SD_CARD"
    class SdCard
    {
        public:

        gpio_num_t cs;
        gpio_num_t clk;
        gpio_num_t mosi;
        gpio_num_t miso;
        sdmmc_host_t host;
        sdspi_slot_config_t  slot_config;
        esp_vfs_fat_sdmmc_mount_config_t mount_config;
        sdmmc_card_t* card;
        FILE *f;
        SdCard(gpio_num_t cs, gpio_num_t clk, gpio_num_t mosi, gpio_num_t miso);
        ~SdCard();


        void Init(void);
        size_t WriteBinaryFile(char* fileName, uint8_t *data, uint16_t len);
        size_t ReadBinaryFile(char* fileName, uint8_t *data, uint32_t offset,uint32_t len);
        void DeleteFile(char* fileName);
        size_t GetFileLen(char *fileName);


    };
}

#endif