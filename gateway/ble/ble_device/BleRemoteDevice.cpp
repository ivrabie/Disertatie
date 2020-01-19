
/*
 * ble_device.c
 *
 *  Created on: Aug 1, 2019
 *      Author: ion
 */

#include <esp_log.h>
#include <string.h>


#include "BleRemoteDevice.h"
#include "BleRemoteDevice_cfg.h"





using namespace BLE;

BleRemoteDevice::BleRemoteDevice()
{
	this->ble_addr_type = (esp_ble_addr_type_t)0xFF;
	this->conn_id = 0xFFFF;
	this->mtu = 0xFFFF;
	this->status = (esp_gatt_status_t)0xFF;
	memset(this->bda, 0xFF, ESP_BD_ADDR_LEN);
	memset(this->deviceName, 0xFF, BLE_DEVICEINFO_EXPECTED_LEN_NAME);
}



BleRemoteDevice::~BleRemoteDevice()
{
	this->ble_addr_type = (esp_ble_addr_type_t)0xFF;
	this->conn_id = 0xFFFF;
	this->mtu = 0xFFFF;
	this->status = (esp_gatt_status_t)0xFF;
	memset(this->bda, 0xFF, ESP_BD_ADDR_LEN);
	memset(this->deviceName, 0xFF, BLE_DEVICEINFO_EXPECTED_LEN_NAME);

}

BleRemoteDevice::BleRemoteDevice(const BleRemoteDevice &bleRemoteDev)
{
	if(bleRemoteDev.bda != NULL)
	{
		memcpy(this->bda,bleRemoteDev.bda, ESP_BD_ADDR_LEN);
	}
	if(bleRemoteDev.deviceName != NULL)
	{
		memcpy(this->deviceName,bleRemoteDev.deviceName, BLE_DEVICEINFO_EXPECTED_LEN_NAME);
	}

	this->ble_addr_type = bleRemoteDev.ble_addr_type;
	this->conn_id = bleRemoteDev.conn_id;
	this->mtu = bleRemoteDev.mtu;
	this->status = bleRemoteDev.status;


}


BleRemoteDevice& BleRemoteDevice::operator=(const BleRemoteDevice &dev)
{
	this->conn_id = dev.conn_id;
	this->mtu = this->mtu;
	this->status = dev.status;
	this->ble_addr_type = dev.ble_addr_type;
	memcpy(this->bda,dev.bda,ESP_BD_ADDR_LEN);
	memcpy(this->deviceName,dev.deviceName,BLE_DEVICEINFO_EXPECTED_LEN_NAME);
	return (*this);
}