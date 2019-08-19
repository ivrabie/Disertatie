/*
 * FlashDeviceManager.h
 *
 *  Created on: Aug 6, 2019
 *      Author: ion
 */

#ifndef SWL_GATEWAY_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_
#define SWL_GATEWAY_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_

#include "../FlashDevice/FlashDevice.h"
#include <stdint.h>
#include <esp_bt_defs.h>
namespace SWL_GATEWAY
{
typedef enum
{
	DEVICE_UPDATEFIELD_CONECTION_ID,
	DEVICE_UPDATEFIELD_CONECTION_STATUS,
	DEVICE_UPDATEFIELD_CONECTION_MTU
}DEVICE_UPDATEFIELD_TYPES;
	class FlashDeviceManager
	{
	public:
	#define MAX_BLE_DEVICES 5u
			FlashDevice listOfDevice[MAX_BLE_DEVICES];
			uint8_t numberOfDevices;
			FlashDeviceManager();
			~FlashDeviceManager();
			void RegisterDevice(FlashDevice *device);
			void RemoveDevice(uint16_t conn_id);
			void RemoveDevice(esp_bd_addr_t bda);
			void UpdateDevice(uint16_t conn_id, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);
			void UpdateDevice(esp_bd_addr_t bda, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);
			void RefreshServiceDiscover(uint16_t conn_id);
			void NotifyDiscoverComplete(esp_gatt_if_t gattc_if,uint16_t conn_id);

			void RequestVersion(esp_gatt_if_t gattc_if, uint16_t conn_id);
			void NotifyEvtChr(uint16_t conn_id,uint16_t handle,uint8_t *value,uint16_t value_len, uint8_t type);
			void RegisterForNotifyBlockReq(esp_gatt_if_t gattc_if, uint16_t conn_id);
	private:
			void updateField(uint8_t idx, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);

	};
}


#endif /* SWL_GATEWAY_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_ */
