
/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/



#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include "SWLoad.h"
#include "NvsAdapter.h"
#include "BleAdapter.h"
#include "GapAdapter.h"

using namespace NVS_ADAPTER;
#define TASK_CYCLE_10ms 10u

#define TASK_PRIO 5u

// Dimensions the buffer that the task being created will use as its stack.
// NOTE:  This is the number of bytes the stack will hold, not the number of
// words as found in vanilla FreeRTOS.
#define STACK_SIZE 2048u

#define SWL_APP 0x0001u

// Task pointers

// Init global classes
SWLOAD::SwLoad swlApp(SWL_APP); // @suppress("Abstract class cannot be instantiated")
// Function that implements the task being created.
extern "C" void vTaskCode_10ms( void * pvParameters )
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount ();
	uint32_t free_heap = 0;
    while(1)
    {

    	vTaskDelayUntil(&xLastWakeTime, TASK_CYCLE_10ms/portTICK_PERIOD_MS );

    	//free_heap = esp_get_free_heap_size();
    	//ESP_LOGI("main","Task_10ms was called");
    }

}

#define MAIN_TAG "MAIN"

extern "C" void app_main()


{
	TaskHandle_t xHandle_10ms = NULL;
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

    /* Init code */
    swlApp.Init();


    if( xTaskCreate(
                     vTaskCode_10ms,       // Function that implements the task.
                     "Task_10ms",          // Text name for the task.
                     STACK_SIZE,        // Stack size in bytes, not words.
                     ( void * ) NULL,    // Parameter passed into the task.
					 	1,// Priority at which the task is created.
                     &xHandle_10ms ) != pdPASS  // Variable to hold the task's data structure.
)
    {
    	ESP_LOGE("main","Task was not created correctly");
    	esp_restart();
    }
}
