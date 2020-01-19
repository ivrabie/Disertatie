/*
 * GattClient.cpp
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#include "GattClient.h"
#include <esp_log.h>


using namespace BLE;

GattClient GattClient::gattc;
etl::function_imp<GattClient, GattcEventInfo, GattClient::gattc, &GattClient::gattcCallback> gattc_callback;
etl::ifunction<GattcEventInfo>* gattcEventHandler = NULL;
extern "C"
{
	void gattcEventHandlerCbk(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
	{
		GattcEventInfo info;
		info.event = event;
		info.param = param;
		info.gattc_if = gattc_if;
		(*gattcEventHandler)(info);
	}
}

GattClient::GattClient()
{
}

GattClient::~GattClient()
{

}

esp_err_t GattClient::Init(void) // @suppress("Member declaration not found")
{
	esp_err_t ret = ESP_OK;
	// if(this->bleAdapter != NULL)
	// {
		if(this->bleAdapter.IsBleStackActive() == false)
		{
			ret = this->bleAdapter.Init();
		}
	// }
	if(ret)
	{
		return ret;
	}

	ret = esp_ble_gattc_register_callback(gattcEventHandlerCbk);
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



void GattClient::gattcCallback(GattcEventInfo info)
{
	esp_gattc_cb_event_t event = info.event;
	esp_gatt_if_t gattc_if = info.gattc_if; 
	esp_ble_gattc_cb_param_t *param = info.param;
	ESP_LOGI(GATT_CLIENT_NAME, "GATTC Event: %d", event);
	ESP_LOGI(GATT_CLIENT_NAME, "GATTC interface: %d", gattc_if);

	this->notify_observers(info);
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

esp_err_t GattClient::Disconnect(esp_gatt_if_t gattc_if, uint16_t conn_id)
{
	esp_err_t ret = esp_ble_gattc_close(gattc_if, conn_id);
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


GattClient& GattClient::getInstance(void)
{
	if(gattcEventHandler == NULL)
	{
		gattcEventHandler = &gattc_callback;
	}
	return GattClient::gattc;
}



esp_err_t GattClient::RegisterForNotify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, uint16_t handle)
{
	esp_err_t ret = esp_ble_gattc_register_for_notify(gattc_if,server_bda,handle);
	if (ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s refresh failed\n", __func__);
	}
	return ret;
}


void GattClient::ReadChar(esp_gatt_if_t gattc_if,uint16_t conn_id, uint16_t handle,esp_gatt_auth_req_t auth_req)
{
		esp_err_t ret = esp_ble_gattc_read_char(gattc_if,conn_id, handle, auth_req);
		if (ret)
		{
			ESP_LOGE(GATT_CLIENT_NAME, "%s Failed to read \n", __func__);
		}
}

void GattClient::ReadDesc(esp_gatt_if_t gattc_if,uint16_t conn_id, uint16_t handle,esp_gatt_auth_req_t auth_req)
{
		esp_err_t ret = esp_ble_gattc_read_char_descr(gattc_if,conn_id, handle, auth_req);
		if (ret)
		{
			ESP_LOGE(GATT_CLIENT_NAME, "%s Failed to read \n", __func__);
		}
}
void GattClient::RefreshCash(esp_bd_addr_t remote_bda)
{
	esp_err_t ret = esp_ble_gattc_cache_refresh(remote_bda);
	if (ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s refresh failed\n", __func__);
	}
}
esp_gatt_status_t  GattClient::GetService(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_bt_uuid_t *svc_uuid, esp_gattc_service_elem_t *result, uint16_t *count, uint16_t offset)
{
	esp_gatt_status_t ret = esp_ble_gattc_get_service(gattc_if,conn_id,svc_uuid,result,count, offset);
	if (ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s Failed to get service \n", __func__);

	}
	return ret;
}
esp_gatt_status_t GattClient::GetCharByUuid(esp_gatt_if_t gattc_if, 
						uint16_t conn_id,
						uint16_t start_handle,
						uint16_t end_handle,
						esp_bt_uuid_t char_uuid,
						esp_gattc_char_elem_t *result,
						uint16_t *count)
{
	esp_gatt_status_t ret = esp_ble_gattc_get_char_by_uuid(gattc_if,conn_id,start_handle,end_handle,char_uuid,result,count);
	if (ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s Failed to get characteristic \n", __func__);
	}
	return ret;
}
esp_gatt_status_t GattClient::GetDescByUuid(esp_gatt_if_t gattc_if,
						uint16_t conn_id,
						uint16_t start_handle,
						uint16_t end_handle,
						esp_bt_uuid_t char_uuid,
						esp_bt_uuid_t descr_uuid,
						esp_gattc_descr_elem_t *result,
						uint16_t *count)
{
	esp_gatt_status_t ret = esp_ble_gattc_get_descr_by_uuid(gattc_if,conn_id,start_handle,end_handle,char_uuid,descr_uuid,result,count);
	if (ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s Failed to get description \n", __func__);
	}
	return ret;
}

esp_err_t GattClient::RequestWriteChar(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req)
{
	esp_err_t ret;
	ret = esp_ble_gattc_write_char(gattc_if,conn_id,handle,value_len,value,write_type,auth_req);
	if (ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s Failed to write characteristic \n", __func__);
	}
	return ret;
}


esp_err_t GattClient::RequestWriteDesc(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t handle, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req)
{
	esp_err_t ret;
	ret = esp_ble_gattc_write_char_descr(gattc_if,conn_id,handle,value_len,value,write_type,auth_req);
	if (ret)
	{
		ESP_LOGE(GATT_CLIENT_NAME, "%s Failed to write characteristic \n", __func__);
	}
	return ret;
}