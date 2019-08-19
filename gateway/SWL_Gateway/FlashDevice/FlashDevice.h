/*
 * flashingService.h
 *
 *  Created on: Jul 18, 2019
 *      Author: ion
 */

#ifndef BLE_FLASHINGPROFILE_FLASHINGPROFILE_H_
#define BLE_FLASHINGPROFILE_FLASHINGPROFILE_H_


#include "esp_bt_defs.h"
#include "../../ble/ble_device/BleRemoteDevice.h"
#include "../FlashingService/FlashingService.h"
namespace SWL_GATEWAY
{
	class FlashDevice:public BLE::BleRemoteDevice
	{
	public:
		uint8_t flashStatus;
		uint8_t flashProgress;
		uint16_t flashMagicNumber;
		uint16_t flashVersion;
		FlashinService flashService;
		FlashDevice();
		FlashDevice(const FlashDevice& flashDev);
		~FlashDevice();





	};
}

#endif

//
//
//
//void FlashProfile_Init(void);
//
//
//#endif /* BLE_FLASHINGPROFILE_FLASHINGPROFILE_H_ */
