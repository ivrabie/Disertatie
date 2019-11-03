/*
 * wifi_usr.c
 *
 *  Created on: Jul 17, 2019
 *      Author: ion
 */


#include <esp_wifi.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <tcpip_adapter.h>
#include <string.h>


#include "WifiAdapter.h"
#include "WifiAdapter_config.h"


using namespace WIFI_ADAPTER;

#define EXAMPLE_ESP_MAXIMUM_RETRY 3u

WifiAdapter WifiAdapter::wifiAdapter;
int WifiAdapter::s_retry_num = 0;
EventGroupHandle_t WifiAdapter::s_wifi_event_group;
WIFI_STATE_TYPE WifiAdapter::state;

WifiAdapter::WifiAdapter()
{

}

WifiAdapter::~WifiAdapter()
{

}


void WifiAdapter::Init(void)
{

	if(WifiAdapter::state == WIFI_STATE_NOT_INIT)
	{
		WifiAdapter::s_wifi_event_group = xEventGroupCreate();

		wifi_init_config_t WIFI_config = WIFI_INIT_CONFIG_DEFAULT();




		tcpip_adapter_init();


		ESP_ERROR_CHECK(esp_event_loop_create_default());

		ESP_ERROR_CHECK(esp_wifi_init(&WIFI_config));

		ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,    WifiAdapter::event_handler, NULL));
		ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,   IP_EVENT_STA_GOT_IP, WifiAdapter::event_handler, NULL));

		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODULE_MODE));

		WifiAdapter::state = WIFI_STATE_INIT;
	}



}

void  WifiAdapter::event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        WifiAdapter::state =  WIFI_STATE_START;
        ESP_LOGI(WIFI_MODULE_NAME, "Wifi started");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (WifiAdapter::s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            xEventGroupClearBits(WifiAdapter::s_wifi_event_group, WifiAdapter::WIFI_CONNECTED_BIT);
            WifiAdapter::s_retry_num++;
            ESP_LOGI(WIFI_MODULE_NAME, "retry to connect to the AP");

        }
        else
        {
        	WifiAdapter::state =  WIFI_STATE_INIT;
        }
        ESP_LOGI(WIFI_MODULE_NAME,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(WIFI_MODULE_NAME, "got ip:%s",
                 ip4addr_ntoa(&event->ip_info.ip));
        WifiAdapter::s_retry_num = 0;
        xEventGroupSetBits(WifiAdapter::s_wifi_event_group, WifiAdapter::WIFI_CONNECTED_BIT);
        WifiAdapter::state =  WIFI_STATE_CONNECTED;

    }
}
void  WifiAdapter::Connect(void)
{
	if(WifiAdapter::state ==  WIFI_STATE_NOT_INIT)
	{
		return;
	}
	else if(WifiAdapter::state ==  WIFI_STATE_START)
	{
		return;
	}
	else if(WifiAdapter::state ==  WIFI_STATE_CONNECTED)
	{
		return;
	}
	else
	{
		memcpy(this->wifi_config.sta.ssid,WIFI_MODULE_SSID,sizeof(WIFI_MODULE_SSID));
		memcpy(this->wifi_config.sta.password,WIFI_MODULE_PASS,sizeof(WIFI_MODULE_PASS));

		ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
		ESP_ERROR_CHECK(esp_wifi_start());


		ESP_LOGI(WIFI_MODULE_NAME, "wifi_init_sta finished.");
		ESP_LOGI(WIFI_MODULE_NAME, "connect to app SSID:%s password:%s",this->wifi_config.sta.ssid, this->wifi_config.sta.password);

	}

}
WifiAdapter& WifiAdapter::getInstance(void)
{
	return WifiAdapter::wifiAdapter;
}





