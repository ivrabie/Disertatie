/*
 * flashingService.c
 *
 *  Created on: Jul 18, 2019
 *      Author: ion
 */

//#include <BleAdapter.hpp>
//#include <BleRemoteDevice.h>
#include "../../SWLoad/FlashDevice/FlashDevice.h"
#include "../../nvs_adapter/NvsAdapter.h"
#include <esp_gatts_api.h>
#include <esp_log.h>
#include <string.h>

#include "../../SWLoad/FlashDevice/FlashDevice_cfg.h"
#define CHAR_VAL_LEN_MAX 0x40
#define FLASH_STATE_MACHINE 0u
using namespace SWLOAD;
using namespace NVS_ADAPTER;
using namespace etl;

FlashDevice::FlashDevice(message_router_id_t id):BLE::BleRemoteDevice(),fsm(id),
gatts_if(0),
flashVersion(0xFFFFFFFF)
{
}


void FlashDevice::initFlashService(void)
{
// ESP_LOGI(FLASHING_SERVICE_NAME, " Start creating");
	GattCharacteristic tempChr;
	GattDescriptor     tempDesc;
	memset(this->flashService.serviceInfo.id.uuid.uuid.uuid128,0,ESP_UUID_LEN_128);
	this->flashService.serviceInfo.id.uuid.len = ESP_UUID_LEN_16;
	this->flashService.serviceInfo.id.uuid.uuid.uuid16 = FLASHING_SERVICE_UUID;
	this->flashService.serviceInfo.is_primary = true;
	this->flashService.serviceInfo.id.inst_id = 0;

	tempChr.uuid.len = ESP_UUID_LEN_16;
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_VERSION_UUID;
	tempChr.control.auto_rsp = ESP_GATT_RSP_BY_APP;
	tempChr.property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE;
	tempChr.perm =  ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;
	tempChr.char_val.attr_len = 0;
	tempChr.char_val.attr_max_len = 0;
	tempChr.char_val.attr_value = 0;

	this->flashService.AddChr(&tempChr);
	// ESP_LOGI(FLASHING_SERVICE_NAME, "Add chr");
	tempChr.uuid.len = ESP_UUID_LEN_16;
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_DATABLOCK_UUID;
	tempChr.char_val.attr_len = 0;
	tempChr.char_val.attr_max_len = 0;
	tempChr.char_val.attr_value = 0;

	tempDesc.uuid.len = ESP_UUID_LEN_16;
	tempDesc.uuid.uuid.uuid16 = FLASHING_DESC_BLOCKSTATUS_UUID;
	tempDesc.perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;
	tempDesc.control.auto_rsp = ESP_GATT_RSP_BY_APP;
	tempDesc.desc_val.attr_len = 0;
	tempDesc.desc_val.attr_max_len = 0;
	tempDesc.desc_val.attr_value = 0;

	tempChr.AddDesc(&tempDesc);

	// ESP_LOGI(FLASHING_SERVICE_NAME, "Add desc");
	this->flashService.AddChr(&tempChr);
	// ESP_LOGI(FLASHING_SERVICE_NAME, "Add chr");
	tempChr.noOfRegistredDesc = 0;
	tempChr.uuid.len = ESP_UUID_LEN_16;
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_ISDEVPREP_UUID;
	tempChr.char_val.attr_len = 0;
	tempChr.char_val.attr_max_len = 0;
	tempChr.char_val.attr_value = 0;

	this->flashService.AddChr(&tempChr);


	tempChr.noOfRegistredDesc = 0;
	tempChr.uuid.len = ESP_UUID_LEN_16;
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_FILESIZE_UUID;
	tempChr.char_val.attr_len = 0;
	tempChr.char_val.attr_max_len = 0;
	tempChr.char_val.attr_value = 0;
	this->flashService.AddChr(&tempChr);
	// ESP_LOGI(FLASHING_SERVICE_NAME, "Add chr");
	this->flashService.numHandles = FLASHING_NUM_OF_HANDLES;
	// ESP_LOGI(FLASHING_SERVICE_NAME, "Created");
}

void FlashDevice::processReadAttr(esp_bt_uuid_t *uuid,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long)
{
	if(uuid->len == ESP_UUID_LEN_16)
	{
		if(uuid->uuid.uuid16 == FLASHING_CHR_VERSION_UUID)
		{
			esp_log_buffer_hex(FLASH_DEVICE, (uint8_t*)&this->flashVersion, sizeof(uint32_t));
			this->flashService.SendResponse(gatts_if,conn_id,trans_id,handle,offset,is_long,(uint8_t*)&this->flashVersion,sizeof(uint32_t));
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_DATABLOCK_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_ISDEVPREP_UUID)
		{
			uint8_t status = (this->get_state_id() == StateId::UPDATING) ? FLASH_UPDATE_STATUS_READY : FLASH_UPDATE_STATUS_NOTREADY;
			this->flashService.SendResponse(gatts_if,conn_id,trans_id,handle,offset,is_long,(uint8_t*)&status,1u);
		}
		else if(uuid->uuid.uuid16 == FLASHING_DESC_BLOCKSTATUS_UUID)
		{
			this->flashService.SendResponse(gatts_if,conn_id,trans_id,handle,offset,is_long,(uint8_t*)&this->blockStatus,1u);
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_FILESIZE_UUID)
		{

		}
	}
}


void FlashDevice::processWriteAttr(esp_bt_uuid_t *uuid, uint32_t trans_id,uint16_t handle,uint16_t offset, uint16_t len, uint8_t *val)
{
	if(uuid->len == ESP_UUID_LEN_16)
	{
		if(uuid->uuid.uuid16 == FLASHING_CHR_VERSION_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_DATABLOCK_UUID)
		{
			ReceivBlockEvt blockEvt(val,len);
			this->receive(blockEvt);
			this->flashService.SendResponse(this->gatts_if,this->conn_id,trans_id,handle,0,false,NULL,0);
			ESP_LOGI(FLASH_DEVICE, "Try to send resp on conn id %d", this->conn_id);			
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_ISDEVPREP_UUID)
		{
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_FILESIZE_UUID)
		{
			#define FLASH_DEV_SIZE_REQ 8u
			if(len == FLASH_DEV_SIZE_REQ)
			{
				uint32_t version = 0u;
				uint32_t fileSize = 0u;
				// Deserialize response
				version |= (uint32_t)(val[0]);
				version |= (uint32_t)(val[1]<<8);
				version |= (uint32_t)(val[2]<<16);
				version |= (uint32_t)(val[3]<<24);

				fileSize |= (uint32_t)(val[4]);
				fileSize |= (uint32_t)(val[5]<<8);
				fileSize |= (uint32_t)(val[6]<<16);
				fileSize |= (uint32_t)(val[7]<<24);
				ReceivFileSizeEvt evt(fileSize,version);
				this->receive(evt);

				this->flashService.SendResponse(this->gatts_if,this->conn_id,trans_id,handle,0,false,NULL,0);
				ESP_LOGI(FLASH_DEVICE, "Try to send resp on conn id %d", this->conn_id);
			}
		}
		else if(uuid->uuid.uuid16 == FLASHING_DESC_BLOCKSTATUS_UUID)
		{
			if(len == 1)
			{
				if(val[0] == (uint8_t)BLOCK_STATUS_FLASHED_APROVED)
				{
					ESP_LOGI(FLASH_DEVICE, "GW approved flashing");
					this->flashService.SendResponse(this->gatts_if,this->conn_id,trans_id,handle,0,false,NULL,0);
					this->nvsAdapter.Write(VERSION_KEY,this->new_version);
					this->ota.SetNewPartition();
					// change to other partition
					// clear nvs and ota info
				}
				else if(val[0] == (uint8_t)BLOCK_STATUS_FLASHED_NOTAPROVED)
				{
					ESP_LOGI(FLASH_DEVICE, "GW approved flashing");
					// clear nvs and ota info
				}
			}
			else
			{
				ESP_LOGE(FLASH_DEVICE, "Incorrect resp event");
			}
			this->flashService.SendResponse(this->gatts_if,this->conn_id,trans_id,handle,0,false,NULL,0);
			ESP_LOGI(FLASH_DEVICE, "Try to send resp on conn id %d", this->conn_id);
			
		}
	}
}


void FlashDevice::Init(void)
{
	esp_err_t ret;
	ret = this->nvsAdapter.Read(VERSION_KEY, &this->flashVersion);
	if(ret != ESP_OK)
	{
		ESP_LOGI(FLASH_DEVICE, "No version saved before in nvs");
		this->flashVersion = 0xFFFFFFFF;
		ret = nvsAdapter.Write(VERSION_KEY, this->flashVersion);
		if(ret != ESP_OK)
		{
			ESP_LOGE(FLASH_DEVICE, "Failed to write default version in nvs");
		}
	}
	ESP_LOGI(FLASH_DEVICE, "Current version 0x%08x", this->flashVersion);
	this->initFlashService();
	this->set_states(this->stateList, StateId::MAX_STATES);
	this->start(false);
	this->ota.Init();
}

void FlashDevice::UpdateNewImage(uint8_t *data, uint32_t len)
{
	if(this->receivedFileLen == 0u)
	{
		this->ota.StartUpdate();
	}
	this->ota.UpdateImage(data,len);
	this->receivedFileLen += len;
	ESP_LOGI(FLASH_DEVICE, "Received len %d", len);
	ESP_LOGI(FLASH_DEVICE, "Received totatl len %d", this->receivedFileLen);
	ESP_LOGI(FLASH_DEVICE, "File size %d", this->fileSize);
	if(this->receivedFileLen == this->fileSize)
	{
		this->blockStatus = BLOCK_STATUS_ALL_FLASHED;
		this->ota.EndUpdate();
	}
	else if(this->receivedFileLen < this->fileSize)
	{
		this->blockStatus = BLOCK_STATUS_INPROGRESS;
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "Too much flashed data %d", this->receivedFileLen);
	}
	

}


void FlashDevice::NotifyReadChr(uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long,  bool need_rsp)
{
	esp_bt_uuid_t *uuid = NULL;

	uuid = this->flashService.GetAttrUuidByHandle(handle);

	if(uuid != NULL)
	{
		this->processReadAttr(uuid,trans_id,handle,offset,is_long);
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "Failed to find handle %d", handle);
	}
	

}


void FlashDevice::NotifyWriteChr(uint32_t trans_id,uint16_t handle,uint16_t offset,  bool need_rsp, uint16_t len, uint8_t *val)
{
	esp_bt_uuid_t *uuid = NULL;
	uuid = this->flashService.GetAttrUuidByHandle(handle);
	if(uuid != NULL)
	{
		this->processWriteAttr(uuid,trans_id,handle,offset,len,val);
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "Failed to find handle %d", handle);
	}
}


void FlashDevice::StartAdvertising(void)
{
	uint8_t data[2u];
	data[0] = (uint8_t)this->magicNumber;
	data[1] = (uint8_t)(this->magicNumber >> 8u);
	this->gapAdapter.StartAdvertising(data,2u);
}



void FlashDevice::UpdateConectionStatus(esp_gatt_status_t status)
{
	this->status = status;
}

void FlashDevice::UpdateConectionParams(uint16_t connId, uint8_t *remoteAddr)
{
	this->conn_id = connId;
	memcpy(this->bda, remoteAddr, ESP_BD_ADDR_LEN);
}

void FlashDevice::RegisterService()
{
	this->flashService.RegisterService(this->gatts_if);
}

bool FlashDevice::NotifyServiceRegistred(esp_gatt_srvc_id_t *serviceInfo,uint16_t service_handle)
{
	return this->flashService.NotifyServiceRegistred(serviceInfo,service_handle);
}

bool FlashDevice::NotifyAttrRegistred(uint16_t attr_handle,uint16_t service_handle,esp_bt_uuid_t *uuid)
{
	return this->flashService.NotifyAttrRegistred(attr_handle,service_handle,uuid);
}
void FlashDevice::CreateService(void)
{
	this->flashService.CreateService();
}

void FlashDevice::PrintService(void)
{
	this->flashService.PrintService();
}

void FlashDevice::StartService(void)
{
	this->flashService.StartService();
}


FlashDevice::~FlashDevice()
{

}



FlashDevice::FlashDevice(const FlashDevice& flashDev):BLE::BleRemoteDevice(flashDev),fsm(flashDev)
{

	this->flashVersion = flashDev.flashVersion;
	this->nvsAdapter = flashDev.nvsAdapter;
}

void FlashDevice::LogUnknownEvent(const etl::imessage& msg)
{
	ESP_LOGE(FLASH_DEVICE, "Current state %d, message id %d", this->get_state_id(), msg.message_id);
}