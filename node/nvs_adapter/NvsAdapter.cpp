


#include <NvsAdapter.h>
#include "nvs_flash.h"


using namespace NVS_ADAPTER;


bool NVSAdapter::isNvsInitiated = false;


NVSAdapter::NVSAdapter()
{

}

NVSAdapter::~NVSAdapter()
{

}


void NVSAdapter:: Init(void)
{
	if(NVSAdapter::isNvsInitiated == false)
	{
		   esp_err_t ret = nvs_flash_init();
		    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		      ESP_ERROR_CHECK(nvs_flash_erase());
		      ret = nvs_flash_init();
		    }
		    if(ret == ESP_OK)
		    {
		    	isNvsInitiated = true;
		    }
	}
}

