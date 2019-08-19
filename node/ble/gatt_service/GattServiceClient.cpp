/*
 * GattServiceClient.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: ion
 */




#include "GattServiceClient.h"
#include "esp_log.h"
using namespace BLE;


GattServiceClient::GattServiceClient()
{

}

GattServiceClient::~GattServiceClient()
{

}
void GattServiceClient::NotifyDiscoverComplete(esp_gatt_if_t gattc_if, uint16_t conn_id)
{
	uint16_t count= 1;
	esp_gattc_service_elem_t result;
	esp_gattc_char_elem_t    resultChr;
	esp_gattc_descr_elem_t   resultDesc;
	uint8_t i = 0;
	uint8_t j = 0;
	this->PrintService();
	esp_gatt_status_t ret = esp_ble_gattc_get_service(gattc_if,conn_id,&this->serviceInfo.id.uuid,&result, &count, 0);
	ESP_LOGI(GATT_SERVICECLIENT_NAME, "Count service %d",count);
	if(ESP_GATT_OK == ret && count != 0u)
	{
		ESP_LOGI(GATT_SERVICECLIENT_NAME, "Result handle %d",result.start_handle);
		this->serviceHandle = result.start_handle;
		this->serviceInfo.is_primary = result.is_primary;
		this->endServiceHandle = result.end_handle;

		for(i = 0; i < this->noOfRegistredChrs; i++)
		{
			count = 1;
			ret = esp_ble_gattc_get_char_by_uuid(gattc_if,conn_id,this->serviceHandle,this->endServiceHandle,this->listOfChr[i].uuid,&resultChr,&count);
			ESP_LOGI(GATT_SERVICECLIENT_NAME, "Count chr %d",count);
			if(ESP_GATT_OK == ret && count != 0u)
			{
				ESP_LOGI(GATT_SERVICECLIENT_NAME, "Result handle %d",resultChr.char_handle);
				this->listOfChr[i].chr_handle = resultChr.char_handle;
				this->listOfChr[i].property = resultChr.properties;
				for(j = 0; j < this->listOfChr[i].noOfRegistredDesc; j++)
				{
					count = 1;
					ret = esp_ble_gattc_get_descr_by_uuid(gattc_if, conn_id,this->serviceHandle,this->endServiceHandle,this->listOfChr[i].uuid,this->listOfChr[i].listOfDesc[j].uuid,&resultDesc,&count);
					ESP_LOGI(GATT_SERVICECLIENT_NAME, "Count desc %d",count);
					if(ESP_GATT_OK == ret && count != 0u)
					{
						ESP_LOGI(GATT_SERVICECLIENT_NAME, "Result handle %d",resultDesc.handle);
						this->listOfChr[i].listOfDesc[j].desc_handle = resultDesc.handle;
					}
				}
			}

		}
	}
	if(this->IsServiceInit() == true)
	{
		ESP_LOGI(GATT_SERVICECLIENT_NAME, "Service was discovered");
	}
	else
	{
		ESP_LOGE(GATT_SERVICECLIENT_NAME, "Service failed to be discovered");
	}
}
void GattServiceClient::RefreshServiceInfo(esp_bd_addr_t remote_bda)
{
	esp_err_t ret = esp_ble_gattc_cache_refresh(remote_bda);
	if (ret)
	{
		ESP_LOGE(GATT_SERVICECLIENT_NAME, "%s refresh failed\n", __func__);
	}

}


void GattServiceClient::RequestReadChr(esp_gatt_if_t gattc_if, uint16_t conn_id,esp_bt_uuid_t *uuid)
{
	uint8_t i = 0;
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		if(GattService::IsUuidEqual(&this->listOfChr[i].uuid,uuid) == true && this->listOfChr[i].chr_handle != 0)
		{
			esp_err_t ret = esp_ble_gattc_read_char(gattc_if,conn_id, this->listOfChr[i].chr_handle, ESP_GATT_AUTH_REQ_NONE);
			if (ret)
			{
				ESP_LOGE(GATT_SERVICE_NAME, "%s Failed to read \n", __func__);
			}
		}
	}
}
void GattServiceClient::RegisterForNotify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, uint16_t handle)
{
	esp_err_t ret = esp_ble_gattc_register_for_notify(gattc_if,server_bda,handle);
	if (ret)
	{
		ESP_LOGE(GATT_SERVICECLIENT_NAME, "%s refresh failed\n", __func__);
	}
}
GattServiceClient::GattServiceClient(const GattServiceClient &servClient):GattService(servClient)
{

}
