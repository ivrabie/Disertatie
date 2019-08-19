/*
 * GattClient.h
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#ifndef BLE_GATT_SERVER_GATTCLIENT_H_
#define BLE_GATT_SERVER_GATTCLIENT_H_
#include "../BleAdapter.h"
#include <esp_err.h>
#include <esp_gatts_api.h>
#include "../Observer.h"
#include "../gatt_service/GattService.h"

#define GATT_SERVER_NAME "GATTS"
namespace BLE
{


 class GattServer
 {

 private:

#define MAX_OBSERVERS 5u
		static Observer *obsReg[MAX_OBSERVERS];
		static uint8_t  noObsReg;



		static void gattsCallback(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
 public:


	 BleAdapter * bleAdapter;
	 GattServer(BleAdapter *ble);

	 esp_err_t Init(void);
	 ~GattServer();

	void RegisterApp(uint16_t appId);
	esp_err_t OpenConnection(esp_gatt_if_t gatts_if, esp_bd_addr_t remote_bda, bool is_direct);
	esp_err_t RegisterService(esp_gatt_if_t gatts_if,GattService *service);
	void AttachObserver(Observer *obs);
	void DeleteObserver(Observer *obs);
	void DeleteObserver(uint8_t idx);

 };

}




#endif
