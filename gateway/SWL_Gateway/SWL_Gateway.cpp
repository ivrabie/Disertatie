/*
 * SWL_Gateway.c
 *
 *  Created on: Jul 20, 2019
 *      Author: ion
 */

//#include <GapAdapter.h>
//#include <GapAdapter_cfg.h>
//#include <BleRemoteDevice.h>
//#include <BleRemoteDevice_types.h>
#include "SWL_Gateway_cfg.h"
#include "SWL_Gateway.h"
#include <esp_err.h>
#include <esp_log.h>
#include <string.h>
#include "../FlashDevice/FlashDevice.h"

using namespace SWL_GATEWAY;

#define MANUFACTURE_DATA_SIZE 4u

SwlGateway::SwlGateway(uint16_t appId,GapAdapter *gapAdapter,GattClient *gattc,NVSAdapter *nvsAdapter,WifiAdapter *wifiAdapter):Observer(appId)
{
	this->new_version = 0xFFFF;
	this->gapAdapter = gapAdapter;
	this->gattc = gattc;
	this->wifiAdapter = wifiAdapter;
	this->nvsAdapter = nvsAdapter;
}

SwlGateway::~SwlGateway()
{

}


void SwlGateway:: updateGap(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{


		switch(event)
		{
		case ESP_GAP_BLE_SCAN_RESULT_EVT:
			if(param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT)
			{
				ESP_LOGI(SWLGW_NAME, "Try create conenctions");
				this->triggerConnetion();
			}
			else
			{
				this->processScanData(param);
			}

			break;
		case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:

			break;
		default:
			break;
		}

}



void SwlGateway::Init(void)
{
	uint8_t manufacture_data[MANUFACTURE_DATA_SIZE];
	uint8_t len;
	this->nvsAdapter->Init();
	this->wifiAdapter->Init();
	this->wifiAdapter->Connect();
	this->gapAdapter->Init();
	this->gattc->Init();
	this->gattc->RegisterApp(this->appId); // @suppress("Invalid arguments")
//	this->buildManufactureDataBuffer(manufacture_data,len);
//	this->gapAdapter->SetDeviceName((uint8_t*)SWLGW_NAME,sizeof(SWLGW_NAME));
//	this->gapAdapter->StartAdvertising(manufacture_data,len);

	this->gapAdapter->StartScan(5u);

}

void SwlGateway::processRawData(uint8_t *data, uint8_t len, FlashDevice *device)
{
		uint8_t idx = 0;
		uint8_t typeLen = 0;
		while(idx < len)
		{
			typeLen = data[idx];
			idx += 1u;
			ESP_LOGI(SWLGW_NAME, "len: %d type %d",typeLen,data[idx]);
			switch(data[idx])
			{
				case ESP_BLE_AD_TYPE_NAME_CMPL:
					if((typeLen - 1u) == BLE_DEVICEINFO_EXPECTED_LEN_NAME)
					{
						memcpy(device->deviceName, &data[idx + 1], BLE_DEVICEINFO_EXPECTED_LEN_NAME);
					}
					break;
				case ESP_BLE_AD_MANUFACTURER_SPECIFIC_TYPE:
					// Application specific
					if((typeLen - 1u) == (sizeof(uint16_t)))
					{
						device->flashMagicNumber = *((uint16_t*)&data[idx + 1]);
					}
					break;
			}
			idx += typeLen;
		}
}


void SwlGateway::updateGattc(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param)
{
		esp_err_t ret;
		switch(event)
		{

		case ESP_GATTC_REG_EVT:
			break;

		case ESP_GATTC_UNREG_EVT:
			break;
		case ESP_GATTC_OPEN_EVT:
			if(param->open.status == ESP_GATT_OK)
			{
				this->flashDeviceManager.UpdateDevice(param->open.remote_bda,param->open.conn_id,DEVICE_UPDATEFIELD_CONECTION_ID);
				this->flashDeviceManager.UpdateDevice(param->open.remote_bda,param->open.status,DEVICE_UPDATEFIELD_CONECTION_STATUS);
				this->flashDeviceManager.UpdateDevice(param->open.remote_bda,param->open.mtu,DEVICE_UPDATEFIELD_CONECTION_MTU);


				ESP_LOGI(SWLGW_NAME, "Conn id: %d \n"
								   	   	"Conn status %d\n"
								   	    "Conn mtu %d",param->open.conn_id,
										param->open.status,
										param->open.mtu);

				ret = esp_ble_gattc_send_mtu_req(gatts_if,param->open.conn_id);
				  if(ret){
				         ESP_LOGE(SWLGW_NAME, "%s gattc req mtu failed, error code = %x\n", __func__, ret);
				         return;
				     }

	//			  ret = esp_ble_gattc_search_service(gatts_if, FlashProfConnectionInfo.conn_id,NULL);
	//			  if(ret)
	//			  {
	//				 ESP_LOGE(SWLGW_NAME, "%s gattc failed to search , error code = %x\n", __func__, ret);
	//				 return;
	//			  }
			}
			else
			{
				ESP_LOGE(SWLGW_NAME, "Conn open failed with status %d",param->open.status);
			}
			break;
		case ESP_GATTC_READ_CHAR_EVT:
			ESP_LOGI(SWLGW_NAME,
									"Read chr status %d\n"
									"Read chr conn_id: %d \n"
									"Read chr hdl: %d \n"
									"Read chr value_len %d",
									param->read.status,
									param->read.conn_id,
									param->read.handle,
									param->read.value_len);
			esp_log_buffer_hex(SWLGW_NAME, param->read.value, param->read.value_len);
			this->flashDeviceManager.NotifyEvtChr(param->read.conn_id,param->read.handle,param->read.value,param->read.value_len,FLASHING_READCHR_TYPE);
			break;
		case ESP_GATTC_WRITE_CHAR_EVT:
			break;
		case ESP_GATTC_CLOSE_EVT:
			break;
		case ESP_GATTC_SEARCH_CMPL_EVT:
			ESP_LOGI(SWLGW_NAME, "Search result id: %d \n"
								    "Search conn id %d\n"
								    "Searched service source %d",param->search_cmpl.status,
									param->search_cmpl.conn_id,
									param->search_cmpl.searched_service_source);

			break;
		case ESP_GATTC_SEARCH_RES_EVT:
	//			ESP_LOGI(SWLGW_NAME,
	//								    "Search conn id %d\n"
	//					 	 	 	 	"Service start hdl: %d \n"
	//									"Service end hdl: %d \n"
	//								    "Service is primary %d\n"
	//								    "Service is len %d\n"
	//									"Service inst id %d\n"
	//									"Service uuid %d",
	//									param->search_res.conn_id,
	//									param->search_res.start_handle,
	//									param->search_res.end_handle,
	//									param->search_res.is_primary,
	//									param->search_res.srvc_id.uuid.len,
	//									param->search_res.srvc_id.inst_id,
	//									param->search_res.srvc_id.uuid.uuid.uuid16);

			break;
		case ESP_GATTC_READ_DESCR_EVT:
			ESP_LOGI(SWLGW_NAME,
													    "Read desc status %d\n"
										 	 	 	 	"Read desc conn_id: %d \n"
														"Read desc hdl: %d \n"
													    "Read desc value_len %d",
														param->read.status,
														param->read.conn_id,
														param->read.handle,
														param->read.value_len);
			esp_log_buffer_hex(SWLGW_NAME, param->read.value, param->read.value_len);
			break;
		case ESP_GATTC_WRITE_DESCR_EVT:
			break;
		case ESP_GATTC_NOTIFY_EVT:
			ESP_LOGI(SWLGW_NAME,
													    "Read notify status %d\n"
										 	 	 	 	"Read notify conn_id: %d \n"
														"Read notify hdl: %d \n"
													    "Read notify value_len %d",
														param->notify.is_notify,
														param->notify.conn_id,
														param->notify.handle,
														param->notify.value_len);
			this->flashDeviceManager.NotifyEvtChr(param->notify.conn_id,param->notify.handle,param->notify.value,param->notify.value_len,FLASHING_NOTIFICATION_TYPE);
			break;
		case ESP_GATTC_PREP_WRITE_EVT:
			break;
		case ESP_GATTC_EXEC_EVT:
			break;
		case ESP_GATTC_ACL_EVT:
			break;
		case ESP_GATTC_CANCEL_OPEN_EVT:
			break;
		case ESP_GATTC_SRVC_CHG_EVT:
			break;
		case ESP_GATTC_ENC_CMPL_CB_EVT:
			break;
		case ESP_GATTC_CFG_MTU_EVT:
			if(param->cfg_mtu.status == ESP_OK)
			{
				ESP_LOGI(SWLGW_NAME, "Mtu cfg status: %d \n"
									    "Conn id %d\n"
									    "Conn mtu %d",param->cfg_mtu.status,
										param->cfg_mtu.conn_id,
										param->cfg_mtu.mtu);
				ESP_LOGI(SWLGW_NAME, "Search service");
//				this->flashDeviceManager.RefreshServiceDiscover(param->connect.conn_id);

				this->flashDeviceManager.UpdateDevice(param->cfg_mtu.conn_id,param->cfg_mtu.mtu,DEVICE_UPDATEFIELD_CONECTION_MTU);
			}
			break;
		case ESP_GATTC_ADV_DATA_EVT:
			break;
		case ESP_GATTC_MULT_ADV_ENB_EVT:
			break;
		case ESP_GATTC_MULT_ADV_UPD_EVT:
			break;
		case ESP_GATTC_MULT_ADV_DATA_EVT:
			break;
		case ESP_GATTC_MULT_ADV_DIS_EVT:
			break;
		case ESP_GATTC_CONGEST_EVT:
			break;
		case ESP_GATTC_BTH_SCAN_ENB_EVT:
			break;
		case ESP_GATTC_BTH_SCAN_CFG_EVT:
			break;
		case ESP_GATTC_BTH_SCAN_RD_EVT:
			break;
		case ESP_GATTC_BTH_SCAN_THR_EVT:
			break;
		case ESP_GATTC_BTH_SCAN_PARAM_EVT:
			break;
		case ESP_GATTC_BTH_SCAN_DIS_EVT:
			break;
		case ESP_GATTC_SCAN_FLT_CFG_EVT:
			break;
		case ESP_GATTC_SCAN_FLT_PARAM_EVT:
			break;
		case ESP_GATTC_SCAN_FLT_STATUS_EVT:
			break;
		case ESP_GATTC_ADV_VSC_EVT:
			break;
		case ESP_GATTC_REG_FOR_NOTIFY_EVT:
			break;
		case ESP_GATTC_UNREG_FOR_NOTIFY_EVT:
			break;
		case ESP_GATTC_CONNECT_EVT:
			ESP_LOGI(SWLGW_NAME, "Conn id: %d \n"
							   	   	"Conn params interval %d\n"
							   	    "Conn params latency %d\n"
									"Conn params timeout %d",param->connect.conn_id,
									param->connect.conn_params.interval,
									param->connect.conn_params.latency,
									param->connect.conn_params.timeout);



			break;
		case ESP_GATTC_DISCONNECT_EVT:
			break;
		case ESP_GATTC_READ_MULTIPLE_EVT:
			break;
		case ESP_GATTC_QUEUE_FULL_EVT:
			break;
		case ESP_GATTC_SET_ASSOC_EVT:
			break;
		case ESP_GATTC_GET_ADDR_LIST_EVT:
			break;
		case ESP_GATTC_DIS_SRVC_CMPL_EVT:
			ESP_LOGI(SWLGW_NAME, "Conn id: %d \n"
									"Conn disc status %d"
								    ,param->dis_srvc_cmpl.conn_id,
								     param->dis_srvc_cmpl.status);
			this->flashDeviceManager.NotifyDiscoverComplete(this->gattcif,param->dis_srvc_cmpl.conn_id);
			this->flashDeviceManager.RegisterForNotifyBlockReq(this->gattcif,param->dis_srvc_cmpl.conn_id);
			this->flashDeviceManager.RequestVersion(gatts_if,param->dis_srvc_cmpl.conn_id);
			break;

		}
}

void SwlGateway::buildManufactureDataBuffer(uint8_t *data, uint8_t &len)
{
	data[0] = SWLGW_MAGIC_NUMBER;
	data[1] = SWLGW_MAGIC_NUMBER>>8u;
	memcpy((void*)&data[2u], (void*)&this->new_version,sizeof(uint16_t));
	len = 4;
}
void SwlGateway::updateGatts(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{

}


void SwlGateway::processScanData(esp_ble_gap_cb_param_t *param)
{
	uint8_t i = 0;
	ESP_LOGI(GAP_NAME, "Scan event: %d \n"
			"Device type %d\n"
			"Ble event type %d\n"
			"Ble addr type %d\n"
			"Rssi: %d\n"
			"Adv len: %d\n"
			"Resp len: %d\n"
			"Packet discarded: %d\n"
			"Scan result: %d\n,"
			"Flags: %d\n", param->scan_rst.search_evt,param->scan_rst.dev_type,
			param->scan_rst.ble_evt_type,param->scan_rst.ble_addr_type,param->scan_rst.rssi,
			param->scan_rst.adv_data_len,param->scan_rst.scan_rsp_len,param->scan_rst.num_dis,
			param->scan_rst.num_resps, param->scan_rst.flag);
	esp_log_buffer_hex(GAP_NAME, param->scan_rst.bda, 6);



		if (param->scan_rst.adv_data_len > 0) {
			ESP_LOGI(GAP_NAME, "adv data:");
			esp_log_buffer_hex(GAP_NAME, &param->scan_rst.ble_adv[0], param->scan_rst.adv_data_len);
			FlashDevice device;
			this->processRawData(&param->scan_rst.ble_adv[0],param->scan_rst.adv_data_len,&device);

			if(SWLGW_MAGIC_NUMBER == device.flashMagicNumber)
			{
				device.ble_addr_type = param->scan_rst.ble_addr_type;
				memcpy((void *)device.bda,(void *)param->scan_rst.bda,ESP_BD_ADDR_LEN);
				this->flashDeviceManager.RegisterDevice(&device);
			}
		}
}

void SwlGateway::triggerConnetion(void)
{
	uint8_t i = 0;
	ESP_LOGI(SWLGW_NAME, "Registred devices %d",flashDeviceManager.numberOfDevices);
	for(i = 0; i < flashDeviceManager.numberOfDevices; i++)
	{
		this->gattc->OpenConnection(this->gattcif,flashDeviceManager.listOfDevice[i].bda,flashDeviceManager.listOfDevice[i].ble_addr_type,true);
	}
}

