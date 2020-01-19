/*
 * FlashDeviceManager.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: ion
 */
#include "FlashDeviceManager.h"
#include <string.h>
#include <esp_bt_defs.h>
using namespace SWL_GATEWAY;

FlashDeviceManager::FlashDeviceManager()
{
}
FlashDeviceManager::~FlashDeviceManager()
{

}

bool FlashDeviceManager::RegisterDevice(FlashDevice *device)
{
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(memcmp((void *)this->listOfDevice[i].bda,(void *)device->bda,ESP_BD_ADDR_LEN) == 0)
		{
			return false;
		}
	}
	this->listOfDevice.push_back(*device);
	ESP_LOGI(FLASHDEVICEMANAGER, "Added device %d",this->listOfDevice.size());
	this->registredDevice += 1u;
	return true;

}


void FlashDeviceManager::RemoveDevice(esp_bd_addr_t bda)
{
	uint8_t foundIdx = 0xFF;
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(memcmp((void *)this->listOfDevice[i].bda,(void *)bda,ESP_BD_ADDR_LEN) == 0)
		{
			foundIdx = i;
		}
	}
	ESP_LOGI(FLASHDEVICEMANAGER, "Found idx %d, vector size %d",foundIdx,this->listOfDevice.size());
	if(foundIdx != 0xFF)
	{
		this->listOfDevice.erase(this->listOfDevice.begin()+foundIdx);
	}
	ESP_LOGI(FLASHDEVICEMANAGER, "Found idx %d, vector size %d",foundIdx,this->listOfDevice.size());
}

void FlashDeviceManager::RemoveDevice(uint16_t conn_id)
{
	uint8_t foundIdx = 0xFF;
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			foundIdx = i;
			break;
		}
	}
	if(foundIdx != 0xFF)
	{
		this->listOfDevice.erase(this->listOfDevice.begin()+foundIdx);
	}
}

void FlashDeviceManager::UpdateDevice(uint16_t conn_id, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type)
{
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
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
	for(i = 0; i < this->listOfDevice.size(); i++)
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
	this->listOfDevice[idx].UpdateField(field_value,field_type);
}


void FlashDeviceManager::NotifyDiscoverComplete(uint16_t conn_id)
{
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			this->listOfDevice[i].NotifyGattServiceDiscoverComplete();
			break;
		}
	}
}
void FlashDeviceManager::RefreshServiceDiscover(uint16_t conn_id)
{
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			this->listOfDevice[i].RefreshGattServiceCache();
			break;
		}
	}
}

void FlashDeviceManager::NotifyReadEvtAttr(uint16_t conn_id,uint16_t handle,uint8_t *value,uint16_t value_len)
{
	uint8_t i = 0u;
	uint8_t usedIdx = 0xFF;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			this->listOfDevice[i].NotifyReadAttrEvent(handle,value,value_len);
			usedIdx = i;
			break;
		}
	}
	if(usedIdx != 0xFF)
	{
		if(this->listOfDevice[usedIdx].get_state_id() == StateId::DELETE)
		{
			this->RemoveDevice(this->listOfDevice[usedIdx].bda);
		}
	}

}


void FlashDeviceManager::NotifyWriteEvtAttr(uint16_t conn_id,uint16_t handle)
{
	uint8_t i = 0;
	uint8_t usedIdx = 0xFF;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			this->listOfDevice[i].NotifyWriteAttrEvent(handle);
			usedIdx = i;
			break;			
		}
	}
	if(usedIdx != 0xFF)
	{
		if(this->listOfDevice[usedIdx].get_state_id() == StateId::DELETE)
		{
			this->RemoveDevice(this->listOfDevice[usedIdx].bda);
		}
	}
}

void FlashDeviceManager::NotifyDisconnect(esp_bd_addr_t bda)
{
	this->RemoveDevice(bda);
	this->OpenAllConenction();
}
void FlashDeviceManager::OpenAllConenction()
{
	uint8_t i = 0;

	if(this->listOfDevice.size() != 0)
	{
		ESP_LOGE(FLASHDEVICEMANAGER, "Connection id %d, router id %d",this->listOfDevice[0].conn_id,this->listOfDevice[0].get_message_router_id());
		esp_log_buffer_hex(FLASHDEVICEMANAGER,this->listOfDevice[0].bda, 6u);
		this->listOfDevice[0].OpenBleConnection();
	}
}


void FlashDeviceManager::UpdateState(const etl::imessage& message, esp_bd_addr_t bda)
{
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(memcmp((void *)this->listOfDevice[i].bda,(void *)bda,ESP_BD_ADDR_LEN) == 0)
		{
			this->listOfDevice[i].receive(this->listOfDevice[i],message);
			break;
		}
	}	
}

uint8_t FlashDeviceManager::GetNoOfRegistredDevices()
{
	return this->registredDevice;
}

void FlashDeviceManager::ResetInternalInfo()
{
	this->registredDevice = 0u;
}

uint8_t FlashDeviceManager::GetCurrentDevProgress()
{
	uint8_t progress =  0u;
	if(this->listOfDevice.size() != 0)
	{
		if(this->listOfDevice[0].flashFileSize != 0)
		{
			progress = (uint8_t)((((float)this->listOfDevice[0].currentLenSent)/((float)this->listOfDevice[0].flashFileSize)) * (float)100);
		}
	}
	return progress;
}