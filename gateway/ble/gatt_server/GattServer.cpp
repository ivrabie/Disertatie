/*
 * GattServer.cpp
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#include "GattServer.h"
#include <esp_log.h>

using namespace BLE;


GattServer GattServer::gattServer;

etl::function_imp<GattServer, GattsEventInfo, GattServer::gattServer, &GattServer::gattsCallback> gatts_callback;
etl::ifunction<GattsEventInfo>* gattsEventHandler = NULL;
extern "C"
{
	void gattsEventHandlerCbk(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
	{
		GattsEventInfo info;
		info.event = event;
		info.param = param;
		info.gatts_if = gatts_if;
		(*gattsEventHandler)(info);
	}
}



GattServer::GattServer()
{
}

GattServer::~GattServer()
{

}

esp_err_t GattServer::Init(void)
{
	esp_err_t ret = ESP_OK;

	if(this->bleAdapter.IsBleStackActive() == false)
	{
		ret = this->bleAdapter.Init();
	}
	
	if(ret)
	{
		return ret;
	}

	ret = esp_ble_gatts_register_callback(gattsEventHandlerCbk);
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



void GattServer::gattsCallback(GattsEventInfo info)
{
	uint8_t i = 0;
	ESP_LOGI(GATT_SERVER_NAME, "GATTS Event: %d", info.event);
	ESP_LOGI(GATT_SERVER_NAME, "GATTS interface: %d", info.gatts_if);
	this->notify_observers(info);
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


esp_err_t GattServer::SendResponse(esp_gatt_if_t gatts_if, uint16_t conn_id, uint32_t trans_id,
				esp_gatt_status_t status, esp_gatt_rsp_t *rsp)
{
	esp_err_t ret = esp_ble_gatts_send_response(gatts_if,conn_id,trans_id, status, rsp);
	if(ret != ESP_OK)
	{
		ESP_LOGI(GATT_SERVICE_NAME,"Send data failed, Failed with error: %d", ret);
	}
	return ret;
}
esp_err_t GattServer::StartService(uint16_t service_handle)
{
	esp_err_t ret;
	ret = esp_ble_gatts_start_service(service_handle);
	if(ret != ESP_OK)
	{
		ESP_LOGI(GATT_SERVICE_NAME,"Service can't be started. Failed with error: %d", ret);
	}
	return ret;
}
esp_err_t GattServer::SendIndication(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle,
					uint16_t value_len, uint8_t *value, bool need_confirm)
{
	esp_err_t ret = esp_ble_gatts_send_indicate( gatts_if,  conn_id,  attr_handle,  value_len,  value,  need_confirm);
	if(ret != ESP_OK)
	{
		ESP_LOGE(GATT_SERVICE_NAME,"Send indication failed with status %d", ret);
	}
	return ret;
}
esp_err_t GattServer::AddCharDescr(uint16_t service_handle,
						esp_bt_uuid_t   *descr_uuid,
						esp_gatt_perm_t perm, esp_attr_value_t *char_descr_val,
						esp_attr_control_t *control)
{
	esp_err_t ret;
	ret = esp_ble_gatts_add_char_descr(service_handle,
							descr_uuid,
							perm,
							char_descr_val,
							control);
	if(ret != ESP_OK)
	{
		ESP_LOGE(GATT_SERVICE_NAME,"Descriptor failed to be added with status %d", ret);
	}
	return ret;
}
esp_err_t GattServer::AddChar(uint16_t service_handle,  esp_bt_uuid_t  *char_uuid,
				esp_gatt_perm_t perm, esp_gatt_char_prop_t property, esp_attr_value_t *char_val,
				esp_attr_control_t *control)
{
	esp_err_t ret;
	ret = esp_ble_gatts_add_char(service_handle,
				char_uuid,
				perm,
				property,
				char_val,
				control);
	if(ret != ESP_OK)
	{
		ESP_LOGE(GATT_SERVICE_NAME,"Descriptor failed to be added with status %d", ret);
	}
	return ret;
}



GattServer& GattServer::getInstance(void)
{
	if(gattsEventHandler == NULL)
	{
		gattsEventHandler = &gatts_callback;
	}
	return GattServer::gattServer;
}