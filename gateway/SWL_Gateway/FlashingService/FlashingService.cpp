/*
 * FlashingService.cpp
 *
 *  Created on: Aug 11, 2019
 *      Author: ion
 */

#include "FlashingService.h"
#include <esp_log.h>
#include <string.h>
using namespace SWL_GATEWAY;

FlashinService::FlashinService():GattServiceClient()
{
	ESP_LOGI(FLASHING_SERVICE_NAME, " Start creating");
	GattCharacteristic tempChr;
	GattDescriptor     tempDesc;
	memset(this->serviceInfo.id.uuid.uuid.uuid128,0,ESP_UUID_LEN_128);
	this->serviceInfo.id.uuid.len = ESP_UUID_LEN_16;
	this->serviceInfo.id.uuid.uuid.uuid16 = FLASHING_SERVICE_UUID;
	this->serviceInfo.is_primary = true;
	this->serviceInfo.id.inst_id = 0;

	tempChr.uuid.len = ESP_UUID_LEN_16;
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_VERSION_UUID;
	tempChr.control.auto_rsp = ESP_GATT_RSP_BY_APP;
	tempChr.property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;
	tempChr.perm =  ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;
	tempChr.char_val.attr_len = 0;
	tempChr.char_val.attr_max_len = 0;
	tempChr.char_val.attr_value = 0;

	this->AddChr(&tempChr);
	ESP_LOGI(FLASHING_SERVICE_NAME, "Add chr");
	tempChr.uuid.len = ESP_UUID_LEN_16;
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_DATABLCOK_UUID;
	tempChr.char_val.attr_len = 0;
	tempChr.char_val.attr_max_len = 0;
	tempChr.char_val.attr_value = 0;

	tempDesc.uuid.len = ESP_UUID_LEN_16;
	tempDesc.uuid.uuid.uuid16 = FLASHING_DESC_BLOCKIDX_UUID;
	tempDesc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;
	tempDesc.control.auto_rsp = ESP_GATT_RSP_BY_APP;
	tempDesc.desc_val.attr_len = 0;
	tempDesc.desc_val.attr_max_len = 0;
	tempDesc.desc_val.attr_value = 0;

	tempChr.AddDesc(&tempDesc);
	ESP_LOGI(FLASHING_SERVICE_NAME, "Add desc");
	tempDesc.uuid.len = ESP_UUID_LEN_16;
	tempDesc.uuid.uuid.uuid16 = FLASHING_DESC_BLOCKCRC_UUID;
	tempDesc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;
	tempDesc.control.auto_rsp = ESP_GATT_RSP_BY_APP;
	tempDesc.desc_val.attr_len = 0;
	tempDesc.desc_val.attr_max_len = 0;
	tempDesc.desc_val.attr_value = 0;

	tempChr.AddDesc(&tempDesc);
	ESP_LOGI(FLASHING_SERVICE_NAME, "Add desc");
	this->AddChr(&tempChr);
	ESP_LOGI(FLASHING_SERVICE_NAME, "Add chr");
	tempChr.noOfRegistredDesc = 0;
	tempChr.uuid.len = ESP_UUID_LEN_16;
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_REQBLOCK_UUID;
	tempChr.char_val.attr_len = 0;
	tempChr.char_val.attr_max_len = 0;
	tempChr.char_val.attr_value = 0;

	this->AddChr(&tempChr);
	ESP_LOGI(FLASHING_SERVICE_NAME, "Add chr");
	this->numHandles = FLASHING_NUM_OF_HANDLES;
	ESP_LOGI(FLASHING_SERVICE_NAME, "Created");
}

void FlashinService::RequestVersion(esp_gatt_if_t gattc_if, uint16_t conn_id)
{
	esp_bt_uuid_t uuid;
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_CHR_VERSION_UUID;

	this->RequestReadChr(gattc_if,conn_id,&uuid);

}
void FlashinService::NotifyChrEvent(uint16_t handle,uint8_t *value,uint16_t value_len,uint8_t type)
{
	uint8_t i = 0;
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		if(this->listOfChr[i].chr_handle == handle)
		{
			this->processChrEvent(&this->listOfChr[i].uuid, value, value_len,type); // @suppress("Invalid arguments")
		}
	}
}

void FlashinService::processChrEvent(esp_bt_uuid_t *uuid, uint8_t *value,uint16_t value_len,uint8_t type)
{
	if(uuid->len == ESP_UUID_LEN_16)
	{
		if(uuid->uuid.uuid16 == FLASHING_CHR_VERSION_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_DATABLCOK_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_REQBLOCK_UUID)
		{
			if(type == FLASHING_NOTIFICATION_TYPE)
			{
				ESP_LOGI(FLASHING_SERVICE_NAME, "Value len %d", value_len);
				esp_log_buffer_hex(FLASHING_SERVICE_NAME,value,value_len);
			}
		}
	}
}



void FlashinService::RegisterReqBlockForNotification(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda)
{
	esp_bt_uuid_t uuid;
	uint16_t attrHandle  = 0;
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_CHR_REQBLOCK_UUID;
	attrHandle = this->searchAttrHandle(&uuid);
	if(attrHandle != 0)
	{
		this->RegisterForNotify(gattc_if,server_bda, attrHandle);
	}

}
uint16_t FlashinService::searchAttrHandle(esp_bt_uuid_t *uuid)
{
	uint8_t i = 0;
	uint16_t retHandle = 0;
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		if(GattService::IsUuidEqual(&this->listOfChr[i].uuid,uuid) == true)
		{
			retHandle = this->listOfChr[i].chr_handle;
			break;
		}
	}
	return retHandle;

}


FlashinService::FlashinService(const FlashinService &flashService):GattServiceClient(flashService)
{

}
FlashinService::~FlashinService()
{

}


