/*
 * FlashDeviceManager.h
 *
 *  Created on: Aug 6, 2019
 *      Author: ion
 */

#ifndef SWLOAD_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_
#define SWLOAD_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_

#include <stdint.h>
#include <esp_bt_defs.h>

#include "../../SWLoad/FlashDevice/FlashDevice.h"
namespace SWLOAD
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

			void ReadChr();
			void ReadDesc();

	private:
			void updateField(uint8_t idx, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);

	};
}


#endif /* SWLOAD_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_ */
