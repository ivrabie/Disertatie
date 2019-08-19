/*
 * FlashDeviceManager.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: ion
 */
#include "../../SWLoad/FlashDeviceManager/FlashDeviceManager.h"

#include <string.h>
#include <esp_bt_defs.h>
using namespace SWLOAD;

FlashDeviceManager::FlashDeviceManager()
{
	this->numberOfDevices = 0;
}
FlashDeviceManager::~FlashDeviceManager()
{

}

void FlashDeviceManager::RegisterDevice(FlashDevice *device)
{
	uint8_t i = 0;
	for(i = 0; i < this->numberOfDevices; i++)
	{
		if(memcmp((void *)this->listOfDevice[i].bda,(void *)device->bda,ESP_BD_ADDR_LEN) == 0)
		{
			return;
		}
	}
	if(this->numberOfDevices < MAX_BLE_DEVICES)
	{
		this->listOfDevice[this->numberOfDevices] = (*device);
		this->numberOfDevices += 1u;
	}
}


void FlashDeviceManager::RemoveDevice(esp_bd_addr_t bda)
{
	uint8_t foundIdx = 0xFF;
	uint8_t i = 0;
	for(i = 0; i < this->numberOfDevices; i++)
	{
		if(memcmp((void *)this->listOfDevice[i].bda,(void *)bda,ESP_BD_ADDR_LEN) == 0)
		{
			foundIdx = i;
		}
	}
	if(foundIdx != 0xFF)
	{
		for(i = foundIdx; i < this->numberOfDevices - 1; i++)
		{
			this->listOfDevice[i] = this->listOfDevice[i + 1];
		}
		this->numberOfDevices -= 1u;
	}
}

void FlashDeviceManager::RemoveDevice(uint16_t conn_id)
{
	uint8_t foundIdx = 0xFF;
	uint8_t i = 0;
	for(i = 0; i < this->numberOfDevices; i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			foundIdx = i;
		}
	}
	if(foundIdx != 0xFF)
	{
		for(i = foundIdx; i < this->numberOfDevices - 1; i++)
		{
			this->listOfDevice[i] = this->listOfDevice[i + 1];
		}
		this->numberOfDevices -= 1u;
	}
}

void FlashDeviceManager::UpdateDevice(uint16_t conn_id, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type)
{
	uint8_t i = 0;
	for(i = 0; i < this->numberOfDevices; i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			this->updateField(i,field_value,field_type);
			break;
		}
	}
}
void FlashDeviceManager::UpdateDevice(esp_bd_addr_t bda, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type)
{
	uint8_t i = 0;
	for(i = 0; i < this->numberOfDevices; i++)
	{
		if(memcmp((void *)this->listOfDevice[i].bda,(void *)bda,ESP_BD_ADDR_LEN) == 0)
		{
			this->updateField(i,field_value,field_type);
			break;
		}
	}
}
void FlashDeviceManager::updateField(uint8_t idx, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type)
{
	switch(field_type)
	{
	case DEVICE_UPDATEFIELD_CONECTION_ID:
		this->listOfDevice[idx].conn_id = field_value;
		break;
	case DEVICE_UPDATEFIELD_CONECTION_STATUS:
		this->listOfDevice[idx].status = (esp_gatt_status_t)field_value;
		break;
	case DEVICE_UPDATEFIELD_CONECTION_MTU:
		this->listOfDevice[idx].mtu = field_value;
		break;
	}
}


