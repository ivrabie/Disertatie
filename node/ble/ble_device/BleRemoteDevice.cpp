
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
//static BLE_DeviceInfo ble_deviceDetected[BLE_DEVICE_NUMBER];
//static uint8_t ble_no_devices = 0;
//
//static bool bleDevice_CheckRegisterDevices(esp_bd_addr_t bda);
//
//void BLEDEVICE_UpdateDevice(uint16_t value, esp_bd_addr_t bda, BLE_DEVICEINFO_FIELD_TYPE field_type)
//{
//	uint8_t  i = 0;
//	for(i = 0; i < ble_no_devices; i++)
//	{
//		if(memcmp(ble_deviceDetected[i].bda, bda, ESP_BD_ADDR_LEN) == 0)
//		{
//			switch(field_type)
//			{
//			case BLE_DEVICEINFO_FIELD_CONECTION_STATUS:
//				ble_deviceDetected[i].status = value;
//				break;
//			case BLE_DEVICEINFO_FIELD_CONECTION_ID:
//				ble_deviceDetected[i].conn_id = value;
//				break;
//			case BLE_DEVICEINFO_FIELD_CONECTION_MTU:
//				ble_deviceDetected[i].mtu = value;
//				break;
//			}
//		}
//
//	}
//}
//
//void BLEDEVICE_RegisterDevice(BLE_DeviceInfo *device)
//{
//	if((bleDevice_CheckRegisterDevices(device->bda) == false) && (ble_no_devices <= BLE_DEVICE_NUMBER))
//	{
//				memcpy(&ble_deviceDetected[ble_no_devices],device,sizeof(BLE_DeviceInfo));
//				ble_no_devices += 1u;
//				ESP_LOGI(BLE_DEVICE_NAME, "Device registered");
//				ESP_LOGI(BLE_DEVICE_NAME, "Device name: %s \n"
//						"Adv version %d\n",
//						device->deviceName,
//						device->version);
//	}
//	else
//	{
//		ESP_LOGE(BLE_DEVICE_NAME, "Max number of registred device or device already exists");
//	}
//}
//
//void BLEDEVICE_UpdateDeviceByConnId(uint16_t value, uint16_t conn_id, BLE_DEVICEINFO_FIELD_TYPE field_type)
//{
//	uint8_t  i = 0;
//	for(i = 0; i < ble_no_devices; i++)
//	{
//		if(ble_deviceDetected[i].conn_id == conn_id)
//		{
//			switch(field_type)
//			{
//			case BLE_DEVICEINFO_FIELD_CONECTION_STATUS:
//				ble_deviceDetected[i].status = value;
//				break;
//			case BLE_DEVICEINFO_FIELD_CONECTION_ID:
//				ble_deviceDetected[i].conn_id = value;
//				break;
//			case BLE_DEVICEINFO_FIELD_CONECTION_MTU:
//				ble_deviceDetected[i].mtu = value;
//				break;
//			}
//		}
//
//	}
//}
//
//
//static bool bleDevice_CheckRegisterDevices(esp_bd_addr_t bda)
//{
//	bool ret = false;
//	uint8_t i = 0;
//
//	for(i = 0; i < ble_no_devices; i++)
//	{
//		if(memcmp(ble_deviceDetected[i].bda,bda,ESP_BD_ADDR_LEN) == 0)
//		{
//			ret = true;
//			break;
//		}
//	}
//	return ret;
//}

