/*
 * FlashingService.h
 *
 *  Created on: Aug 11, 2019
 *      Author: ion
 */

#ifndef SWLOAD_FLASHINGSERVICE_FLASHINGSERVICE_H_
#define SWLOAD_FLASHINGSERVICE_FLASHINGSERVICE_H_

#include "../../ble/gatt_service/GattServiceServer.h"
#include <stdint.h>

using namespace BLE;
namespace SWLOAD
{
#define FLASHING_SERVICE_NAME "FLS"
	class FlashinService:public GattServiceServer
	{
#define FLASHING_SERVICE_UUID 0xF000
#define FLASHING_CHR_VERSION_UUID  0xF100
#define FLASHING_CHR_DATABLCOK_UUID 0xF200
#define FLASHING_DESC_BLOCKIDX_UUID 0xF201
#define FLASHING_DESC_BLOCKCRC_UUID 0xF202
#define FLASHING_CHR_REQBLOCK_UUID  0xF300
#define FLASHING_CHR_BLOCK_SIZE  10u
#define FLASHING_NUM_OF_HANDLES 20u

	private:
//		uint16_t chrVersion = 0;
//		uint8_t  chrDataBlock[FLASHING_CHR_BLOCK_SIZE];
//		uint16_t descDataBlockIdx = 0;
//		uint32_t descDataBlockCrc = 0;
//		uint16_t chrDataBlockReq  = 0;
		uint16_t searchAttrHandle(esp_bt_uuid_t *uuid);
		void processReadChr(esp_bt_uuid_t *uuid, esp_gatt_if_t gatts_if,uint16_t conn_id,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long);
	public:
		FlashinService();
		~FlashinService();
		FlashinService(const FlashinService &flashService);
		void NotifyReadChr(esp_gatt_if_t gatts_if,uint16_t conn_id,uint32_t trans_id,uint16_t handle,uint16_t offset,bool is_long,  bool need_rsp);
		void RequestBlock(esp_gatt_if_t gatts_if,uint16_t conn_id);
	};
}


#endif /* SWLOAD_FLASHINGSERVICE_FLASHINGSERVICE_H_ */
