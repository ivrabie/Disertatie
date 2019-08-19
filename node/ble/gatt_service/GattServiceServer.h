/*
 * GattServiceServer.h
 *
 *  Created on: Aug 12, 2019
 *      Author: ion
 */

#ifndef BLE_GATT_SERVICE_GATTSERVICESERVER_H_
#define BLE_GATT_SERVICE_GATTSERVICESERVER_H_

#include "GattService.h"
#include "../gatt_server/GattServer.h"
namespace BLE
{

	class GattServiceServer:public GattService
	{
	public:

		 esp_gatt_rsp_t  respValue;

		GattServiceServer();
		~GattServiceServer();
		GattServiceServer(const GattServiceServer &serServer);
		void StartService(void);
		bool NotifyServiceRegistred(esp_gatt_srvc_id_t *serviceInfo,uint16_t service_handle);
		bool NotifyAttrRegistred(uint16_t attr_handle,uint16_t service_handle,esp_bt_uuid_t *uuid);
		void CreateService(void);
		void SendIndication(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle, uint16_t value_len, uint8_t *value, bool need_confirm);
		void SendResponse(esp_gatt_if_t gatts_if, uint16_t  conn_id,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long, uint8_t *value, uint16_t value_len );

	};
}



#endif /* BLE_GATT_SERVICE_GATTSERVICESERVER_H_ */
