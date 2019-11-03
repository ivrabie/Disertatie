/*
 * FlashDeviceManager.h
 *
 *  Created on: Aug 6, 2019
 *      Author: ion
 */

#ifndef SWL_GATEWAY_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_
#define SWL_GATEWAY_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_

#include "../FlashDevice/FlashDevice.h"
#include <stdint.h>
#include <esp_bt_defs.h>
namespace SWL_GATEWAY
{

#define MAX_BLE_DEVICES 5u
#define MAX_PING_PONG_BUFF 2u
typedef enum
{
	EVENT_TYPE_READ,
	EVENT_TYPE_WRITE,
}EventType;
#define MAX_EVENT_INFO_SIZE 100u

struct EventInfo {
	uint16_t conn_id;
	uint16_t handle;		
	uint16_t val_len;
	EventType event_type;
	uint8_t val[MAX_EVENT_INFO_SIZE];
	EventInfo& operator=(const EventInfo &info)
	{
		this->conn_id = info.conn_id;
		this->handle = info.handle;
		this->val_len = info.val_len;
		memcpy(this->val,info.val,this->val_len);
		this->event_type = info.event_type;
		return (*this);
	}
};


struct BufferedInfo
{
	etl::vector<EventInfo, MAX_BLE_DEVICES> listOfEvents[MAX_PING_PONG_BUFF];
	uint8_t currentListIdxUsed;
};
#define FLASHDEVICEMANAGER "FlashDeviceManager"
	class FlashDeviceManager
	{
	public:
			etl::vector<FlashDevice, MAX_BLE_DEVICES> listOfDevice;
			BufferedInfo bufferedEvents;
			FlashDeviceManager();
			~FlashDeviceManager();
			bool RegisterDevice(FlashDevice *flashService);
			void RemoveDevice(uint16_t conn_id);
			void RemoveDevice(esp_bd_addr_t bda);
			void UpdateDevice(uint16_t conn_id, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);
			void UpdateDevice(esp_bd_addr_t bda, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);
			void RefreshServiceDiscover(uint16_t conn_id);
			void NotifyDiscoverComplete(uint16_t conn_id);
			void NotifyReadEvtAttr(uint16_t conn_id,uint16_t handle,uint8_t *value,uint16_t value_len);
			void NotifyWriteEvtAttr(uint16_t conn_id,uint16_t handle);
			void NotifyDisconnect(esp_bd_addr_t bda);
			void OpenAllConenction();

			void UpdateState(const etl::imessage& message, esp_bd_addr_t bda);

	private:
			void updateField(uint8_t idx, uint16_t field_value, DEVICE_UPDATEFIELD_TYPES field_type);
			void processEvent(EventType type, uint16_t conn_id,uint16_t handle,uint8_t *val, uint32_t len);

	};
}


#endif /* SWL_GATEWAY_FLASHDEVICEMANAGER_FLASHDEVICEMANAGER_H_ */
