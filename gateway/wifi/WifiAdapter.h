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
	class WifiAdapter
	{
	public:
		static int s_retry_num;
		static const int WIFI_CONNECTED_BIT = BIT0;
		static EventGroupHandle_t s_wifi_event_group;


	public:
		WifiAdapter();
		~WifiAdapter();

		 void Init(void);
		 void Connect(void);
		 static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);

	};
}





#endif /* WIFI_WIFIADAPTER_H_ */
