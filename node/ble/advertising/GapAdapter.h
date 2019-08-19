/*
 * advertising.h
 *
 *  Created on: Jul 19, 2019
 *      Author: ion
 */

#ifndef BLE_ADVERTISING_GAPADAPTER_H_
#define BLE_ADVERTISING_GAPADAPTER_H_
#include <esp_err.h>
#include <esp_gap_ble_api.h>
#include "../Observer.h"
#include "../BleAdapter.h"
#include "GapAdapter_cfg.h"
namespace BLE
{
	class GapAdapter
	{
#define MAX_OBSERVERS 5u
#define MAX_RAW_DATA_SIZE 31u
		typedef enum
		{
			GapAdpapterState_DEINIT,
			GapAdpapterState_INIT,
			GapAdpapterState_SCANING,
			GapAdpapterState_ADVERTISING,
		}GapAdpapterStateType;
		private:
		uint8_t rawData[MAX_RAW_DATA_SIZE];
		uint8_t rawDataSize;


		int8_t addAdvRawData(uint8_t *data, uint8_t len,esp_ble_adv_data_type dataType);
		esp_err_t IsBleStackInit();
		static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
		static void processScanData(esp_ble_gap_cb_param_t *param);


		public:


			static Observer *obsReg[MAX_OBSERVERS];
			static uint8_t noObsReg;
			static esp_ble_adv_params_t adv_params;
			BleAdapter * bleAdapter;



			GapAdapter(BleAdapter *ble);
			~GapAdapter();

			esp_ble_scan_params_t scanParams;



			esp_err_t Init();
			esp_err_t StartScan(uint16_t duration);
			esp_err_t StartScan(uint16_t duration,esp_ble_scan_params_t *scanParams);
			esp_err_t StopScan(void);
			int8_t   StartAdvertising(uint8_t *manufactureData, uint8_t len);
			int8_t   StartAdvertising(void);
			int8_t   SetDeviceName(uint8_t *devName, uint8_t len);

			void AttachObserver(Observer *obs);
			void DeleteObserver(Observer *obs);
			void DeleteObserver(uint8_t idx);


	};


}


#endif /* BLE_ADVERTISING_GAPADAPTER_H_ */
