/*
 * ble.h
 *
 *  Created on: Jul 17, 2019
 *      Author: ion
 */

#ifndef BLE_BLEADAPTER_H_
#define BLE_BLEADAPTER_H_


#include "esp_err.h"

namespace BLE
{
	typedef enum
	{
		BLEADAPTER_NO_INIT,
		BLEADAPTER_INIT
	}BLEADAPTER_STATE_TYPE;
	class BleAdapter
	{

	private:
		static BleAdapter bleAdapter;
		BleAdapter();
	public:

		BLEADAPTER_STATE_TYPE bleAdapterState;

		
		~BleAdapter();
		esp_err_t Init(void);
		bool IsBleStackActive(void);
		static BleAdapter& getInstance(void);
	};

}

#endif /* BLE_BLEADAPTER_H_ */
