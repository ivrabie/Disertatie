/*
 * ble_device.h
 *
 *  Created on: Aug 1, 2019
 *      Author: ion
 */

#ifndef BLE_BLE_DEVICE_BLE_DEVICE_H_
#define BLE_BLE_DEVICE_BLE_DEVICE_H_
#include "BleRemoteDevice_cfg.h"
#include <esp_gattc_api.h>
namespace BLE
{

	class BleRemoteDevice
	{
	public:
		BleRemoteDevice();
		BleRemoteDevice(const BleRemoteDevice& bleRemoteDev);
		~BleRemoteDevice();
		esp_bd_addr_t       bda;
		esp_ble_addr_type_t ble_addr_type;
		uint8_t  deviceName[BLE_DEVICEINFO_EXPECTED_LEN_NAME];
		esp_gatt_status_t status;       /*!< Operation status */
		uint16_t conn_id;               /*!< Connection id */
		uint16_t mtu;

		BleRemoteDevice& operator=(const BleRemoteDevice &dev);
	};
}




#endif /* BLE_BLE_DEVICE_BLE_DEVICE_H_ */
