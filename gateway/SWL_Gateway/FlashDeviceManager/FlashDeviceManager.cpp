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
	this->bufferedEvents.currentListIdxUsed = 0u;
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

void FlashDeviceManager::processEvent(EventType type, uint16_t conn_id,uint16_t handle,uint8_t *val, uint32_t len)
{
	bool isEventOkay = true;
	EventInfo recvEvent;
	if(this->bufferedEvents.listOfEvents[this->bufferedEvents.currentListIdxUsed].size() != 0u)
	{
		for(uint8_t i = 0u; i < this->bufferedEvents.listOfEvents[this->bufferedEvents.currentListIdxUsed].size(); i++)
		{
			if(type != this->bufferedEvents.listOfEvents[this->bufferedEvents.currentListIdxUsed][i].event_type)
			{
				isEventOkay = false;
				break;
			}
		}
	}
	if(isEventOkay == false)
	{
		ESP_LOGE(FLASHDEVICEMANAGER, "The event type exptected %d, received %d",this->bufferedEvents.listOfEvents[this->bufferedEvents.currentListIdxUsed][0].event_type,
				type);
	}
	else
	{
		recvEvent.event_type = type;
		recvEvent.conn_id = conn_id;
		recvEvent.handle = handle;
		recvEvent.val_len = len;
		if(len < MAX_EVENT_INFO_SIZE && len != 0)
		{
			memcpy(recvEvent.val,val,len);
		}
		else
		{
			ESP_LOGE(FLASHDEVICEMANAGER, "The len of the event is too big %d",len);
		}
		if(this->bufferedEvents.listOfEvents[this->bufferedEvents.currentListIdxUsed].size() < MAX_BLE_DEVICES)
		{
			this->bufferedEvents.listOfEvents[this->bufferedEvents.currentListIdxUsed].push_back(recvEvent);
		}
		else
		{
			ESP_LOGE(FLASHDEVICEMANAGER, "Buffers full");
		}
	}
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
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
			// this->bufferedEvents.listOfEvents[this->bufferedEvents.currentListIdxUsed][j].conn_id)
		{
			this->listOfDevice[i].NotifyReadAttrEvent(handle,value,value_len);
			break;
		}
	}

}


void FlashDeviceManager::NotifyWriteEvtAttr(uint16_t conn_id,uint16_t handle)
{
	uint8_t i = 0;
	for(i = 0; i < this->listOfDevice.size(); i++)
	{
		if(this->listOfDevice[i].conn_id == conn_id)
		{
			this->listOfDevice[i].NotifyWriteAttrEvent(handle);
			break;			
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