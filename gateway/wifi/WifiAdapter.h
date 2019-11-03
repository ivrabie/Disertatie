/*
 * wifi_usr.h
 *
 *  Created on: Jul 17, 2019
 *      Author: ion
 */

#ifndef WIFI_WIFIADAPTER_H_
#define WIFI_WIFIADAPTER_H_


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <esp_wifi_types.h>



namespace WIFI_ADAPTER
{
enum WIFI_STATE_TYPE
{
	WIFI_STATE_NOT_INIT,
	WIFI_STATE_INIT,
	WIFI_STATE_START,
	WIFI_STATE_CONNECTED
};
	class WifiAdapter
	{
		static WifiAdapter wifiAdapter;
		WifiAdapter();
	public:
		
		static WIFI_STATE_TYPE state;
		static int s_retry_num;
		static const int WIFI_CONNECTED_BIT = BIT0;
		static EventGroupHandle_t s_wifi_event_group;
		wifi_config_t wifi_config;

		~WifiAdapter();

		void Init(void);
		void Connect(void);
		static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
		static WifiAdapter& getInstance(void); 
	};
}





#endif /* WIFI_WIFIADAPTER_H_ */
