/*
 * 
 *
 *  Created on: Jul 19, 2019
 *      Author: ion
 */

#ifndef BLE_ADVERTISING_GAPADAPTER_H_
#define BLE_ADVERTISING_GAPADAPTER_H_
#include <esp_err.h>
#include <esp_gap_ble_api.h>
#include "../BleAdapter.h"
#include "GapAdapter_cfg.h"
#include "../../etl/observer.h"
typedef struct
{
	esp_gap_ble_cb_event_t event;
	esp_ble_gap_cb_param_t *param;

} GapEventInfo;

#define GAP_MAX_OBSERVERS 2u

extern etl::ifunction<GapEventInfo>* gapEventHandler;


namespace BLE
{
typedef etl::observer<GapEventInfo> Gap_Observer;
	class GapAdapter:public etl::observable<Gap_Observer, GAP_MAX_OBSERVERS>
	{
#define MAX_RAW_DATA_SIZE 31u
		typedef enum
		{
			GapAdpapterState_DEINIT,
			GapAdpapterState_INIT,
			GapAdpapterState_SCANING,
			GapAdpapterState_ADVERTISING,
		}GapAdpapterStateType;

		private:
			
			GapAdapter();
			~GapAdapter();

		public:
		uint8_t rawData[MAX_RAW_DATA_SIZE];
		uint8_t rawDataSize;
		static GapAdapter gapAdapter;

		int8_t addAdvRawData(uint8_t *data, uint8_t len,esp_ble_adv_data_type dataType);
		esp_err_t IsBleStackInit();
		void gap_event_handler(GapEventInfo info);


		esp_ble_adv_params_t adv_params;
		BleAdapter  &bleAdapter = BleAdapter::getInstance();





		esp_ble_scan_params_t scanParams;


		static GapAdapter& getInstance(void);
		esp_err_t Init();
		esp_err_t StartScan(uint16_t duration);
		esp_err_t StartScan(uint16_t duration,esp_ble_scan_params_t *scanParams);
		esp_err_t StopScan(void);
		int8_t   StartAdvertising(uint8_t *manufactureData, uint8_t len);
		int8_t   StartAdvertising(void);
		int8_t   SetDeviceName(uint8_t *devName, uint8_t len);
	};


}


#endif /* BLE_ADVERTISING_GAPADAPTER_H_ */
