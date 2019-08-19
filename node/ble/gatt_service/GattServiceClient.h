/*
 * GattServiceClient.h
 *
 *  Created on: Aug 12, 2019
 *      Author: ion
 */

#ifndef BLE_GATT_SERVICE_GATTSERVICECLIENT_H_
#define BLE_GATT_SERVICE_GATTSERVICECLIENT_H_

#include "../gatt_service/GattService.h"
namespace BLE
{
#define GATT_SERVICECLIENT_NAME "GATT_SERVICE_CLIENT"
	class GattServiceClient:public GattService
	{
		public:
		GattServiceClient();
		GattServiceClient(const GattServiceClient &servClient);
		~GattServiceClient();
		void RequestReadChr(esp_gatt_if_t gattc_if, uint16_t conn_id,esp_bt_uuid_t *uuid);
		void RefreshServiceInfo(esp_bd_addr_t remote_bda);
		void NotifyDiscoverComplete(esp_gatt_if_t gattc_if, uint16_t conn_id);
		void RegisterForNotify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, uint16_t handle);
	};
}


#endif /* BLE_GATT_SERVICE_GATTSERVICECLIENT_H_ */
