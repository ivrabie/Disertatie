/*
 * SWL_Gateway.h
 *
 *  Created on: Jul 20, 2019
 *      Author: ion
 */

#ifndef SWLOAD_SWLOAD_H_
#define SWLOAD_SWLOAD_H_

#include<stdint.h>

#include "../FlashDevice/FlashDevice.h"
#include <esp_gap_ble_api.h>
#include "../ble/advertising/GapAdapter.h"
#include "../ble/gatt_server/GattServer.h"
#include "../nvs_adapter/NvsAdapter.h"
#include "../ble/gatt_server/GattServer.h"
using namespace BLE;
using namespace NVS_ADAPTER;

namespace SWLOAD
{
	class SwLoad:public Gap_Observer, public Gatts_Observer
	{
#define FLASH_STATE_MACHINE 0u
	typedef enum
	{
		SwLoadAppState_IDLE,
		SwLoadAppState_SCANNING,
		SwLoadAppState_WORKINING,
	}SwLoadAppStateType;
	private:


		void processRawData(uint8_t *data, uint8_t len, FlashDevice *device);
	public:

		uint16_t appId;
		GapAdapter &gapAdapter = GapAdapter::getInstance();
		NVSAdapter &nvsAdapter = NVSAdapter::getInstance();
		GattServer &gattServer = GattServer::getInstance();
		FlashDevice flashDevice;
		uint32_t   currentVersion;
		SwLoadAppStateType state;
		uint16_t gatts_if;
		SwLoad(uint16_t appId);
		~SwLoad();

		void Init(void);


	    void notification(GapEventInfo info);
		void notification(GattsEventInfo info);
	};
}


#endif /* SWLOAD_SWLOAD_H_ */
