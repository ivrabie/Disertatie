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
#include <../../etl/observer.h>
#define GATT_CLIENT_NAME "GATTC"

#define GATTCL_MAX_OBSERVERS 2u

typedef struct
{
	esp_gattc_cb_event_t event;
	esp_gatt_if_t gattc_if;
	esp_ble_gattc_cb_param_t *param;
}GattcEventInfo;
extern etl::ifunction<GattcEventInfo>* gattcEventHandler;
namespace BLE
{
typedef etl::observer<GattcEventInfo> Gattc_Observer;
class GattClient:public etl::observable<Gattc_Observer, GATTCL_MAX_OBSERVERS>
{

 private:


	
 public:

	static GattClient gattc;
	 BleAdapter &bleAdapter = BleAdapter::getInstance();
	 GattClient();

	 esp_err_t Init(void);
	 ~GattClient();
	void RegisterApp(uint16_t appId);
	esp_err_t SearchService(esp_gatt_if_t gattc_if, uint16_t conn_id);
	esp_err_t OpenConnection(esp_gatt_if_t gattc_if, esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type, bool is_direct);
	esp_err_t RegisterForNotify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, uint16_t handle);
	void ReadChar(esp_gatt_if_t gattc_if,uint16_t conn_id, uint16_t handle,esp_gatt_auth_req_t auth_req);
	void RefreshCash(esp_bd_addr_t remote_bda);
	esp_gatt_status_t  GetService(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_bt_uuid_t *svc_uuid, esp_gattc_service_elem_t *result, uint16_t *count, uint16_t offset);
	esp_gatt_status_t GetCharByUuid(esp_gatt_if_t gattc_if, 
							uint16_t conn_id,
							uint16_t start_handle,
							uint16_t end_handle,
							esp_bt_uuid_t char_uuid,
							esp_gattc_char_elem_t *result,
							uint16_t *count);
	esp_gatt_status_t GetDescByUuid(esp_gatt_if_t gattc_if,
							uint16_t conn_id,
							uint16_t start_handle,
							uint16_t end_handle,
							esp_bt_uuid_t char_uuid,
							esp_bt_uuid_t descr_uuid,
							esp_gattc_descr_elem_t *result,
							uint16_t *count);
	void ReadDesc(esp_gatt_if_t gattc_if,uint16_t conn_id, uint16_t handle,esp_gatt_auth_req_t auth_req);
	void gattcCallback(GattcEventInfo info);
	esp_err_t RequestWriteChar(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req);
	esp_err_t RequestWriteDesc(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req);
	static GattClient& getInstance(void);
 };

}




#endif /* BLE_GATT_CLIENT_GATTCLIENT_H_ */
