/*
 * nvs_adapter.h
 *
 *  Created on: Jul 17, 2019
 *      Author: ion
 */

#ifndef NVS_ADAPTER_NVSADAPTER_H_
#define NVS_ADAPTER_NVSADAPTER_H_

#include "nvs_flash.h"
namespace NVS_ADAPTER
{
#define STORAGE_NAMESPACE "storage"
#define NVS_ADAPTER_TAG "NVS_ADAPTER"
	class NVSAdapter
	{
		
		bool isNvsInitiated;
		nvs_handle_t nvsHandle;
		NVSAdapter();
		~NVSAdapter();
	public:

		static NVSAdapter nvs;
		void Init(void);

		esp_err_t Read(const char *key, uint32_t *out_value);
		esp_err_t Read(const char *key, char *out_value, size_t *length);

		esp_err_t Write(char *key, uint32_t value);
		esp_err_t Write(char *key, const char *value);

		static NVSAdapter& getInstance(void);

	};
}


#endif /* NVS_ADAPTER_NVSADAPTER_H_ */
