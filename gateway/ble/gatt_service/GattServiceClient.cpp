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
bool GattServiceClient::NotifyDiscoverComplete(esp_gatt_if_t gattc_if, uint16_t conn_id)
{
	uint16_t count= 1;
	esp_gattc_service_elem_t result;
	esp_gattc_char_elem_t    resultChr;
	esp_gattc_descr_elem_t   resultDesc;
	uint8_t i = 0;
	uint8_t j = 0;
	bool retVal = false;
	if(this->IsServiceInit() == false)
	{
		this->PrintService();
		esp_gatt_status_t ret = this->gattc.GetService(gattc_if,conn_id,&this->serviceInfo.id.uuid,&result, &count, 0);
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
				ret = this->gattc.GetCharByUuid(gattc_if,conn_id,this->serviceHandle,this->endServiceHandle,this->listOfChr[i].uuid,&resultChr,&count);
				ESP_LOGI(GATT_SERVICECLIENT_NAME, "Count chr %d",count);
				if(ESP_GATT_OK == ret && count != 0u)
				{
					ESP_LOGI(GATT_SERVICECLIENT_NAME, "Result handle %d",resultChr.char_handle);
					this->listOfChr[i].chr_handle = resultChr.char_handle;
					this->listOfChr[i].property = resultChr.properties;
					for(j = 0; j < this->listOfChr[i].noOfRegistredDesc; j++)
					{
						count = 1;
						ret = this->gattc.GetDescByUuid(gattc_if, conn_id,this->serviceHandle,this->endServiceHandle,this->listOfChr[i].uuid,this->listOfChr[i].listOfDesc[j].uuid,&resultDesc,&count);
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
			retVal = true;
		}
		else
		{
			ESP_LOGE(GATT_SERVICECLIENT_NAME, "Service failed to be discovered");
		}
	}
	else
	{
		ESP_LOGW(GATT_SERVICECLIENT_NAME, "Service already discovered");
	}
	return retVal;
}
void GattServiceClient::RefreshServiceInfo(esp_bd_addr_t remote_bda)
{
	this->gattc.RefreshCash(remote_bda);

}


void GattServiceClient::RequestReadAttr(esp_gatt_if_t gattc_if, uint16_t conn_id,esp_bt_uuid_t *uuid)
{
	AttributeType attrType;
	uint16_t attrHandle = this->SearchAttrHandle(uuid,attrType);

	if(attrType == ATTR_TYPE_CHR)
	{
		this->gattc.ReadChar(gattc_if,conn_id, attrHandle, ESP_GATT_AUTH_REQ_NONE);
	}
	else if(attrType == ATTR_TYPE_DESC)
	{
			this->gattc.ReadDesc(gattc_if,conn_id, attrHandle, ESP_GATT_AUTH_REQ_NONE);
	}
	else if(attrType == ATTR_TYPE_NONE)
	{
		ESP_LOGE(GATT_SERVICECLIENT_NAME, "Failed to foud uuid %d for read", uuid->uuid.uuid16);
	}
}

esp_err_t GattServiceClient::RegisterForNotify(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda, esp_bt_uuid_t *uuid)
{
	esp_err_t ret = ESP_FAIL;
	uint16_t attrHandle  = 0;
	AttributeType attrType;
	attrHandle = this->SearchAttrHandle(uuid,attrType);

	if(attrHandle != 0)
	{
		ret = this->gattc.RegisterForNotify(gattc_if,server_bda,attrHandle);
		if(ret != ESP_OK)
		{
			ESP_LOGE(GATT_SERVICECLIENT_NAME, "Failed to write chr %d", uuid->uuid.uuid16);
		}
	}
	else
	{
		ESP_LOGE(GATT_SERVICECLIENT_NAME, "Failed to write chr %d handle %d", uuid->uuid.uuid16,attrHandle);
	}
	return ret;

}
GattServiceClient::GattServiceClient(const GattServiceClient &servClient):GattService(servClient)
{

}


GattServiceClient& GattServiceClient::operator=(const GattServiceClient &gattService)
{
	GattService::operator=(gattService);
	return (*this);
}


esp_err_t GattServiceClient::RequestWriteAttr(esp_gatt_if_t gattc_if, uint16_t conn_id,esp_bt_uuid_t *uuid, uint16_t value_len, uint8_t *value, esp_gatt_write_type_t write_type, esp_gatt_auth_req_t auth_req)
{
	esp_err_t ret = ESP_FAIL;
	AttributeType attrType;
	uint16_t attrHandle = this->SearchAttrHandle(uuid,attrType);
	if(attrType == ATTR_TYPE_CHR)
	{
		ret = this->gattc.RequestWriteChar(gattc_if,conn_id,attrHandle,value_len,value,write_type,auth_req);
	}
	else if(attrType == ATTR_TYPE_DESC)
	{
		ret = this->gattc.RequestWriteDesc(gattc_if,conn_id,attrHandle,value_len,value,write_type,auth_req);
	}
	else if(attrType == ATTR_TYPE_NONE)
	{
		ESP_LOGE(GATT_SERVICECLIENT_NAME, "Failed to foud uuid %d for read", uuid->uuid.uuid16);
	}
	return ret;
}

void GattServiceClient::OpenConnection(esp_gatt_if_t gattc_if, esp_bd_addr_t remote_bda, esp_ble_addr_type_t remote_addr_type, bool is_direct)
{
	
	esp_err_t ret = this->gattc.OpenConnection(gattc_if,remote_bda,remote_addr_type,is_direct);
	if(ret != ESP_OK)
	{
		ESP_LOGE(GATT_SERVICECLIENT_NAME, "Connection failed to open service %d", this->serviceInfo.id.uuid.uuid.uuid16);
	}
}

void GattServiceClient::DisconnectService(esp_gatt_if_t gattc_if, uint16_t conn_id)
{
	this->gattc.Disconnect(gattc_if,conn_id);
}