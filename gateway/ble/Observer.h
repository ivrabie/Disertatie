/*
 * Observer.h
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#ifndef SWL_GATEWAY_OBSERVER_H_
#define SWL_GATEWAY_OBSERVER_H_
#include "esp_bt_defs.h"
#include <esp_gattc_api.h>
#include <esp_gatts_api.h>
#include <esp_gap_ble_api.h>
namespace BLE
{

	class Observer
	{
	public:

		uint16_t appId;
		uint16_t gattcif;
		esp_gatt_if_t gattsif;
		Observer(uint16_t appId);
		virtual ~Observer();
		virtual void updateGap(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) = 0;
		virtual void updateGattc(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param) = 0;


		virtual void updateGatts(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) = 0;


		static bool AttachObs(Observer *listobs[], uint8_t &current_len, uint8_t max_len,  Observer *obs);
		static void DeleteObs(Observer *listobs[], uint8_t &current_len, Observer *obs);
		static void DeleteObs(Observer *listobs[], uint8_t &current_len, uint8_t idx);


	};




}



#endif /* SWL_GATEWAY_OBSERVER_H_ */
