/*
 * flashingService.c
 *
 *  Created on: Jul 18, 2019
 *      Author: ion
 */

//#include <BleAdapter.hpp>
//#include <BleRemoteDevice.h>
#include "../../SWLoad/FlashDevice/FlashDevice.h"

#include <esp_gatts_api.h>
#include <esp_log.h>
#include <string.h>

#include "../../SWLoad/FlashDevice/FlashDevice_cfg.h"
#define CHAR_VAL_LEN_MAX 0x40

using namespace SWLOAD;

FlashDevice::FlashDevice():BLE::BleRemoteDevice()
{
	this->flashProgress = 0xFF;
	this->flashStatus   = 0xFF;
	this->flashMagicNumber = 0xFFFF;
	this->flashVersion = 0xFFFF;
}

FlashDevice::~FlashDevice()
{

}



FlashDevice::FlashDevice(const FlashDevice& flashDev):BLE::BleRemoteDevice(flashDev)
{
	this->flashProgress = flashDev.flashProgress;
	this->flashStatus = flashDev.flashStatus;
	this->flashMagicNumber = flashDev.flashMagicNumber;
	this->flashVersion = flashDev.flashVersion;
}


//
//Characteristic  chr1;
//Descriptor      desc1;
//
//Descriptor     *currentDesc;
//Characteristic *currentChr;
//
//ServiceType        Flashing_ServiceInfo;
//
//
//void FlashProf_GattcCallback(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param)
//{
//	esp_err_t ret;
//	switch(event)
//	{
//
//	case ESP_GATTC_REG_EVT:
//		break;
//
//	case ESP_GATTC_UNREG_EVT:
//		break;
//	case ESP_GATTC_OPEN_EVT:
//		if(param->open.status == ESP_GATT_OK)
//		{
//
//			BLEDEVICE_UpdateDevice(param->open.conn_id,param->open.remote_bda,BLE_DEVICEINFO_FIELD_CONECTION_ID);
//			BLEDEVICE_UpdateDevice(param->open.status,param->open.remote_bda,BLE_DEVICEINFO_FIELD_CONECTION_STATUS);
//			BLEDEVICE_UpdateDevice(param->open.mtu,param->open.remote_bda,BLE_DEVICEINFO_FIELD_CONECTION_MTU);
//
//
//			ESP_LOGI(FLASHING_NAME, "Conn id: %d \n"
//							   	   	"Conn status %d\n"
//							   	    "Conn mtu %d",param->open.conn_id,
//									param->open.status,
//									param->open.mtu);
//
//			ret = esp_ble_gattc_send_mtu_req(gatts_if,param->open.conn_id);
//			  if(ret){
//			         ESP_LOGE(FLASHING_NAME, "%s gattc req mtu failed, error code = %x\n", __func__, ret);
//			         return;
//			     }
//
////			  ret = esp_ble_gattc_search_service(gatts_if, FlashProfConnectionInfo.conn_id,NULL);
////			  if(ret)
////			  {
////				 ESP_LOGE(FLASHING_NAME, "%s gattc failed to search , error code = %x\n", __func__, ret);
////				 return;
////			  }
//		}
//		else
//		{
//			ESP_LOGE(FLASHING_NAME, "Conn open failed with status %d",param->open.status);
//		}
//		break;
//	case ESP_GATTC_READ_CHAR_EVT:
//		ESP_LOGI(FLASHING_NAME,
//										    "Read chr status %d\n"
//							 	 	 	 	"Read chr conn_id: %d \n"
//											"Read chr hdl: %d \n"
//										    "Read chr value_len %d",
//											param->read.status,
//											param->read.conn_id,
//											param->read.handle,
//											param->read.value_len);
//		esp_log_buffer_hex(FLASHING_NAME, param->read.value, param->read.value_len);
//		break;
//	case ESP_GATTC_WRITE_CHAR_EVT:
//		break;
//	case ESP_GATTC_CLOSE_EVT:
//		break;
//	case ESP_GATTC_SEARCH_CMPL_EVT:
//		ESP_LOGI(FLASHING_NAME, "Search result id: %d \n"
//							    "Search conn id %d\n"
//							    "Searched service source %d",param->search_cmpl.status,
//								param->search_cmpl.conn_id,
//								param->search_cmpl.searched_service_source);
//
//		break;
//	case ESP_GATTC_SEARCH_RES_EVT:
////			ESP_LOGI(FLASHING_NAME,
////								    "Search conn id %d\n"
////					 	 	 	 	"Service start hdl: %d \n"
////									"Service end hdl: %d \n"
////								    "Service is primary %d\n"
////								    "Service is len %d\n"
////									"Service inst id %d\n"
////									"Service uuid %d",
////									param->search_res.conn_id,
////									param->search_res.start_handle,
////									param->search_res.end_handle,
////									param->search_res.is_primary,
////									param->search_res.srvc_id.uuid.len,
////									param->search_res.srvc_id.inst_id,
////									param->search_res.srvc_id.uuid.uuid.uuid16);
////			if(param->search_res.srvc_id.uuid.uuid.uuid16 == ble_RawData.uuid && param->search_res.srvc_id.uuid.len == ESP_UUID_LEN_16)
////			{
////				esp_gatt_status_t ret_gattc;
////				uint8_t i = 0;
////				esp_gattc_char_elem_t result[3];
////				uint16_t count = 3;
////				ret_gattc = esp_ble_gattc_get_all_char(gatts_if, param->search_res.conn_id,param->search_res.start_handle,param->search_res.end_handle,
////						result,&count,0);
////				if(ret_gattc == ESP_GATT_OK)
////				{
////					for(i = 0; i < count; i++)
////					{
////						ESP_LOGI(FLASHING_NAME,
////												"Chr handle %d\n"
////												"Chr props: %d \n"
////												"Chr uuid: %d \n"
////												"Chr len: %d",
////								result[i].char_handle,
////								result[i].properties,
////								result[i].uuid.uuid.uuid16,
////								result[i].uuid.len);
////
////						ret = esp_ble_gattc_read_char(gatts_if, param->search_res.conn_id, result[i].char_handle, 0);
////						if(ret)
////						  {
////							 ESP_LOGE(FLASHING_NAME, "%s gattc failed to read chr , error code = %x\n", __func__, ret);
////							 return;
////						  }
////
////						 esp_gattc_descr_elem_t result_desc[3];
////						 uint16_t count_desc = 3;
////						 ret_gattc = esp_ble_gattc_get_all_descr(gatts_if,param->search_res.conn_id,result[i].char_handle,result_desc,&count_desc,0);
////						 uint8_t j = 0;
////						 if(ret_gattc == ESP_GATT_OK)
////						 {
////							 for(j = 0; j < count_desc; j++)
////							{
////								ESP_LOGI(FLASHING_NAME,
////														"Desc handle %d\n"
////														"Desc uuid:  %d \n"
////														"Desc len:   %d",
////								result_desc[i].handle,
////								result_desc[i].uuid.uuid.uuid16,
////								result_desc[i].uuid.len);
////
////								ret = esp_ble_gattc_read_char_descr(gatts_if, param->search_res.conn_id, result_desc[i].handle, 0);
////								if(ret)
////								  {
////									 ESP_LOGE(FLASHING_NAME, "%s gattc failed to read descr , error code = %x\n", __func__, ret);
////									 return;
////								  }
////							}
////						 }
////						 else
////						 {
////							 ESP_LOGE(FLASHING_NAME, "Can't get descriptors with status %d",ret_gattc);
////						 }
////					}
////				}
////				else
////				{
////					ESP_LOGE(FLASHING_NAME, "Can't get characteristics with status %d",ret_gattc);
////				}
////			}
//		break;
//	case ESP_GATTC_READ_DESCR_EVT:
////		ESP_LOGI(FLASHING_NAME,
////												    "Read desc status %d\n"
////									 	 	 	 	"Read desc conn_id: %d \n"
////													"Read desc hdl: %d \n"
////												    "Read desc value_len %d",
////													param->read.status,
////													param->read.conn_id,
////													param->read.handle,
////													param->read.value_len);
////				esp_log_buffer_hex(FLASHING_NAME, param->read.value, param->read.value_len);
//		break;
//	case ESP_GATTC_WRITE_DESCR_EVT:
//		break;
//	case ESP_GATTC_NOTIFY_EVT:
//		break;
//	case ESP_GATTC_PREP_WRITE_EVT:
//		break;
//	case ESP_GATTC_EXEC_EVT:
//		break;
//	case ESP_GATTC_ACL_EVT:
//		break;
//	case ESP_GATTC_CANCEL_OPEN_EVT:
//		break;
//	case ESP_GATTC_SRVC_CHG_EVT:
//		break;
//	case ESP_GATTC_ENC_CMPL_CB_EVT:
//		break;
//	case ESP_GATTC_CFG_MTU_EVT:
//		if(param->cfg_mtu.status == ESP_OK)
//		{
//			ESP_LOGI(FLASHING_NAME, "Mtu cfg status: %d \n"
//								    "Conn id %d\n"
//								    "Conn mtu %d",param->cfg_mtu.status,
//									param->cfg_mtu.conn_id,
//									param->cfg_mtu.mtu);
//			BLEDEVICE_UpdateDeviceByConnId(param->cfg_mtu.mtu,param->cfg_mtu.conn_id,BLE_DEVICEINFO_FIELD_CONECTION_MTU);
//		}
//		break;
//	case ESP_GATTC_ADV_DATA_EVT:
//		break;
//	case ESP_GATTC_MULT_ADV_ENB_EVT:
//		break;
//	case ESP_GATTC_MULT_ADV_UPD_EVT:
//		break;
//	case ESP_GATTC_MULT_ADV_DATA_EVT:
//		break;
//	case ESP_GATTC_MULT_ADV_DIS_EVT:
//		break;
//	case ESP_GATTC_CONGEST_EVT:
//		break;
//	case ESP_GATTC_BTH_SCAN_ENB_EVT:
//		break;
//	case ESP_GATTC_BTH_SCAN_CFG_EVT:
//		break;
//	case ESP_GATTC_BTH_SCAN_RD_EVT:
//		break;
//	case ESP_GATTC_BTH_SCAN_THR_EVT:
//		break;
//	case ESP_GATTC_BTH_SCAN_PARAM_EVT:
//		break;
//	case ESP_GATTC_BTH_SCAN_DIS_EVT:
//		break;
//	case ESP_GATTC_SCAN_FLT_CFG_EVT:
//		break;
//	case ESP_GATTC_SCAN_FLT_PARAM_EVT:
//		break;
//	case ESP_GATTC_SCAN_FLT_STATUS_EVT:
//		break;
//	case ESP_GATTC_ADV_VSC_EVT:
//		break;
//	case ESP_GATTC_REG_FOR_NOTIFY_EVT:
//		break;
//	case ESP_GATTC_UNREG_FOR_NOTIFY_EVT:
//		break;
//	case ESP_GATTC_CONNECT_EVT:
//		ESP_LOGI(FLASHING_NAME, "Conn id: %d \n"
//						   	   	"Conn params interval %d\n"
//						   	    "Conn params latency %d\n"
//								"Conn params timeout %d",param->connect.conn_id,
//								param->connect.conn_params.interval,
//								param->connect.conn_params.latency,
//								param->connect.conn_params.timeout);
//		break;
//	case ESP_GATTC_DISCONNECT_EVT:
//		break;
//	case ESP_GATTC_READ_MULTIPLE_EVT:
//		break;
//	case ESP_GATTC_QUEUE_FULL_EVT:
//		break;
//	case ESP_GATTC_SET_ASSOC_EVT:
//		break;
//	case ESP_GATTC_GET_ADDR_LIST_EVT:
//		break;
//	case ESP_GATTC_DIS_SRVC_CMPL_EVT:
//		ESP_LOGI(FLASHING_NAME, "Conn id: %d \n"
//								"Conn disc status %d"
//							    ,param->dis_srvc_cmpl.conn_id,
//							     param->dis_srvc_cmpl.status);
//		break;
//
//	}
//}
//
//
//
//
//
//
//void FlashProfile_Init(void)
//{
//	esp_err_t ret;
//	BLE_RegisterProfile(FLASHING_PROFILE,FlashProf_GattcCallback);
//
//}
//
//
//
