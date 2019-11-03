/*
 * flashingService.h
 *
 *  Created on: Jul 18, 2019
 *      Author: ion
 */

#ifndef BLE_FLASHINGPROFILE_FLASHINGPROFILE_H_
#define BLE_FLASHINGPROFILE_FLASHINGPROFILE_H_


#include "esp_bt_defs.h"
#include "../../ble/ble_device/BleRemoteDevice.h"
#include "esp_log.h"
#include "../../ble/gatt_service/GattServiceServer.h"
#include "../../nvs_adapter/NvsAdapter.h"
#include "../../ble/advertising/GapAdapter.h"
#include "FlashStateMachine/FlashStates.h"
#include "../../ota_adapter/OtaAdapter.h"
using namespace NVS_ADAPTER;
using namespace BLE;
using namespace OTA_ADAPTER;
namespace SWLOAD
{
	
	#define FLASH_DEVICE "FlashDevice"
	#define VERSION_KEY "version"
	class FlashDevice:public BLE::BleRemoteDevice,public etl::fsm
	{

typedef enum
{
	FLASH_UPDATE_STATUS_READY,
	FLASH_UPDATE_STATUS_NOTREADY
}FLASH_UPDATE_STATUS;


typedef enum
{
	BLOCK_STATUS_NOT_INIT,
	BLOCK_STATUS_INPROGRESS,
	BLOCK_STATUS_ALL_FLASHED,
	BLOCK_STATUS_FLASHED_APROVED,
	BLOCK_STATUS_FLASHED_NOTAPROVED
}BlockStatusType;

#define FLASHING_SERVICE_UUID 0xF000
#define FLASHING_CHR_VERSION_UUID  0xF100
#define FLASHING_CHR_DATABLOCK_UUID 0xF200
#define FLASHING_DESC_BLOCKSTATUS_UUID 0xF201
#define FLASHING_CHR_ISDEVPREP_UUID  0xF300
#define FLASHING_CHR_FILESIZE_UUID  0xF400
#define FLASHING_CHR_BLOCK_SIZE  10u
#define FLASHING_NUM_OF_HANDLES 20u
#define SWLOAD_MAGIC_NUMBER 0xA5A5u
	private:
		void initFlashService(void);
		void processReadAttr(esp_bt_uuid_t *uuid,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long);
		void processWriteAttr(esp_bt_uuid_t *uuid, uint32_t trans_id,uint16_t handle,uint16_t offset, uint16_t len, uint8_t *val);
	public:
		uint8_t gatts_if;
		uint16_t magicNumber = SWLOAD_MAGIC_NUMBER;
		uint32_t flashVersion;
		uint32_t new_version;
		uint32_t fileSize;
		bool isDeviceReady;
		BlockStatusType blockStatus = BLOCK_STATUS_NOT_INIT;
		NVSAdapter &nvsAdapter = NVSAdapter::getInstance();
		GapAdapter &gapAdapter = GapAdapter::getInstance();
		OtaAdapter &ota = OtaAdapter::getInstance();  
		GattServiceServer flashService;

		uint32_t receivedFileLen = 0;

		// States
		DisconnectedState dissconectedState;
		ConnectedState connectedState;
		UpdatingState updatingState;
		ifsm_state* stateList[StateId::MAX_STATES] =
		{
			&this->dissconectedState, &this->connectedState, &this->updatingState
		};
		FlashDevice(message_router_id_t id);
		FlashDevice(const FlashDevice& flashDev);
		~FlashDevice();
		void NotifyReadChr(uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long,  bool need_rsp);
		void NotifyWriteChr(uint32_t trans_id,uint16_t handle,uint16_t offset,  bool need_rsp, uint16_t len, uint8_t *val);
		void StartAdvertising(void);
		void UpdateConectionParams(uint16_t connId, uint8_t *remoteAddr);
		void UpdateConectionStatus(esp_gatt_status_t status);
		void RegisterService();
		bool NotifyServiceRegistred(esp_gatt_srvc_id_t *serviceInfo,uint16_t service_handle); // @suppress("Member declaration not found")
		void CreateService(void);
		bool NotifyAttrRegistred(uint16_t attr_handle,uint16_t service_handle,esp_bt_uuid_t *uuid); // @suppress("Member declaration not found")
		void PrintService(void);
		void StartService(void);
		void UpdateNewImage(uint8_t *data, uint32_t len);
		void Init(void);
		void LogUnknownEvent(const etl::imessage& msg);
	};
}

#endif

//
//
//
//void FlashProfile_Init(void);
//
//
//#endif /* BLE_FLASHINGPROFILE_FLASHINGPROFILE_H_ */
