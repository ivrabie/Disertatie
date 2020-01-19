/*
 * GattServiceClient.h
 *
 *  Created on: Aug 12, 2019
 *      Author: ion
 */

#ifndef BLE_GATT_SERVICE_GATTSERVICECLIENT_H_
#define BLE_GATT_SERVICE_GATTSERVICECLIENT_H_

#include "../gatt_service/GattService.h"
#include "../gatt_client/GattClient.h"
namespace BLE
{
#define GATT_SERVICECLIENT_NAME "GATT_SERVICE_CLIENT"
	class GattServiceClient:public GattService
	{
		private:
		

		public:
		GattClient& gattc = GattClient::getInstance();
		GattServiceClient();
		GattServiceClient(const GattServiceClient &servClient);
		~GattServiceClient();
		esp_err_t RequestWriteAttr(esp_gatt_if_t gattc_if, uint16_t conn_id,esp_bt_uuid_t *uuid, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req);
		void RequestReadAttr(esp_gatt_if_t gattc_if, uint16_t conn_id,esp_bt_uuid_t *uuid);
		void RefreshServiceInfo(esp_bd_addr_t remote_bda);
		bool NotifyDiscoverComplete(esp_gatt_if_t gattc_if, uint16_t conn_id);
		esp_err_t RegisterForNotify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, esp_bt_uuid_t *uuid);
		GattServiceClient& operator=(const GattServiceClient &gattService);
		void OpenConnection(esp_gatt_if_t gattc_if, esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type, bool is_direct);
		void DisconnectService(esp_gatt_if_t gattc_if, uint16_t conn_id);
	};
}


#endif /* BLE_GATT_SERVICE_GATTSERVICECLIENT_H_ */
