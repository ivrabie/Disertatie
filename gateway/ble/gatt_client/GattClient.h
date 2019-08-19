/*
 * GattClient.h
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#ifndef BLE_GATT_CLIENT_GATTCLIENT_H_
#define BLE_GATT_CLIENT_GATTCLIENT_H_
#include "../BleAdapter.h"
#include <esp_err.h>
#include <esp_gattc_api.h>
#include "../Observer.h"
#define GATT_CLIENT_NAME "GATTC"
namespace BLE
{


 class GattClient
 {

 private:

#define MAX_OBSERVERS 5u
		static Observer *obsReg[MAX_OBSERVERS];
		static uint8_t  noObsReg;

	   static void gattcCallback(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param);
 public:


	 BleAdapter * bleAdapter;
	 GattClient(BleAdapter *ble);

	 esp_err_t Init(void);
	 ~GattClient();
	void RegisterApp(uint16_t appId);
	esp_err_t SearchService(esp_gatt_if_t gattc_if, uint16_t conn_id);
	esp_err_t OpenConnection(esp_gatt_if_t gattc_if, esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type, bool is_direct);
	void AttachObserver(Observer *obs);
	void DeleteObserver(Observer *obs);
	void DeleteObserver(uint8_t idx);

 };

}




#endif /* BLE_GATT_CLIENT_GATTCLIENT_H_ */
