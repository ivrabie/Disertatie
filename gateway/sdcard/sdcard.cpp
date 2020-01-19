
#include "sdcard.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "../etl/cstring.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
using namespace SDCARD;
using namespace etl;
SdCard::SdCard(gpio_num_t cs, gpio_num_t clk, gpio_num_t mosi, gpio_num_t miso):cs(cs),clk(clk),mosi(mosi),miso(miso)
{
    this->slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    this->host = SDSPI_HOST_DEFAULT();
    this->mount_config.format_if_mount_failed = false;
    this->mount_config.max_files = 5;
    this->mount_config.allocation_unit_size = 16 * 1024;



    slot_config.gpio_miso = miso;
    slot_config.gpio_mosi = mosi;
    slot_config.gpio_sck  = clk;
    slot_config.gpio_cs   = cs;

}

void SdCard::Init(void)
{
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(SDCARD_TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(SDCARD_TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
    }
    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
}

size_t SdCard::ReadBinaryFile(char* fileName, uint8_t *data, uint32_t offset,uint32_t len)
{
    size_t readLen = 0xFFFFFFFFu;
    string <50>str("/sdcard/");
    str += fileName;
    // Use POSIX and C standard library functions to work with files.
    // First create a file.
    ESP_LOGI(SDCARD_TAG, "Opening file %s", str.c_str());
    this->f = fopen(str.c_str(), "rb");
    if (this->f == NULL) {
        ESP_LOGE(SDCARD_TAG, "Failed to open file for reading");
        return readLen;
    }

    if(this->f != NULL)
    {

        if(fseek(this->f,offset,SEEK_SET) != 0)
        {
            ESP_LOGI(SDCARD_TAG, "Failed to set position in the file %s", str.c_str());
            return readLen;
        }
        readLen = fread(data,sizeof(uint8_t),len,this->f); 
        ESP_LOGI(SDCARD_TAG, "Read bytes %d", readLen);
    }
    else
    {
        ESP_LOGE(SDCARD_TAG, "Failed to read");    
    }

    fclose(f);
    return readLen;
}

size_t SdCard::WriteBinaryFile(char* fileName, uint8_t *data, uint16_t len)
{
    size_t wroteLen = 0xFFFFFFFF;
    string <50>str("/sdcard/");
    str += fileName;
        
    // Use POSIX and C standard library functions to work with files.
    // First create a file.
    ESP_LOGI(SDCARD_TAG, "Opening file %s", str.c_str());
    this->f = fopen(str.c_str(), "ab");
    if (this->f == NULL) {
        ESP_LOGE(SDCARD_TAG, "Failed to open file for writing");
        return wroteLen;
    }
    if(this->f != NULL)
    {
        wroteLen = fwrite(data,sizeof(uint8_t),len,this->f); 
        ESP_LOGI(SDCARD_TAG, "Wrote bytes %d", wroteLen);
        ESP_LOGI(SDCARD_TAG, "Len bytes %d", len);
    }
    else
    {
        ESP_LOGE(SDCARD_TAG, "Failed to write");    
    }
    fclose(f);
    return wroteLen;
    
}





void SdCard::DeleteFile(char* fileName)
{
    string <50>str("/sdcard/");
    str += fileName;
    struct stat st;
    if (stat(str.c_str(), &st) == 0) {
        // Delete it if it exists
        unlink(str.c_str());
    }
}


size_t SdCard::GetFileLen(char *fileName)
{
    string <50>str("/sdcard/");
    str += fileName;
    size_t fileLen = 0u;
    struct stat st;
    if (stat(str.c_str(), &st) == 0) {
        // Delete it if it exists
        fileLen = st.st_size;
    }
    return fileLen;
}

SdCard::~SdCard()
{
    esp_vfs_fat_sdmmc_unmount();
} 