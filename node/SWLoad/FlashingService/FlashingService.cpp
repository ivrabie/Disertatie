/*
 * FlashingService.cpp
 *
 *  Created on: Aug 11, 2019
 *      Author: ion
 */

#include "FlashingService.h"
#include <esp_log.h>
using namespace SWLOAD;

FlashinService::FlashinService()
{
	ESP_LOGI(FLASHING_SERVICE_NAME, " Start creating");
	GattCharacteristic tempChr;
	GattDescriptor     tempDesc;

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




void FlashinService::NotifyReadChr(esp_gatt_if_t gatts_if,uint16_t conn_id,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long,  bool need_rsp)
{
	uint8_t i = 0;
	if(need_rsp == true)
	{
		for(i = 0; i < this->noOfRegistredChrs; i++)
		{
			if(this->listOfChr[i].chr_handle == handle)
			{
				this->processReadChr(&this->listOfChr[i].uuid,gatts_if,conn_id,trans_id,handle,offset,is_long); // @suppress("Invalid arguments")
			}
		}
	}

}


void FlashinService::processReadChr(esp_bt_uuid_t *uuid, esp_gatt_if_t gatts_if,uint16_t conn_id,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long)
{
	if(uuid->len == ESP_UUID_LEN_16)
	{
		if(uuid->uuid.uuid16 == FLASHING_CHR_VERSION_UUID)
		{
			uint8_t data1[2u];
			data1[0] = 0x01;
			data1[1] = 0x02;
			esp_log_buffer_hex(FLASHING_SERVICE_NAME, data1, 2);
			this->SendResponse(gatts_if,conn_id,trans_id,handle,offset,is_long,data1,2);
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_DATABLCOK_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_REQBLOCK_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_REQBLOCK_UUID)
		{

		}
	}
}

void FlashinService::RequestBlock(esp_gatt_if_t gatts_if,uint16_t conn_id)
{
	esp_bt_uuid_t uuid;
	uint16_t attrHandle;
	uint8_t data[2];
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_CHR_REQBLOCK_UUID;
	attrHandle = this->searchAttrHandle(&uuid);
	data[0] = 0x1;
	data[1] = 0x3;
	if(attrHandle != 0)
	{
		ESP_LOGI(FLASHING_SERVICE_NAME, "Notification sent Conn id %d gatt_if %d attrHandle %d",conn_id,gatts_if,attrHandle);
		this->SendIndication(gatts_if,conn_id,attrHandle,2,data,false);
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

FlashinService::FlashinService(const FlashinService &flashService):GattServiceServer(flashService)
{

}
FlashinService::~FlashinService()
{

}


