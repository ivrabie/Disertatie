/*
 * SWL_Gateway.h
 *
 *  Created on: Jul 20, 2019
 *      Author: ion
 */

#ifndef SWLOAD_SWLOAD_H_
#define SWLOAD_SWLOAD_H_

#include<stdint.h>
#include "../ble/Observer.h"
#include "../FlashDevice/FlashDevice.h"
#include <esp_gap_ble_api.h>
#include "../ble/advertising/GapAdapter.h"

#include "../nvs_adapter/NvsAdapter.h"
#include "../SWLoad/FlashDeviceManager/FlashDeviceManager.h"
#include "../ble/gatt_server/GattServer.h"
#include "FlashingService/FlashingService.h"
using namespace BLE;
using namespace NVS_ADAPTER;
namespace SWLOAD
{
	class SwLoad:BLE::Observer
	{

	typedef enum
	{
		SwLoadAppState_IDLE,
		SwLoadAppState_SCANNING,
		SwLoadAppState_WORKINING,
	}SwLoadAppStateType;
	private:

		SwLoadAppStateType state;
		FlashDevice device;
		void processRawData(uint8_t *data, uint8_t len, FlashDevice *device);
	public:

		uint16_t   currentVersion;
		FlashinService flashService;
		GapAdapter *gapAdapter;
		NVSAdapter *nvsAdapter;
		GattServer *gattServer;
		SwLoad(uint16_t appId,GapAdapter *gapAdapter,NVSAdapter *nvsAdapter,GattServer *gattServer);
		~SwLoad();

		void Init(void);


	    void updateGap(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);


		void updateGattc(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param);

		void updateGatts(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
	};
}


#endif /* SWLOAD_SWLOAD_H_ */
