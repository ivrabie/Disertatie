/*
 * SWL_Gateway.h
 *
 *  Created on: Jul 20, 2019
 *      Author: ion
 */

#ifndef SWL_GATEWAY_SWL_GATEWAY_H_
#define SWL_GATEWAY_SWL_GATEWAY_H_

#include<stdint.h>
#include "../ble/Observer.h"
#include "../FlashDevice/FlashDevice.h"
#include <esp_gap_ble_api.h>
#include "FlashDeviceManager/FlashDeviceManager.h"
#include "../ble/advertising/GapAdapter.h"
#include "../ble/gatt_client/GattClient.h"

#include "../wifi/WifiAdapter.h"
#include "../nvs_adapter/NvsAdapter.h"
using namespace BLE;
using namespace NVS_ADAPTER;
using namespace WIFI_ADAPTER;
namespace SWL_GATEWAY
{
	class SwlGateway:BLE::Observer
	{


	private:

		void processScanData(esp_ble_gap_cb_param_t *param);

		void processRawData(uint8_t *data, uint8_t len, FlashDevice *device);
		void buildManufactureDataBuffer(uint8_t *data, uint8_t &len);
		void triggerConnetion(void);
	public:
		uint16_t new_version;
		FlashDeviceManager flashDeviceManager;
		GapAdapter *gapAdapter;
		GattClient *gattc;
		NVSAdapter *nvsAdapter;
		WifiAdapter* wifiAdapter;



		SwlGateway(uint16_t appId,GapAdapter *gapAdapter,GattClient *gattc,NVSAdapter *nvsAdapter,WifiAdapter *wifiAdapter);
		~SwlGateway();

		void Init(void);



		void updateGap(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

		void updateGattc(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param);

		void updateGatts(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
	};
}


//void SWLGW_Init(void);
//uint16_t SWLGW_GetCurrentVersion(void);
//void SWLGW_SetCurrentVersion(uint16_t version);
//void SWLGW_AdvDataIndication(uint8_t *data,
//							 uint8_t len,
//							 esp_bd_addr_t bda,
//							 esp_ble_addr_type_t ble_addr_type);
#endif /* SWL_GATEWAY_SWL_GATEWAY_H_ */
