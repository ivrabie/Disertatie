/*
 * FlashingService.h
 *
 *  Created on: Aug 11, 2019
 *      Author: ion
 */

#ifndef SWLOAD_FLASHINGSERVICE_FLASHINGSERVICE_H_
#define SWLOAD_FLASHINGSERVICE_FLASHINGSERVICE_H_

#include "../../ble/gatt_service/GattServiceClient.h"
#include <stdint.h>

using namespace BLE;
namespace SWL_GATEWAY
{
#define FLASHING_SERVICE_NAME "FLS"
	class FlashinService:public GattServiceClient
	{
#define FLASHING_SERVICE_UUID 0xF000
#define FLASHING_CHR_VERSION_UUID  0xF100
#define FLASHING_CHR_DATABLCOK_UUID 0xF200
#define FLASHING_DESC_BLOCKIDX_UUID 0xF201
#define FLASHING_DESC_BLOCKCRC_UUID 0xF202
#define FLASHING_CHR_REQBLOCK_UUID  0xF300
#define FLASHING_CHR_BLOCK_SIZE  10u
#define FLASHING_NUM_OF_HANDLES 20u

#define FLASHING_NOTIFICATION_TYPE 0u
#define FLASHING_READCHR_TYPE      1u
	private:
//		uint16_t chrVersion = 0;
//		uint8_t  chrDataBlock[FLASHING_CHR_BLOCK_SIZE];
//		uint16_t descDataBlockIdx = 0;
//		uint32_t descDataBlockCrc = 0;
//		uint16_t chrDataBlockReq  = 0;
		uint16_t searchAttrHandle(esp_bt_uuid_t *uuid);
		void processChrEvent(esp_bt_uuid_t *uuid, uint8_t *value,uint16_t value_len,uint8_t type);
	public:
		FlashinService(const FlashinService &flashService);
		FlashinService();
		~FlashinService();

		void RequestVersion(esp_gatt_if_t gattc_if, uint16_t conn_id);
		void NotifyChrEvent(uint16_t handle,uint8_t *value,uint16_t value_len,uint8_t type);
		void RegisterReqBlockForNotification(esp_gatt_if_t gattc_if, esp_bd_addr_t server_bda);


	};
}


#endif /* SWLOAD_FLASHINGSERVICE_FLASHINGSERVICE_H_ */
