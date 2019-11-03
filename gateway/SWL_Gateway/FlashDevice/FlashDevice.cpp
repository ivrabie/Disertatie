/*
 * flashingService.c
 *
 *  Created on: Jul 18, 2019
 *      Author: ion
 */

//#include <BleAdapter.hpp>
//#include <BleRemoteDevice.h>
#include <esp_gatts_api.h>
#include <esp_log.h>
#include "FlashDevice.h"
#include "FlashDevice_cfg.h"
#include <string.h>

#define CHAR_VAL_LEN_MAX 0x40
#define FLASH_VERS_RESP_LEN 4u
using namespace SWL_GATEWAY;
const etl::message_router_id_t FLASH_CONTROL = 0;
FlashDevice::FlashDevice(message_router_id_t id, VersionManager *vsMng,SdCard *sd,uint8_t gattc_if):
BLE::BleRemoteDevice(),fsm(id),vsMng(vsMng),sd(sd),gattc_if(gattc_if)
{
	this->flashProgress = 0xFF;
	this->flashStatus   = 0xFF;
	this->flashMagicNumber = 0xFFFF;
	this->flashFileSize = 0x0;
}

FlashDevice::~FlashDevice()
{

}



FlashDevice::FlashDevice(const FlashDevice& flashDev):BLE::BleRemoteDevice(flashDev),fsm(flashDev)
{
	this->flashService = flashDev.flashService;
	this->flashProgress = flashDev.flashProgress;
	this->flashStatus = flashDev.flashStatus;
	this->flashMagicNumber = flashDev.flashMagicNumber;
	this->flashFileSize = flashDev.flashFileSize;
	this->gattc_if = flashDev.gattc_if;
	this->sd = flashDev.sd;
	this->vsMng = flashDev.vsMng;
}

FlashDevice& FlashDevice::operator=(const FlashDevice &dev)
{
	this->flashService = dev.flashService;
	this->flashProgress = dev.flashProgress;
	this->flashStatus = dev.flashStatus;
	this->flashMagicNumber = dev.flashMagicNumber;
	this->flashFileSize = dev.flashFileSize;
	this->gattc_if = dev.gattc_if;
	this->vsMng = dev.vsMng;
	BleRemoteDevice::operator=(dev);
	fsm::operator=(dev);
	return (*this);
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
	tempChr.uuid.uuid.uuid16 = FLASHING_CHR_DATABLCOK_UUID;
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



void FlashDevice::RequestVersion()
{
	esp_bt_uuid_t uuid;
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_CHR_VERSION_UUID;

	this->flashService.RequestReadAttr(this->gattc_if,this->conn_id,&uuid);

}
void FlashDevice::NotifyReadAttrEvent(uint16_t handle,uint8_t *value,uint16_t value_len)
{
	esp_bt_uuid_t *uuid;
	uuid = this->flashService.GetAttrUuidByHandle(handle);
	if(uuid != NULL)
	{
		this->processReadAttrEvent(uuid, value, value_len);
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "Handle %d was not found", value_len);
	}
}

void FlashDevice::processReadAttrEvent(esp_bt_uuid_t *uuid, uint8_t *value,uint16_t value_len)
{
	if(uuid->len == ESP_UUID_LEN_16)
	{
		if(uuid->uuid.uuid16 == FLASHING_CHR_VERSION_UUID)
		{
			uint32_t version = 0u;
			ESP_LOGI(FLASH_DEVICE, "Value len %d", value_len);
			esp_log_buffer_hex(FLASH_DEVICE,value,value_len);

			if(value_len == FLASH_VERS_RESP_LEN)
			{
				version |= (uint32_t)(value[0]<<24u);
				version |= (uint32_t)(value[1]<<16u);
				version |= (uint32_t)(value[2]<<8u);
				version |= (uint32_t)(value[3]);
				VersionRespEvt versEvt(version);
				this->receive(versEvt);
			}
			else
			{
				// Todo create a new event for error case
				version = 0xFFFFFFFF;
				ESP_LOGE(FLASH_DEVICE, "Incorrect version resp len %d expected %d", value_len,FLASH_VERS_RESP_LEN);
			}
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_DATABLCOK_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_ISDEVPREP_UUID)
		{
			if(value_len == 1u)
			{
				if(value[0] == FLASH_UPDATE_STATUS_READY)
				{
					PrepReadyEvt evt;
					this->receive(evt);
				}
			}
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_FILESIZE_UUID)
		{
			
		}
		else if(uuid->uuid.uuid16 == FLASHING_DESC_BLOCKSTATUS_UUID)
		{
			if(value_len == 1u)
			{
				if(value[0] == (uint8_t)BLOCK_STATUS_INPROGRESS)
				{
					NextBlockEvt evt;
					this->receive(evt);
				}
				else if(value[0] == BLOCK_STATUS_ALL_FLASHED)
				{
					FlashedCompleteEvt flashedCompleteEvt;
					this->receive(flashedCompleteEvt);
				}
			}
		}
	}
}




void FlashDevice::processWriteAttrEvent(esp_bt_uuid_t *uuid)
{
	if(uuid->len == ESP_UUID_LEN_16)
	{
		
		if(uuid->uuid.uuid16 == FLASHING_CHR_VERSION_UUID)
		{

		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_DATABLCOK_UUID)
		{
			CheckBlockStatusEvt blockStatusEvt;
			this->receive(blockStatusEvt);
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_ISDEVPREP_UUID)
		{
			
		}
		else if(uuid->uuid.uuid16 == FLASHING_CHR_FILESIZE_UUID)
		{
			CheckDevReadyEvt devReadyEvt;
			this->receive(devReadyEvt);
		}
		else if(uuid->uuid.uuid16 == FLASHING_DESC_BLOCKSTATUS_UUID)
		{
			FlashedCompleteRespEvt flashedCompleteEvt;
			this->receive(flashedCompleteEvt);
		}
	}
}

void FlashDevice::RequestIsDevReady()
{
	esp_bt_uuid_t uuid;
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_CHR_ISDEVPREP_UUID;
	this->flashService.RequestReadAttr(this->gattc_if,this->conn_id,&uuid);
}

void FlashDevice::NotifyWriteAttrEvent(uint16_t handle)
{
	esp_bt_uuid_t *uuid;
	uuid = this->flashService.GetAttrUuidByHandle(handle);
	if(uuid != NULL)
	{
		this->processWriteAttrEvent(uuid);
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "Handle %d was not found", handle);
	}

}

void FlashDevice::RequestBlockStatus()
{
	esp_bt_uuid_t desc_uuid;
	desc_uuid.len = ESP_UUID_LEN_16;
	desc_uuid.uuid.uuid16 = FLASHING_DESC_BLOCKSTATUS_UUID;
	this->flashService.RequestReadAttr(this->gattc_if,this->conn_id,&desc_uuid);
}

void FlashDevice::UpdateField(uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type)
{
	switch(field_type)
	{
	case DEVICE_UPDATEFIELD_CONECTION_ID:
		this->conn_id = field_value;
		break;
	case DEVICE_UPDATEFIELD_CONECTION_STATUS:
		this->status = (esp_gatt_status_t)field_value;
		break;
	case DEVICE_UPDATEFIELD_CONECTION_MTU:
		this->mtu = field_value;
		break;
	}
}


void FlashDevice::NotifyGattServiceDiscoverComplete()
{
	this->flashService.NotifyDiscoverComplete(this->gattc_if,this->conn_id);
	if(this->flashService.IsServiceInit() == true)
	{
		ServiceDiscoveredEvt srvEvt;
		this->receive(srvEvt);
	}
}

void FlashDevice::RefreshGattServiceCache(void)
{
	this->flashService.RefreshServiceInfo(this->bda);
}

void FlashDevice::OpenBleConnection()
{
	if(this->get_state_id() == StateId::DISCONNECTED)
	{
		this->initFlashService();
		this->flashService.OpenConnection(this->gattc_if,this->bda,this->ble_addr_type,true);
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "Current state %d, already connected", this->get_state_id());
	}
	

}

void FlashDevice::InitStateMachine(void)
{
	this->set_states(this->stateList, StateId::MAX_STATES);
	this->start(false);
}

void FlashDevice::LogUnknownEvent(const etl::imessage& msg)
{
	ESP_LOGE(FLASH_DEVICE, "Current state %d, message id %d", this->get_state_id(), msg.message_id);
}

uint32_t FlashDevice::GetCurrentVersion()
{
	uint32_t version = FLASH_DEFAULT;
	if(this->vsMng == NULL)
	{
		ESP_LOGE(FLASH_DEVICE, "Version manager is NULL");
	}
	else
	{
		version = vsMng->GetLatestVersion();
	}
	return version;
}

char* FlashDevice::getFileName()
{
	char *fileName = NULL;
	if(this->sd != NULL)
	{
		fileName = this->vsMng->GetUpdateFileName();
		if(fileName == NULL)
		{
			ESP_LOGE(FLASH_DEVICE, "File name is NULL");
		}
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "SdCard is NULL");
	}

	return fileName;
}
void FlashDevice::SendFileSizeAndVersion()
{
	char *fileName = this->getFileName();
	esp_bt_uuid_t uuid;
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_CHR_FILESIZE_UUID;
	uint8_t data[8u];
	if(fileName != NULL)
	{
		this->flashFileSize = this->sd->GetFileLen(fileName);
		if(flashFileSize != 0)
		{
			uint32_t version = this->GetCurrentVersion();
			data[0] = (uint8_t)version;
			data[1] = (uint8_t)(version>>8u);
			data[2] = (uint8_t)(version>>16u);
			data[3] = (uint8_t)(version>>24u);

			data[4] = (uint8_t)this->flashFileSize;
			data[5] = (uint8_t)(this->flashFileSize>>8u);
			data[6] = (uint8_t)(this->flashFileSize>>16u);
			data[7] = (uint8_t)(this->flashFileSize>>24u);
			ESP_LOGI(FLASH_DEVICE, "Request for write version %d with len %d",version,this->flashFileSize);
			this->flashService.RequestWriteAttr(this->gattc_if,this->conn_id,&uuid,8u,data,ESP_GATT_WRITE_TYPE_RSP,ESP_GATT_AUTH_REQ_NONE);
		}
		else
		{
			ESP_LOGE(FLASH_DEVICE, "File to flash is not present or empty");
		}
	}
}

bool FlashDevice::SendBlock()
{
	bool ret = false;
	char *fileName = NULL;
	esp_bt_uuid_t uuid;
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_CHR_DATABLCOK_UUID;
	if(this->sd != NULL)
	{
		fileName = this->vsMng->GetUpdateFileName();
		if(fileName != NULL)
		{
			this->dataLen = this->sd->ReadBinaryFile(fileName,this->dataToSend,this->currentLenSent,FLASH_BLOCK_SIZE);
			if(this->dataLen != 0)
			{
				this->currentLenSent += this->dataLen;
				ESP_LOGI(FLASH_DEVICE, "File size read %d", this->dataLen);
				ESP_LOGI(FLASH_DEVICE, "File size current read %d", this->currentLenSent);
				this->flashService.RequestWriteAttr(this->gattc_if,
				this->conn_id,
				&uuid,
				this->dataLen,
				this->dataToSend,
				ESP_GATT_WRITE_TYPE_RSP,
				ESP_GATT_AUTH_REQ_NONE);
				ret = true;
			}
			else
			{
				ESP_LOGE(FLASH_DEVICE, "Flashed should already completed");
			}
		}
		else
		{
			ESP_LOGE(FLASH_DEVICE, "File name is NULL");
		}
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "SdCard is NULL");
	}
	return ret;
}


void FlashDevice::ValidateFlashing()
{
	uint8_t resp;
	esp_bt_uuid_t uuid;
	uuid.len = ESP_UUID_LEN_16;
	uuid.uuid.uuid16 = FLASHING_DESC_BLOCKSTATUS_UUID;
	if(this->flashFileSize == this->currentLenSent)
	{
		ESP_LOGI(FLASH_DEVICE, "Flashed approved by GW");
		resp = (uint8_t)BLOCK_STATUS_FLASHED_APROVED;
	}
	else
	{
		ESP_LOGE(FLASH_DEVICE, "Flashed not approved by GW");
		resp = (uint8_t)BLOCK_STATUS_FLASHED_NOTAPROVED;
	}
	this->flashService.RequestWriteAttr(this->gattc_if,
	this->conn_id,
	&uuid,
	1u,
	&resp,
	ESP_GATT_WRITE_TYPE_RSP,
	ESP_GATT_AUTH_REQ_NONE);

}