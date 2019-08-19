/*
 * GattClient.cpp
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#include "GattClient.h"
#include <esp_log.h>


using namespace BLE;


uint8_t GattClient::noObsReg = 0u;
Observer* GattClient::obsReg[5] = {};


GattClient::GattClient(BleAdapter *ble)
{
	this->bleAdapter = ble;
}

GattClient::~GattClient()
{

}

esp_err_t GattClient::Init(void) // @suppress("Member declaration not found")
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

	ret = esp_ble_gattc_register_callback(GattClient::gattcCallback);
	if(ret){
		 ESP_LOGE(GATT_CLIENT_NAME, "%s gattc register failed, error code = %x\n", __func__, ret);
		 return ret;
	}

	return ret;
}


void GattClient::RegisterApp(uint16_t appId)
{
	esp_err_t ret;
	ret = esp_ble_gattc_app_register(appId);
	if(ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s gattc register app failed, error code = %x\n", __func__, ret);
	}
}


void GattClient::AttachObserver(Observer *obs)
{
	Observer::AttachObs(GattClient::obsReg, GattClient::noObsReg, MAX_OBSERVERS, obs);
}
void GattClient::DeleteObserver(Observer *obs)
{
	Observer::DeleteObs(GattClient::obsReg, GattClient::noObsReg, obs);
}
void GattClient::DeleteObserver(uint8_t idx)
{
	Observer::DeleteObs(GattClient::obsReg, GattClient::noObsReg, idx);
}

void GattClient::gattcCallback(esp_gattc_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gattc_cb_param_t *param)
{
	uint8_t i = 0;
	ESP_LOGI(GATT_CLIENT_NAME, "GATTC Event: %d", event);
	ESP_LOGI(GATT_CLIENT_NAME, "GATTC interface: %d", gatts_if);
	if(event == ESP_GATTC_REG_EVT)
	{
		if(param->reg.status == ESP_GATT_OK)
		{
			ESP_LOGI(GATT_CLIENT_NAME, "Profile %d registered with status %d",param->reg.app_id ,param->reg.status);
			for(i = 0; i < GattClient::noObsReg; i++)
			{
				if(GattClient::obsReg[i]->appId == param->reg.app_id)
				{
					GattClient::obsReg[i]->gattcif = gatts_if;
					GattClient::obsReg[i]->updateGattc(event,gatts_if,param);
					break;
				}
			}
		}
	}
	else
	{
		for(i = 0; i < GattClient::noObsReg; i++)
		{
			if(GattClient::obsReg[i]->gattcif == gatts_if)
			{
				GattClient::obsReg[i]->updateGattc(event,gatts_if,param);
				break;
			}
		}
	}
}
esp_err_t GattClient::SearchService(esp_gatt_if_t gattc_if, uint16_t conn_id)
{
	esp_err_t ret = esp_ble_gattc_search_service(gattc_if, conn_id, NULL);
	if(ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s Search service failed, error code = %x\n", __func__, ret);
	}
	return ret;
}



esp_err_t GattClient::OpenConnection(esp_gatt_if_t gattc_if, esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type, bool is_direct)
{
	esp_err_t ret = esp_ble_gattc_open( gattc_if,  remote_bda,  remote_addr_type,  is_direct);
	return ret;
}
