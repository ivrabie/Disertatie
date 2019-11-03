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

SwLoad::SwLoad(uint16_t appId):
appId(appId),
flashDevice(0)
{
	this->currentVersion = 0x0;
	this->state = SwLoadAppState_IDLE;
	this->gapAdapter.add_observer(*this);
	this->gattServer.add_observer(*this);
	ESP_LOGI(SWLOAD_NAME, "Created");
}

SwLoad::~SwLoad()
{

}


void SwLoad::notification(GapEventInfo info)
{
	esp_gap_ble_cb_event_t event = info.event;
	esp_ble_gap_cb_param_t *param = info.param;
}



void SwLoad::Init(void)
{
	this->nvsAdapter.Init();
	ESP_LOGI(SWLOAD_NAME, "Nvs Init");
	this->gapAdapter.Init();
	ESP_LOGI(SWLOAD_NAME, "Gap adapter init");
	this->gattServer.Init();
	ESP_LOGI(SWLOAD_NAME, "Gatts adapter init");
	this->flashDevice.Init();
	this->gattServer.RegisterApp(this->appId);
	ESP_LOGI(SWLOAD_NAME, "Register app");


}






void SwLoad::notification(GattsEventInfo info)
{
	esp_gatts_cb_event_t event = info.event;
	esp_gatt_if_t gatts_if = info.gatts_if; 
	esp_ble_gatts_cb_param_t *param = info.param;

	if(event == ESP_GATTS_REG_EVT)
	{
		if(param->reg.status == ESP_GATT_OK && param->reg.app_id == this->appId)
		{
			ESP_LOGI(SWLOAD_NAME, "Profile %d registered with status %d",param->reg.app_id ,param->reg.status);
			this->gatts_if = gatts_if;
			this->flashDevice.gatts_if = gatts_if;
			this->flashDevice.RegisterService();
		}
	}
	else if(this->gatts_if == gatts_if)
	{
		ConnectEvt evt;
		switch(event)
		{
			case ESP_GATTS_REG_EVT:	
				break;
			case ESP_GATTS_READ_EVT:
				this->flashDevice.NotifyReadChr(param->read.trans_id,param->read.handle,param->read.offset,param->read.is_long,param->read.need_rsp);
				break;
			case ESP_GATTS_WRITE_EVT:
				this->flashDevice.NotifyWriteChr(param->write.trans_id,param->write.handle,param->write.offset,param->write.need_rsp,param->write.len,param->write.value);
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
					if(this->flashDevice.NotifyServiceRegistred(&param->create.service_id,param->create.service_handle) == true)
					{
						this->flashDevice.CreateService();
					}
				}
				break;
			case ESP_GATTS_ADD_INCL_SRVC_EVT:
				break;
			case ESP_GATTS_ADD_CHAR_EVT:
				if(param->add_char.status == ESP_GATT_OK)
				{
					this->flashDevice.NotifyAttrRegistred(param->add_char.attr_handle,param->add_char.service_handle,&param->add_char.char_uuid);
					if(this->flashDevice.flashService.IsServiceInit() == true)
					{
						this->flashDevice.PrintService();
						this->flashDevice.StartService();
					}
				}
				break;
			case ESP_GATTS_ADD_CHAR_DESCR_EVT:
				if(param->add_char_descr.status == ESP_GATT_OK)
				{
					this->flashDevice.NotifyAttrRegistred(param->add_char_descr.attr_handle,param->add_char_descr.service_handle,&param->add_char_descr.descr_uuid);
					if(this->flashDevice.flashService.IsServiceInit() == true)
					{
						this->flashDevice.PrintService();
						this->flashDevice.StartService();
					}
				}
				break;
			case ESP_GATTS_DELETE_EVT:
				break;
			case ESP_GATTS_START_EVT:
				ESP_LOGI(SWLOAD_NAME, "SERVICE_START_EVT, status %d, service_handle %d",
				param->start.status, param->start.service_handle);
				this->flashDevice.StartAdvertising();
				break;
			case ESP_GATTS_STOP_EVT:
				break;
			case ESP_GATTS_CONNECT_EVT:
				ESP_LOGI(SWLOAD_NAME, "ESP_GATTS_CONNECT_EVT, connection id %d",param->connect.conn_id);
				evt.setParams(param->connect.conn_id,param->connect.remote_bda);
				this->flashDevice.receive(evt);
				break;
			case ESP_GATTS_DISCONNECT_EVT:
				break;
			case ESP_GATTS_OPEN_EVT:
				this->flashDevice.UpdateConectionStatus(param->open.status);
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
			ESP_LOGI(SWLOAD_NAME, "ESP_GATTS_RESPONSE_EVT, status %d, handle %d",
				param->rsp.status, param->rsp.handle);	
				break;
			case ESP_GATTS_CREAT_ATTR_TAB_EVT:
				break;
			case ESP_GATTS_SET_ATTR_VAL_EVT:
				break;
			case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
				break;
		}
	}
}


