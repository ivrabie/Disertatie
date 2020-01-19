/*
 * advertising.c
 *
 *  Created on: Jul 19, 2019
 *      Author: ion
 */



#include <string.h>
#include <esp_log.h>
#include <esp_gap_ble_api.h>
#include <esp_bt_defs.h>
#include "GapAdapter.h"
#include "GapAdapter_cfg.h"
//#include "gatt_api.h"
using namespace BLE;




#define TEST_MANUFACTURER_DATA_LEN  17

#define SERVICE_DATA_TEST  5u



GapAdapter GapAdapter::gapAdapter;
etl::function_imp<GapAdapter, GapEventInfo, GapAdapter::gapAdapter, &GapAdapter::gap_event_handler> gap_callback;
etl::ifunction<GapEventInfo>* gapEventHandler = NULL;
extern "C"
{
	void gapEventHandlerCbk(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
	{
		GapEventInfo info;
		info.event = event;
		info.param = param;
		(*gapEventHandler)(info);
	}
}

GapAdapter::GapAdapter()
{
	this->scanParams.scan_type = BLE_SCAN_TYPE_ACTIVE;
	this->scanParams.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
	this->scanParams.scan_filter_policy =BLE_SCAN_FILTER_ALLOW_ALL;
	this->scanParams.scan_interval = 0x10;
	this->scanParams.scan_window = 0x5;
	this->scanParams.scan_duplicate =  BLE_SCAN_DUPLICATE_DISABLE;

	this->rawDataSize = 0u;
	this->adv_params.adv_int_min = 0x20;
	this->adv_params.adv_int_max = 0x40,
	this->adv_params.adv_type    = ADV_TYPE_IND,
	this->adv_params.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
	this->adv_params.channel_map  = ADV_CHNL_ALL;
	this->adv_params.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;



}
GapAdapter::~GapAdapter()
{

}
esp_err_t GapAdapter::IsBleStackInit()
{
	esp_err_t ret = ESP_FAIL;
	// if(this->bleAdapter != NULL)
	// {
	if(this->bleAdapter.bleAdapterState != BLEADAPTER_INIT)
	{
		ret = this->bleAdapter.Init();
	}
	else
	{
		ret = ESP_OK;
	}
	// }
	return ret;
}

esp_err_t GapAdapter::Init()
{
	esp_err_t ret = ESP_OK;

	ret = this->IsBleStackInit();
	if(ret)
	{
		return ret;
	}


	ret = esp_ble_gap_register_callback(gapEventHandlerCbk);
	 if (ret){
		 ESP_LOGE(GAP_NAME, "gap register error, error code = %x", ret);
		 return ret;
	 }
	 return ret;

}
esp_err_t GapAdapter::StartScan(uint16_t duration)
{

	esp_err_t scan_ret = esp_ble_gap_set_scan_params(&this->scanParams);
	if (scan_ret){
		ESP_LOGE(GAP_NAME, "set scan params error, error code = %x", scan_ret);
	}
	scan_ret = esp_ble_gap_start_scanning(duration);
	if (scan_ret){
		ESP_LOGE(GAP_NAME, "start scaning error, error code = %x", scan_ret);
	}
	return scan_ret;
}

esp_err_t GapAdapter::StartScan(uint16_t duration,esp_ble_scan_params_t *scanParams)
{
	memcpy(&this->scanParams,scanParams,sizeof(esp_ble_scan_params_t));
	esp_err_t scan_ret = esp_ble_gap_set_scan_params(&this->scanParams);
	if (scan_ret){
		ESP_LOGE(GAP_NAME, "set scan params error, error code = %x", scan_ret);
	}
	scan_ret = esp_ble_gap_start_scanning(duration);
	if (scan_ret){
		ESP_LOGE(GAP_NAME, "start scaning error, error code = %x", scan_ret);
	}
	return scan_ret;
}


esp_err_t GapAdapter::StopScan(void)
{
	esp_err_t scan_ret;
	scan_ret = esp_ble_gap_stop_scanning();
	if (scan_ret){
		ESP_LOGE(GAP_NAME, "start scaning error, error code = %x", scan_ret);
	}
	return scan_ret;
}

void GapAdapter::gap_event_handler(GapEventInfo info)
{
	esp_gap_ble_cb_event_t  event = info.event;
	esp_ble_gap_cb_param_t *param = info.param;
	ESP_LOGI(GAP_NAME, "GAP Event: %d", event);
	esp_err_t ret;

	switch(event)
	{
	case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
		ESP_LOGI(GAP_NAME, "Status %d", param->adv_data_cmpl.status);
		break;
	case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
		ESP_LOGI(GAP_NAME, "Status %d", param->scan_rsp_data_cmpl.status);
		break;
	case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
		ESP_LOGI(GAP_NAME, "Status %d", param->scan_rsp_data_cmpl.status);
		break;
	case ESP_GAP_BLE_SCAN_RESULT_EVT:
		break;
	case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
		ESP_LOGI(GAP_NAME, "Status %d", param->adv_data_raw_cmpl.status);
		if(param->adv_data_raw_cmpl.status == ESP_BT_STATUS_SUCCESS)
		{
			ret = esp_ble_gap_start_advertising(&this->adv_params);
			if (ret)
			{
				ESP_LOGE(GAP_NAME, "start advertising, error code = %x", ret);
			}
		}
		break;
	case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
		ESP_LOGI(GAP_NAME, "Status %d", param->adv_start_cmpl.status);
		break;
	case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_AUTH_CMPL_EVT:
		break;
	case ESP_GAP_BLE_KEY_EVT:
		break;
	case ESP_GAP_BLE_SEC_REQ_EVT:
		break;
	case ESP_GAP_BLE_PASSKEY_NOTIF_EVT:
		break;
	case ESP_GAP_BLE_PASSKEY_REQ_EVT:
		break;
	case ESP_GAP_BLE_OOB_REQ_EVT:
		break;
	case ESP_GAP_BLE_LOCAL_IR_EVT:
		break;
	case ESP_GAP_BLE_LOCAL_ER_EVT:
		break;
	case ESP_GAP_BLE_NC_REQ_EVT:
		break;
	case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
		ESP_LOGI(GAP_NAME, "Scan stopped");
		break;
	case ESP_GAP_BLE_SET_STATIC_RAND_ADDR_EVT:
		break;
	case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
		break;
	case ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT:
		break;
	case ESP_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT:
		break;

	default:
		break;

	}
	this->notify_observers(info);
}

int8_t GapAdapter::StartAdvertising(uint8_t *manufactureData, uint8_t len)
{
	int8_t bytesAdvertised = this->addAdvRawData(manufactureData,len,ESP_BLE_AD_MANUFACTURER_SPECIFIC_TYPE);
	esp_err_t err;
	if(bytesAdvertised >= 0)
	{
		err = esp_ble_gap_config_adv_data_raw(this->rawData, MAX_RAW_DATA_SIZE);
		if (err)
		{
			ESP_LOGE(GAP_NAME, "config raw adv data failed, error code = %x ", err);
			bytesAdvertised = -1;
			return bytesAdvertised;
		}
	}
	return bytesAdvertised;
}

int8_t GapAdapter::addAdvRawData(uint8_t *data, uint8_t len,esp_ble_adv_data_type dataType)
{
	int8_t bytesAdvertised = 0u;
	/* 1 byte adv_len 1 byte adv_type */
	if((this->rawDataSize + len + 2u) < MAX_RAW_DATA_SIZE)
	{
		bytesAdvertised = len;
	}
	else
	{
		bytesAdvertised = MAX_RAW_DATA_SIZE - (this->rawDataSize + len + 2u);
	}
	if(bytesAdvertised > 0)
	{
		this->rawData[this->rawDataSize] = len + 1;
		this->rawDataSize += 1u;
		this->rawData[this->rawDataSize] = dataType;
		this->rawDataSize += 1u;

		memcpy((void*)&this->rawData[this->rawDataSize],data,bytesAdvertised);
		this->rawDataSize += bytesAdvertised;
	}

	return bytesAdvertised;

}

int8_t GapAdapter::SetDeviceName(uint8_t *devName, uint8_t len)
{
	return this->addAdvRawData(devName,len,ESP_BLE_AD_TYPE_NAME_CMPL);
}
int8_t GapAdapter::StartAdvertising(void)
{
	int8_t ret = 0;
	ret = (int8_t)esp_ble_gap_start_advertising(&this->adv_params);
	if (ret)
	{
		ESP_LOGE(GAP_NAME, "start advertising, error code = %x", ret);
	}
	return ret;
}

GapAdapter& GapAdapter::getInstance(void)
{
	if(gapEventHandler == NULL)
	{
		gapEventHandler = &gap_callback;
	}
	return GapAdapter::gapAdapter;
}