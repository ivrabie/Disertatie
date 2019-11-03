


#include <NvsAdapter.h>
#include "nvs_flash.h"
#include <esp_log.h>


using namespace NVS_ADAPTER;



NVSAdapter NVSAdapter::nvs;

NVSAdapter::NVSAdapter()
{
	this->nvsHandle = 0;
}

NVSAdapter::~NVSAdapter()
{
	if(this->nvsHandle != 0u)
	{
		// Close
    	nvs_close(this->nvsHandle);
		this->nvsHandle = 0;
	}
	nvs_flash_deinit();
}


void NVSAdapter:: Init(void)
{
	if(this->isNvsInitiated == false)
	{
		esp_err_t ret = nvs_flash_init();
		if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
			ESP_ERROR_CHECK(nvs_flash_erase());
			ret = nvs_flash_init();
		}
		if(ret == ESP_OK)
		{
			this->isNvsInitiated = true;
		}

		// Open
		esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &this->nvsHandle);
		if (err == ESP_OK) 
		{
			ESP_LOGI(NVS_ADAPTER_TAG, "NVS adapter success with handle %d", this->nvsHandle);
		}
		else
		{
			ESP_LOGE(NVS_ADAPTER_TAG, "NVS adapter failed with code %d", err);
		}
	}

}


esp_err_t NVSAdapter::Read(const char *key, uint32_t *out_value)
{
	 esp_err_t err = nvs_get_u32(this->nvsHandle, key, out_value);

	if(err != ESP_OK)
	{
		ESP_LOGE(NVS_ADAPTER_TAG, "NVS read failed with code %d", err);
	}
	return err;
}
esp_err_t NVSAdapter::Read(const char *key, char *out_value, size_t *length)
{
	esp_err_t err = nvs_get_str(this->nvsHandle, key, out_value, length);

	if(err != ESP_OK)
	{
		ESP_LOGE(NVS_ADAPTER_TAG, "NVS read failed with code %d", err);
	}
	return err;
}

esp_err_t NVSAdapter::Write(char *key, uint32_t value)
{
	esp_err_t err = nvs_set_u32(this->nvsHandle, key, value);
	if(err != ESP_OK)
	{
		ESP_LOGE(NVS_ADAPTER_TAG, "NVS write failed with code %d", err);
		return err;
	}

	err = nvs_commit(this->nvsHandle);
	if(err != ESP_OK)
	{
		ESP_LOGE(NVS_ADAPTER_TAG, "NVS write commit failed with code %d", err);
	}
	return err;
}
esp_err_t NVSAdapter::Write(char *key, const char *value)
{
	esp_err_t err = nvs_set_str(this->nvsHandle, key, value);
	if(err != ESP_OK)
	{
		ESP_LOGE(NVS_ADAPTER_TAG, "NVS write failed with code %d", err);
		return err;
	}

	err = nvs_commit(this->nvsHandle);
	if(err != ESP_OK)
	{
		ESP_LOGE(NVS_ADAPTER_TAG, "NVS write commit failed with code %d", err);
	}
	return err;
}

NVSAdapter& NVSAdapter::getInstance(void)
{
	return NVSAdapter::nvs;
}