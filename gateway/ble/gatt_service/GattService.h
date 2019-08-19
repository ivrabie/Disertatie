/*
 * GattService.h
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */

#ifndef BLE_GATT_SERVICE_GATTSERVICE_H_
#define BLE_GATT_SERVICE_GATTSERVICE_H_
#include <esp_gatts_api.h>
#include <esp_gattc_api.h>

namespace BLE
{
#define SERVICE_PER_BOARD 2u
#define GATT_SERVICE_NAME "GATT_SERVICE"
	class GattDescriptor
	{
	public:

		esp_bt_uuid_t        uuid;
		esp_gatt_perm_t      perm;
		esp_attr_value_t     desc_val;
		esp_attr_control_t   control;
		uint16_t 			 desc_handle = 0;
		virtual void Clone(const GattDescriptor &desc);
		void PrintDesc(void);
		GattDescriptor();
		GattDescriptor(const GattDescriptor &desc);
		~GattDescriptor();
	};

class GattCharacteristic
{

private:
		uint8_t isDescAdded(esp_bt_uuid_t *uuid);
public:
#define MAX_DESC_PER_CHR 5u
	esp_bt_uuid_t        uuid;
	esp_gatt_perm_t      perm;
	esp_gatt_char_prop_t property;
	esp_attr_value_t     char_val;
	esp_attr_control_t   control;
	uint16_t 			 chr_handle = 0;

	GattDescriptor listOfDesc[MAX_DESC_PER_CHR];
	uint16_t noOfRegistredDesc = 0;
	virtual void Clone(const GattCharacteristic &val);
	GattCharacteristic();
	GattCharacteristic(const GattCharacteristic &chr);
	void PrintChr(void);
	bool AddDesc(GattDescriptor *chr);
	bool DeleteDesc(GattDescriptor *chr);
	bool DeleteDesc(esp_bt_uuid_t *uuid);


	~GattCharacteristic();
};

	class GattService
	{
#define MAX_CHR_PER_SERVICE 5u
		private:

			uint8_t isChrsAdded(esp_bt_uuid_t *uuid);

		public:
			GattCharacteristic listOfChr[MAX_CHR_PER_SERVICE];
			uint16_t noOfRegistredChrs = 0;
			uint16_t numHandles;
			uint16_t serviceHandle = 0;
			uint16_t endServiceHandle = 0;
			esp_gatt_srvc_id_t serviceInfo;
			GattService();
			~GattService();
			GattService(const GattService &serv);
			bool AddChr(GattCharacteristic *chr);
			bool DeleteChr(GattCharacteristic *chr);
			bool DeleteChr(esp_bt_uuid_t *uuid);
			virtual void Clone(const GattService &serv);
			void PrintService(void);
			bool IsServiceInit(void);
			void Update(esp_gatt_srvc_id_t *serviceInfo,uint16_t service_handle);
			void Update(uint16_t attr_handle,uint16_t service_handle,esp_bt_uuid_t *uuid);
			static bool IsUuidEqual(esp_bt_uuid_t *uuid, esp_bt_uuid_t *uuid1);

	};




}


#endif /* BLE_GATT_SERVICE_GATTSERVICE_H_ */
