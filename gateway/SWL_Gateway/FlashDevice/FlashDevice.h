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
#include "../../ble/gatt_service/GattServiceClient.h"
#include "../../etl/fsm.h"
#include "../../etl/message_types.h"
#include "FlashStateMachine/FlashStates.h"
#include "FlashStateMachine/FlashEvents.h"
#include "../VersionManager/VersionManager.h"
#include "../../sdcard/sdcard.h"
using namespace BLE;
using namespace etl;
using namespace SDCARD;
namespace SWL_GATEWAY
{
typedef enum
{
	DEVICE_UPDATEFIELD_CONECTION_ID,
	DEVICE_UPDATEFIELD_CONECTION_STATUS,
	DEVICE_UPDATEFIELD_CONECTION_MTU
}DEVICE_UPDATEFIELD_TYPES;

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

#define FLASH_DEVICE "FlashDevice"



	class FlashDevice:public BLE::BleRemoteDevice,public etl::fsm
	{
#define FLASHING_SERVICE_UUID 0xF000
#define FLASHING_CHR_VERSION_UUID  0xF100
#define FLASHING_CHR_DATABLCOK_UUID 0xF200
#define FLASHING_DESC_BLOCKSTATUS_UUID 0xF201
#define FLASHING_CHR_ISDEVPREP_UUID  0xF300
#define FLASHING_CHR_FILESIZE_UUID  0xF400
#define FLASHING_CHR_BLOCK_SIZE  10u
#define FLASHING_NUM_OF_HANDLES 20u
#define FLASH_BLOCK_SIZE  500u


#define FLASH_DEFAULT 0xFFFFFFFu


	private:
		void processReadAttrEvent(esp_bt_uuid_t *uuid, uint8_t *value,uint16_t value_len);
		void processWriteAttrEvent(esp_bt_uuid_t *uuid);
		void initFlashService(void);
		char* getFileName();
	public:

	 	VersionManager *vsMng = NULL;
		SdCard *sd = NULL;
		uint8_t flashStatus;
		uint8_t flashProgress;
		uint16_t flashMagicNumber;
		uint32_t flashFileSize = 0u;
		uint16_t blockNo = 0;
		uint8_t dataToSend[FLASH_BLOCK_SIZE];
		uint32_t dataLen = 0u;
		uint32_t currentLenSent = 0u;
		uint8_t gattc_if = 0u;
		GattServiceClient flashService;

		// The states.
		DisconnectedState  disconnectState;
		ConnectedState     connectState;
		ServerDiscoveredState serverDiscoveredState;
		VersionReceivedState versionReceivedState;
		SendBlockState sendBlockState;
		DeleteState deleteState;
		FlashedCompleteState flashCompleteState;
		ifsm_state* stateList[StateId::MAX_STATES] =
		{
		&this->disconnectState, &this->connectState, &this->serverDiscoveredState,
		&this->versionReceivedState, &this->sendBlockState, &this->deleteState, &flashCompleteState
		};
		FlashDevice(message_router_id_t id, VersionManager *vsMng, SdCard *sd,uint8_t gattc_if);
		FlashDevice(const FlashDevice& flashDev);
		FlashDevice& operator=(const FlashDevice &dev);

		void RequestVersion();
		void NotifyReadAttrEvent(uint16_t handle,uint8_t *value,uint16_t value_len);
		void NotifyWriteAttrEvent(uint16_t handle);
		void RegisterReqBlockForNotification();
		void NotifyGattServiceDiscoverComplete();
		void RefreshGattServiceCache(void);
		void UpdateField(uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);
		void OpenBleConnection();
		void InitStateMachine(void);
		void LogUnknownEvent(const etl::imessage& msg);
		uint32_t GetCurrentVersion();
		bool SendBlock();
		void SendFileSizeAndVersion();
		void RequestIsDevReady();
		void RequestBlockStatus();
		void ValidateFlashing();
		~FlashDevice();
		
	};
}

#endif

