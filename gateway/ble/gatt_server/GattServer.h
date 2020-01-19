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
#include "../gatt_service/GattService.h"
#include "../../etl/observer.h"
#define GATT_SERVER_NAME "GATTS"
#define GATTS_MAX_OBSERVERS 2u
typedef struct
{
	esp_gatts_cb_event_t event;
	esp_gatt_if_t gatts_if; 
	esp_ble_gatts_cb_param_t *param;
}GattsEventInfo;

extern etl::ifunction<GattsEventInfo>* gattsEventHandler;
namespace BLE
{

typedef etl::observer<GattsEventInfo> Gatts_Observer;
 class GattServer:public etl::observable<Gatts_Observer, GATTS_MAX_OBSERVERS>
 {
	public:
	 BleAdapter& bleAdapter = BleAdapter::getInstance();
	 static GattServer gattServer;
	 GattServer();
	 esp_err_t Init(void);
	 ~GattServer();
	void RegisterApp(uint16_t appId);
	esp_err_t OpenConnection(esp_gatt_if_t gatts_if, esp_bd_addr_t remote_bda, bool is_direct);
	esp_err_t RegisterService(esp_gatt_if_t gatts_if,GattService *service);
	esp_err_t SendResponse(esp_gatt_if_t gatts_if, uint16_t conn_id, uint32_t trans_id,
                    esp_gatt_status_t status, esp_gatt_rsp_t *rsp);
	esp_err_t StartService(uint16_t service_handle);
	esp_err_t SendIndication(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle,
                        uint16_t value_len, uint8_t *value, bool need_confirm);
	esp_err_t AddCharDescr(uint16_t service_handle,
                            esp_bt_uuid_t   *descr_uuid,
                            esp_gatt_perm_t perm, esp_attr_value_t *char_descr_val,
                            esp_attr_control_t *control);
	esp_err_t AddChar(uint16_t service_handle,  esp_bt_uuid_t  *char_uuid,
                    esp_gatt_perm_t perm, esp_gatt_char_prop_t property, esp_attr_value_t *char_val,
                    esp_attr_control_t *control);
	void gattsCallback(GattsEventInfo info);
	static GattServer& getInstance(void);
 };
}




#endif
