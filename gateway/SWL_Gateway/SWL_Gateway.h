/*
 * SWL_Gateway.h
 *
 *  Created on: Jul 20, 2019
 *      Author: ion
 */

#ifndef SWL_GATEWAY_SWL_GATEWAY_H_
#define SWL_GATEWAY_SWL_GATEWAY_H_

#include<stdint.h>
#include "../FlashDevice/FlashDevice.h"
#include <esp_gap_ble_api.h>
#include "FlashDeviceManager/FlashDeviceManager.h"
#include "../ble/advertising/GapAdapter.h"
#include "../ble/gatt_client/GattClient.h"

#include "../wifi/WifiAdapter.h"
#include "../nvs_adapter/NvsAdapter.h"
#include "../HttpClient/HttpClient.h"
#include "../sdcard/sdcard.h"
#include "UpdateFileApp/UpdateFileApp.h"
#include "SwlOled/SwlOled.h"
#include "esp_bt.h"
#include "../../Ssd1306/ssd1306.hpp"
using namespace BLE;
using namespace NVS_ADAPTER;
using namespace WIFI_ADAPTER;
using namespace HTTP_CLIENT;
using namespace SDCARD;
namespace SWL_GATEWAY{
	class SwlGateway:public Gap_Observer,public Http_Observer, public Gattc_Observer{
	private:
		bool isScanningRunning = false;
		bool isFlashingInProgress = false;
		void processScanData(esp_ble_gap_cb_param_t *param);
		void processRawData(uint8_t *data, uint8_t len,  FlashDevice *flashDev);
		void buildManufactureDataBuffer(uint8_t *data, uint8_t &len);
		void triggerConnetion(void);
	public:
		uint16_t appId;
		SdCard &sd;
		UpdateFileApp& updateFileApp;
		VersionManager &vers;
		SWL_OLED::SwlOled &swlOled;
		GapAdapter &gapAdapter = GapAdapter::getInstance();
		GattClient &gattc = GattClient::getInstance();
		WifiAdapter  &wifiAdapter = WifiAdapter::getInstance();
		FlashDeviceManager flashDeviceManager;
		int16_t gattc_if = 0;
		SwlGateway(uint16_t appId,SdCard &sd,UpdateFileApp& updateFileApp,VersionManager &vers,SWL_OLED::SwlOled &swlOled);
		~SwlGateway();
		void Init(void);
		void notification(GapEventInfo info);
		void notification(GattcEventInfo info);
		void notification(esp_http_client_event_t * evt);
		void TaskMain_100ms(void);
	};}

#endif /* SWL_GATEWAY_SWL_GATEWAY_H_ */
