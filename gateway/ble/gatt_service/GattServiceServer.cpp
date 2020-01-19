/*
 * GattServiceServer.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: ion
 */




#include "GattServiceServer.h"
#include "GattService.h"
#include "string.h"
#include "esp_log.h"
#include "esp_err.h"
using namespace BLE;

GattServiceServer::GattServiceServer():GattService()
{

}
GattServiceServer::~GattServiceServer()
{

}
void GattServiceServer::CreateService(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
	for(i = 0; i < this->noOfRegistredChrs; i++)
	{
		this->gatts.AddChar(this->serviceHandle,
				&this->listOfChr[i].uuid,
				this->listOfChr[i].perm,
				this->listOfChr[i].property,
				&this->listOfChr[i].char_val,
				&this->listOfChr[i].control);


		for(j = 0; j < this->listOfChr[i].noOfRegistredDesc; j++)
		{
			this->gatts.AddCharDescr(this->serviceHandle,
							&this->listOfChr[i].listOfDesc[j].uuid,
							this->listOfChr[i].listOfDesc[j].perm,
							&this->listOfChr[i].listOfDesc[j].desc_val,
							&this->listOfChr[i].listOfDesc[j].control);
		}
	}
}


bool GattServiceServer::NotifyServiceRegistred(esp_gatt_srvc_id_t *serviceInfo,uint16_t service_handle) // @suppress("Member declaration not found")
{
	bool ret;
	ret = GattService::IsUuidEqual(&this->serviceInfo.id.uuid, &serviceInfo->id.uuid);
	if(ret == true)
	{
		memcpy(&this->serviceInfo,serviceInfo,sizeof(esp_gatt_srvc_id_t));
		this->serviceHandle = service_handle;
	}
	return ret;
}
bool GattServiceServer::NotifyAttrRegistred(uint16_t attr_handle,uint16_t service_handle,esp_bt_uuid_t *uuid) // @suppress("Member declaration not found")
{
	bool ret = false;
	uint8_t i,j;
	if(this->serviceHandle == service_handle)
	{
		for(i = 0; i < this->noOfRegistredChrs; i++)
		{
			ret = GattService::IsUuidEqual(&this->listOfChr[i].uuid, uuid);
			if(ret == true)
			{
				this->listOfChr[i].chr_handle = attr_handle;
				break;
			}
			else
			{
				for(j = 0; j < this->listOfChr[i].noOfRegistredDesc; j++)
				{
					ret = GattService::IsUuidEqual(&this->listOfChr[i].listOfDesc[j].uuid, uuid);
					if(ret == true)
					{
						this->listOfChr[i].listOfDesc[j].desc_handle = attr_handle;
						break;
					}
				}
				if(ret == true)
				{
					break;
				}
			}
		}
	}
	return ret;
}

void GattServiceServer::SendIndication(esp_gatt_if_t gatts_if, uint16_t conn_id, uint16_t attr_handle, uint16_t value_len, uint8_t *value, bool need_confirm)
{
	this->gatts.SendIndication( gatts_if,  conn_id,  attr_handle,  value_len,  value,  need_confirm);
	ESP_LOGI(GATT_SERVICE_NAME,"Send indication with conn id %d attr_handle %d, len %d, need_conf %d", conn_id,attr_handle,value_len,need_confirm);
	esp_log_buffer_hex(GATT_SERVICE_NAME,value,value_len);
}

void GattServiceServer::StartService(void)
{
	this->gatts.StartService(this->serviceHandle);
}

void GattServiceServer::SendResponse(esp_gatt_if_t gatts_if, uint16_t  conn_id,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long, uint8_t *value, uint16_t value_len )
{
	if(is_long != true)
	{
		this->respValue.attr_value.len = value_len;
		memcpy((void*)this->respValue.attr_value.value,(void*)value,value_len);
		this->respValue.attr_value.offset = 0;
		this->respValue.attr_value.handle = handle;
		this->respValue.attr_value.auth_req = 0;
		esp_log_buffer_hex(GATT_SERVICE_NAME, &this->respValue.attr_value.value[0], this->respValue.attr_value.len);
		this->gatts.SendResponse(gatts_if,conn_id,trans_id, ESP_GATT_OK, &this->respValue);
	}
	else
	{
		ESP_LOGI(GATT_SERVICE_NAME,"Response is long. Failed with error.");
	}
}


bool GattServiceServer::RegisterService(esp_gatt_if_t gatts_if)
{
	return (this->gatts.RegisterService(gatts_if,this) == ESP_OK) ? true : false;
}
GattServiceServer::GattServiceServer(const GattServiceServer &serServer):GattService(serServer)
{

}

GattServiceServer& GattServiceServer::operator=(const GattServiceServer& gattsServerService)
{
	memcpy(&this->respValue,&gattsServerService.respValue, sizeof(esp_gatt_rsp_t));
	GattService::operator=(gattsServerService);
	return (*this);
}