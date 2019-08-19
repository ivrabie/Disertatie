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
#include <esp_err.h>
#include <esp_log.h>
#include <string.h>
#include "../FlashDevice/FlashDevice.h"
#include "SWLoad.h"
#include "SWLoad_cfg.h"

using namespace SWLOAD;

#define MANUFACTURE_DATA_SIZE 4u

SwLoad::SwLoad(uint16_t appId,GapAdapter *gapAdapter,NVSAdapter *nvsAdapter,GattServer *gattServer):Observer(appId)
{
	this->currentVersion = 0x0;
	this->gapAdapter = gapAdapter;
	this->nvsAdapter = nvsAdapter;
	this->gattServer = gattServer;
	this->state = SwLoadAppState_IDLE;
	ESP_LOGI(SWLOAD_NAME, "Created");
}

SwLoad::~SwLoad()
{

}


void SwLoad::updateGap(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{

}



void SwLoad::Init(void)
{
	this->nvsAdapter->Init();
	this->gapAdapter->Init();
	this->gattServer->Init();
	this->gattServer->RegisterApp(this->appId);

}


void SwLoad::updateGattc(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param)
{

}




void SwLoad::updateGatts(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
//		ESP_LOGI(SWLOAD_NAME, "GATT Event: %d", event);
//		ESP_LOGI(SWLOAD_NAME, "GATT interface: %d", gatts_if);
		uint8_t data[2u];
		switch(event)
		{
			case ESP_GATTS_REG_EVT:
				this->gattServer->RegisterService(gatts_if,&this->flashService);
				break;
			case ESP_GATTS_READ_EVT:
				this->flashService.NotifyReadChr(gatts_if,param->read.conn_id,param->read.trans_id,param->read.handle,param->read.offset,param->read.is_long,param->read.need_rsp);

				break;
			case ESP_GATTS_WRITE_EVT:
				break;
			case ESP_GATTS_EXEC_WRITE_EVT:
				break;
			case ESP_GATTS_MTU_EVT:
				break;
			case ESP_GATTS_CONF_EVT:
		        ESP_LOGI(SWLOAD_NAME, "ESP_GATTS_CONF_EVT, status %d, att_handle %d, len %d, conn id %d",
		                 param->conf.status, param->conf.handle,param->conf.len, param->conf.conn_id);
		        esp_log_buffer_hex(SWLOAD_NAME,param->conf.value,param->conf.len);
				break;
			case ESP_GATTS_UNREG_EVT:
				break;
			case ESP_GATTS_CREATE_EVT:
				if(param->create.status == ESP_GATT_OK)
				{
					if(this->flashService.NotifyServiceRegistred(&param->create.service_id,param->create.service_handle) == true)
					{
						this->flashService.CreateService();
					}
				}
				break;
			case ESP_GATTS_ADD_INCL_SRVC_EVT:
				break;
			case ESP_GATTS_ADD_CHAR_EVT:
				if(param->add_char.status == ESP_GATT_OK)
				{
					this->flashService.NotifyAttrRegistred(param->add_char.attr_handle,param->add_char.service_handle,&param->add_char.char_uuid);
					if(this->flashService.IsServiceInit() == true)
					{
						this->flashService.PrintService();
						this->flashService.StartService();
					}
				}
				break;
			case ESP_GATTS_ADD_CHAR_DESCR_EVT:
				if(param->add_char_descr.status == ESP_GATT_OK)
				{
					this->flashService.NotifyAttrRegistred(param->add_char_descr.attr_handle,param->add_char_descr.service_handle,&param->add_char_descr.descr_uuid);
					if(this->flashService.IsServiceInit() == true)
					{
						this->flashService.PrintService();
						this->flashService.StartService();
					}
				}
				break;
			case ESP_GATTS_DELETE_EVT:
				break;
			case ESP_GATTS_START_EVT:
		        ESP_LOGI(SWLOAD_NAME, "SERVICE_START_EVT, status %d, service_handle %d",
		                 param->start.status, param->start.service_handle);
		        data[0] = (uint8_t)SWLOAD_MAGIC_NUMBER;
		        data[1] = (uint8_t)(SWLOAD_MAGIC_NUMBER >> 8u);
		    	this->gapAdapter->StartAdvertising(data,2u);
				break;
			case ESP_GATTS_STOP_EVT:
				break;
			case ESP_GATTS_CONNECT_EVT:
				this->device.conn_id = param->connect.conn_id;
				memcpy(this->device.bda, param->connect.remote_bda, ESP_BD_ADDR_LEN);
				break;
			case ESP_GATTS_DISCONNECT_EVT:
				break;
			case ESP_GATTS_OPEN_EVT:
				this->device.status = param->open.status;
				break;
			case ESP_GATTS_CANCEL_OPEN_EVT:
				break;
			case ESP_GATTS_CLOSE_EVT:
				break;
			case ESP_GATTS_LISTEN_EVT:
				break;
			case ESP_GATTS_CONGEST_EVT:
				break;
			case ESP_GATTS_RESPONSE_EVT:
				this->flashService.RequestBlock(gatts_if,this->device.conn_id);
				break;
			case ESP_GATTS_CREAT_ATTR_TAB_EVT:
				break;
			case ESP_GATTS_SET_ATTR_VAL_EVT:
				break;
			case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
				break;
		}
}


