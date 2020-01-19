
/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/




#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_log.h>
#include <sdkconfig.h>

#include "WifiAdapter.h"
#include "NvsAdapter.h"
#include "BleAdapter.h"
#include "GapAdapter.h"
#include "SWL_Gateway.h"
#include "../etl/vector.h"
#include "../etl/cstring.h"
#include "../HttpClient/HttpClient.h"
#include "../sdcard/sdcard.h"
#include "../SWL_Gateway/VersionManager/VersionManager.h"
#include "../SWL_Gateway/UpdateFileApp/UpdateFileApp.h"
#include "../Ssd1306/ssd1306.hpp"
#include "../SWL_Gateway/SwlOled/SwlOled.h"
using namespace NVS_ADAPTER;
using namespace WIFI_ADAPTER;
using namespace HTTP_CLIENT;
using namespace SWL_GATEWAY;
#define TASK_CYCLE_10ms 10u
#define TASK_CYCLE_100ms 100u
#define TASK_PRIO 5u


#define STACK_SIZE 8192u

#define SWL_APP 0x0001u
#define PIN_NUM_MISO UINT8_C(2)
#  define PIN_NUM_MOSI UINT8_C(15)
#define PIN_NUM_CLK  UINT8_C(14)
#define PIN_NUM_CS   UINT8_C(13)
#define PIN_NUM_I2C_SDA UINT8_C(18)
#define PIN_NUM_I2C_SCL UINT8_C(19)
// Init global classes
OLED oled((gpio_num_t)PIN_NUM_I2C_SCL,(gpio_num_t)PIN_NUM_I2C_SDA,SSD1306_128x64);
SWL_OLED::SwlOled swlOled(oled);
SDCARD::SdCard sdcard((gpio_num_t)PIN_NUM_CS,(gpio_num_t)PIN_NUM_CLK,(gpio_num_t)PIN_NUM_MOSI,(gpio_num_t)PIN_NUM_MISO);
VersionManager versMng;
UpdateFileApp updateFile(sdcard,versMng,swlOled);
SWL_GATEWAY::SwlGateway swlApp(SWL_APP,sdcard,updateFile,versMng,swlOled); // @suppress("Abstract class cannot be instantiated")



extern "C" void vTaskCode_100ms( void * pvParameters )
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount ();
	uint32_t free_heap = 0;
    while(1)
    {
    	vTaskDelayUntil(&xLastWakeTime, TASK_CYCLE_100ms/portTICK_PERIOD_MS );
    	free_heap = esp_get_free_heap_size();
        swlApp.TaskMain_100ms();
    	if(free_heap <1024)
    	{
    		ESP_LOGE("main","low memory %d", free_heap);
    	}
    }
}

#define MAIN_TAG "MAIN"
extern "C" void app_main()
{
	TaskHandle_t xHandle_10ms = NULL;
    TaskHandle_t xHandle_100ms = NULL;
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    ESP_LOGI(MAIN_TAG,"This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    ESP_LOGI(MAIN_TAG,"silicon revision %d, ", chip_info.revision);
    ESP_LOGI(MAIN_TAG,"%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    swlApp.Init();
    BaseType_t retVal = xTaskCreate(vTaskCode_100ms,"Task_100ms",STACK_SIZE,( void * ) NULL,1,&xHandle_100ms );
        if( retVal != pdPASS)
        {
            ESP_LOGE("main","Task was not created correctly");
            esp_restart();
        }
}
