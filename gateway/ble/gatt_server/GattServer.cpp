/*
 * GattServer.cpp
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#include "GattServer.h"
#include <esp_log.h>

using namespace BLE;


uint8_t GattServer::noObsReg = 0u;
Observer* GattServer::obsReg[5] = {};


GattServer::GattServer(BleAdapter *ble)
{
	this->bleAdapter = ble;
}

GattServer::~GattServer()
{

}

esp_err_t GattServer::Init(void)
{
	esp_err_t ret = ESP_OK;
	if(this->bleAdapter != NULL)
	{
		if(this->bleAdapter->IsBleStackActive() == false)
		{
			ret = this->bleAdapter->Init();
		}
	}
	if(ret)
	{
		return ret;
	}

	ret = esp_ble_gatts_register_callback(GattServer::gattsCallback);
	if(ret){
		 ESP_LOGE(GATT_SERVER_NAME, "%s gattc register failed, error code = %x\n", __func__, ret);
		 return ret;
	}
	return ret;
}


void GattServer::RegisterApp(uint16_t appId)
{
	esp_err_t ret;
	ret = esp_ble_gatts_app_register(appId);
	if(ret)
	{
		ESP_LOGE(GATT_SERVER_NAME, "%s gattc register app failed, error code = %x\n", __func__, ret);
	}
}
esp_err_t GattServer::RegisterService(esp_gatt_if_t gatts_if,GattService *service)
{
	esp_err_t ret;
	ret = esp_ble_gatts_create_service(gatts_if,&service->serviceInfo, service->numHandles);
	if(ret != ESP_OK)
	{
		ESP_LOGI(GATT_SERVER_NAME,"Service can't be created. Failed with error: %d", ret);
	}
	return ret;
}

void GattServer::AttachObserver(Observer *obs)
{
	Observer::AttachObs(GattServer::obsReg, GattServer::noObsReg, MAX_OBSERVERS, obs);
}
void GattServer::DeleteObserver(Observer *obs)
{
	Observer::DeleteObs(GattServer::obsReg, GattServer::noObsReg, obs);
}
void GattServer::DeleteObserver(uint8_t idx)
{
	Observer::DeleteObs(GattServer::obsReg, GattServer::noObsReg, idx);
}

void GattServer::gattsCallback(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
	uint8_t i = 0;
	ESP_LOGI(GATT_SERVER_NAME, "GATTS Event: %d", event);
	ESP_LOGI(GATT_SERVER_NAME, "GATTS interface: %d", gatts_if);
	if(event == ESP_GATTS_REG_EVT)
	{
		if(param->reg.status == ESP_GATT_OK)
		{
			ESP_LOGI(GATT_SERVER_NAME, "Profile %d registered with status %d",param->reg.app_id ,param->reg.status);
			for(i = 0; i < GattServer::noObsReg; i++)
			{
				if(GattServer::obsReg[i]->appId == param->reg.app_id)
				{
					GattServer::obsReg[i]->gattsif = gatts_if;
					GattServer::obsReg[i]->updateGatts(event,gatts_if,param);
					break;
				}
			}
		}
	}
	else
	{
		for(i = 0; i < GattServer::noObsReg; i++)
		{
			if(GattServer::obsReg[i]->gattsif == gatts_if)
			{
				GattServer::obsReg[i]->updateGatts(event,gatts_if,param);
				break;
			}
		}
	}
}
esp_err_t GattServer::OpenConnection(esp_gatt_if_t gatts_if, esp_bd_addr_t remote_bda, bool is_direct)
{
	esp_err_t ret = esp_ble_gatts_open(gatts_if, remote_bda, is_direct);
	if(ret)
	{
		ESP_LOGE(GATT_SERVER_NAME, "%s gattc register app failed, error code = %x\n", __func__, ret);
	}
	return ret;
}






