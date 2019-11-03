/*
 * gatt_service.cpp
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */


#include "GattService.h"
#include <string.h>
#include <esp_log.h>
using namespace BLE;

GattService::GattService()
{
	this->noOfRegistredChrs = 0u;
}

GattService::~GattService()
{

}

bool GattService::IsUuidEqual(esp_bt_uuid_t *uuid, esp_bt_uuid_t *uuid1)
{
	bool ret = false;
	if(uuid->len == uuid1->len)
	{
		if(uuid->len == ESP_UUID_LEN_16)
		{
			ret = (uuid->uuid.uuid16 == uuid1->uuid.uuid16) ? true : false;
		}
		else if(uuid->len == ESP_UUID_LEN_32)
		{
			ret = (uuid->uuid.uuid32 == uuid1->uuid.uuid16) ? true : false;
		}
		else
		{
			ret = (memcmp((void*)uuid->uuid.uuid128,(void*)uuid1->uuid.uuid128,ESP_UUID_LEN_128) == 0) ? true : false;
		}
	}
	return ret;
}

uint8_t GattService::isChrsAdded(esp_bt_uuid_t *uuid)
{
	uint8_t i = 0;
	uint8_t foundIdx = 0xFF;
	bool ret = false;
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		ret = GattService::IsUuidEqual(uuid,&this->listOfChr[i].uuid);
		if(true == ret)
		{
			foundIdx = i;
			break;
		}
	}
	return foundIdx;
}

bool GattService::IsServiceInit(void)
{
	bool ret = true;
	uint8_t i = 0;
	uint8_t j = 0;
	if(this->serviceHandle == 0)
	{
		ret = false;
	}
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		if(this->listOfChr[i].chr_handle == 0)
		{
			ret = false;
			break;
		}
		else
		{
			for(j = 0; j < this->listOfChr[i].noOfRegistredDesc; j++)
			{
				if(this->listOfChr[i].listOfDesc[j].desc_handle == 0)
				{
					ret = false;
					break;
				}
			}
			if(ret == false)
			{
				break;
			}
		}
	}
	return ret;
}


uint16_t GattService::SearchAttrHandle(esp_bt_uuid_t *uuid,AttributeType &attrType)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint16_t retHandle = 0;
	bool isDesc = false;
	attrType = ATTR_TYPE_NONE;
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		if(GattService::IsUuidEqual(&this->listOfChr[i].uuid,uuid) == true)
		{
			retHandle = this->listOfChr[i].chr_handle;
			attrType = ATTR_TYPE_CHR;
			break;
		}
		for(j = 0; j < this->listOfChr[i].noOfRegistredDesc; j++)
		{
			if(GattService::IsUuidEqual(&this->listOfChr[i].listOfDesc[j].uuid,uuid) == true)
			{
				retHandle = this->listOfChr[i].listOfDesc[j].desc_handle;
				isDesc = true;
				attrType = ATTR_TYPE_DESC;
				break;
			}

		}
		if(isDesc == true)
		{
			break;
		}
	}
	return retHandle;
}


void GattCharacteristic::PrintChr(void)
{
	uint8_t i = 0;
	ESP_LOGI(GATT_SERVICE_NAME, "Chr perm %d "
								"Chr control %d "
								"Chr handle %d"
								"Chr uuid len %d "
								"Chr property %d "
								"Chr nr of desc %d ",
								this->perm,
								this->control.auto_rsp,
								this->chr_handle,
								this->uuid.len,
								this->property,
								this->noOfRegistredDesc);
	if(this->uuid.len == ESP_UUID_LEN_16)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Chr uuid %d",this->uuid.uuid.uuid16);
	}
	else if(this->uuid.len == ESP_UUID_LEN_32)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Chr uuid %d",this->uuid.uuid.uuid32);
	}
	else if(this->uuid.len == ESP_UUID_LEN_128)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Chr uuid ");
		esp_log_buffer_hex(GATT_SERVICE_NAME, this->uuid.uuid.uuid128, ESP_UUID_LEN_128);
	}
	for(i = 0; i < this->noOfRegistredDesc; i++)
	{
		this->listOfDesc[i].PrintDesc();
	}
}


void GattDescriptor::PrintDesc(void)
{
	ESP_LOGI(GATT_SERVICE_NAME, "Desc perm %d "
								"Desc control %d "
								"Desc handle %d "
								"Desc uuid len %d ",
								this->perm,
								this->control.auto_rsp,
								this->desc_handle,
								this->uuid.len);
	if(this->uuid.len == ESP_UUID_LEN_16)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Desc uuid %d",this->uuid.uuid.uuid16);
	}
	else if(this->uuid.len == ESP_UUID_LEN_32)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Desc uuid %d",this->uuid.uuid.uuid32);
	}
	else if(this->uuid.len == ESP_UUID_LEN_128)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Desc uuid ");
		esp_log_buffer_hex(GATT_SERVICE_NAME, this->uuid.uuid.uuid128, ESP_UUID_LEN_128);
	}
}
bool GattService::AddChr(GattCharacteristic *chr)
{
	bool ret = false;
	uint8_t foundIdx = this->isChrsAdded(&chr->uuid);

	if(foundIdx == 0xFF)
	{
		if(this->noOfRegistredChrs < MAX_CHR_PER_SERVICE)
		{
			this->listOfChr[this->noOfRegistredChrs].Clone(*chr);
			this->noOfRegistredChrs += 1u;
			ret = true;
		}
	}
	return ret;

}
bool GattService::DeleteChr(GattCharacteristic *chr)
{
	bool ret = false;
	uint8_t foundIdx = this->isChrsAdded(&chr->uuid);
	uint8_t i = 0;
	if(foundIdx != 0xFF)
	{
		for(i = foundIdx; i < this->noOfRegistredChrs - 1; i++)
		{
			this->listOfChr[i].Clone(this->listOfChr[i + 1]);
		}
		this->noOfRegistredChrs -= 1u;
		ret = true;
	}
	return ret;
}

void GattService::PrintService(void)
{
	uint8_t i = 0;
	ESP_LOGI(GATT_SERVICE_NAME, "Service is primary %d"
								"Service inst id %d"
								"Service start handle %d"
								"Service stop handle %d"
								"Service uuid len %d"
								"Service nr chr %d",
								this->serviceInfo.is_primary,
								this->serviceInfo.id.inst_id,
								this->serviceHandle,
								this->endServiceHandle,
								this->serviceInfo.id.uuid.len,
								this->noOfRegistredChrs);
	if(this->serviceInfo.id.uuid.len == ESP_UUID_LEN_16)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Service uuid %d",this->serviceInfo.id.uuid.uuid.uuid32);
	}
	else if(this->serviceInfo.id.uuid.len == ESP_UUID_LEN_32)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Service uuid %d",this->serviceInfo.id.uuid.uuid.uuid32);
	}
	else if(this->serviceInfo.id.uuid.len == ESP_UUID_LEN_128)
	{
		ESP_LOGI(GATT_SERVICE_NAME, "Service uuid ");
		esp_log_buffer_hex(GATT_SERVICE_NAME, this->serviceInfo.id.uuid.uuid.uuid128,ESP_UUID_LEN_128);
	}
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		this->listOfChr[i].PrintChr();
	}
}
bool GattService::DeleteChr(esp_bt_uuid_t *uuid)
{
	bool ret = false;
	uint8_t foundIdx = this->isChrsAdded(uuid);
	uint8_t i = 0;
	if(foundIdx != 0xFF)
	{
		for(i = foundIdx; i < this->noOfRegistredChrs - 1; i++)
		{
			this->listOfChr[i].Clone(this->listOfChr[i + 1]);
		}
		this->noOfRegistredChrs -= 1u;
		ret = true;
	}
	return ret;
}




void GattService::Clone(const GattService &serv)
{
	uint8_t i;
	//ESP_LOGI(GATT_SERVICE_NAME, "Copy service");
	for(i = 0; i < serv.noOfRegistredChrs; i++)
	{
		this->listOfChr[i] = serv.listOfChr[i];
	}
	this->noOfRegistredChrs = serv.noOfRegistredChrs;
	this->endServiceHandle = serv.endServiceHandle;
	this->numHandles = serv.numHandles;
	this->serviceHandle = serv.serviceHandle;
	memcpy((void*)&this->serviceInfo,(void*)&serv.serviceInfo,sizeof(esp_gatt_srvc_id_t));
}
void GattDescriptor::Clone(const GattDescriptor &desc)
{
	//ESP_LOGI(GATT_SERVICE_NAME, "Copy desc");
	memcpy(&this->uuid,&desc.uuid, sizeof(esp_bt_uuid_t));
	memcpy(&this->desc_val,&desc.desc_val, sizeof(esp_attr_value_t));
	this->desc_handle = desc.desc_handle;
	this->perm = desc.perm;
	this->control = desc.control;
}
void GattCharacteristic::Clone(const GattCharacteristic &chr)
{
	//ESP_LOGI(GATT_SERVICE_NAME, "Copy chr");
	uint8_t i = 0;
	memcpy(&this->uuid,&chr.uuid, sizeof(esp_bt_uuid_t));
	memcpy(&this->char_val,&chr.char_val, sizeof(esp_attr_value_t));
	this->chr_handle = chr.chr_handle;
	this->perm = chr.perm;
	this->control = chr.control;
	this->property = chr.property;
	this->noOfRegistredDesc = chr.noOfRegistredDesc;
	for(i = 0; i < MAX_DESC_PER_CHR; i++)
	{
		this->listOfDesc[i].Clone(chr.listOfDesc[i]);
	}
}



GattCharacteristic::GattCharacteristic()
{

}

GattCharacteristic::~GattCharacteristic()
{

}

GattCharacteristic::GattCharacteristic(const GattCharacteristic &chr)
{
	this->Clone(chr);
}

uint8_t GattCharacteristic::isDescAdded(esp_bt_uuid_t *uuid)
{
	uint8_t i = 0;
	uint8_t foundIdx = 0xFF;
	bool ret = false;

	for(i = 0; i < this->noOfRegistredDesc; i++)
	{
		ret = GattService::IsUuidEqual(uuid,&this->listOfDesc[i].uuid);
		if(true == ret)
		{
			foundIdx = i;
			break;
		}
	}
	return foundIdx;
}

bool GattCharacteristic::AddDesc(GattDescriptor *desc)
{
	bool ret = false;
	uint8_t foundIdx = this->isDescAdded(&desc->uuid);

	if(foundIdx == 0xFF)
	{
		if(this->noOfRegistredDesc < MAX_CHR_PER_SERVICE)
		{
			this->listOfDesc[this->noOfRegistredDesc].Clone(*desc);
			this->noOfRegistredDesc += 1u;
			ret = true;
		}
	}
	return ret;
}
bool GattCharacteristic::DeleteDesc(GattDescriptor *desc)
{
	bool ret = false;
	uint8_t foundIdx = this->isDescAdded(&desc->uuid);
	uint8_t i = 0;
	if(foundIdx != 0xFF)
	{
		for(i = foundIdx; i < this->noOfRegistredDesc - 1; i++)
		{
			this->listOfDesc[i].Clone(this->listOfDesc[i + 1]);
		}
		this->noOfRegistredDesc -= 1u;
		ret = true;
	}
	return ret;
}
bool GattCharacteristic::DeleteDesc(esp_bt_uuid_t *uuid)
{
	bool ret = false;
	uint8_t foundIdx = this->isDescAdded(uuid);
	uint8_t i = 0;
	if(foundIdx != 0xFF)
	{
		for(i = foundIdx; i < this->noOfRegistredDesc - 1; i++)
		{
			this->listOfDesc[i].Clone(this->listOfDesc[i + 1]);
		}
		this->noOfRegistredDesc -= 1u;
		ret = true;
	}
	return ret;
}

GattService::GattService(const GattService &serv)
{
	this->Clone(serv);
}


GattDescriptor::GattDescriptor()
{

}

GattDescriptor::GattDescriptor(const GattDescriptor &desc)
{
	this->Clone(desc);
}

GattDescriptor::~GattDescriptor()
{

}


GattService& GattService::operator=(const GattService &gattService)
{
	uint8_t i;
	//ESP_LOGI(GATT_SERVICE_NAME, "Copy service");
	for(i = 0; i < gattService.noOfRegistredChrs; i++)
	{
		this->listOfChr[i] = gattService.listOfChr[i];
	}
	this->noOfRegistredChrs = gattService.noOfRegistredChrs;
	this->endServiceHandle = gattService.endServiceHandle;
	this->numHandles = gattService.numHandles;
	this->serviceHandle = gattService.serviceHandle;
	memcpy((void*)&this->serviceInfo,(void*)&gattService.serviceInfo,sizeof(esp_gatt_srvc_id_t));
	return (*this);
}

esp_bt_uuid_t* GattService::GetAttrUuidByHandle(uint16_t handle)
{
	uint8_t i;
	uint8_t j;
	esp_bt_uuid_t * ret = NULL;
	bool isDesc = false;
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		if(this->listOfChr[i].chr_handle == handle)
		{
			ret = &this->listOfChr[i].uuid; // @suppress("Invalid arguments")
			break;
		}
		for(j = 0; j < this->listOfChr[i].noOfRegistredDesc; j++)
		{
			if(this->listOfChr[i].listOfDesc[j].desc_handle == handle)
			{
				ret = &this->listOfChr[i].listOfDesc[j].uuid;
				isDesc = true;
			}
		}
		if(isDesc == true)
		{
			break;
		}

	}
	return ret;
}