/*
 * ble.c
 *
 *  Created on: Jul 17, 2019
 *      Author: ion
 */


#include "BleAdapter.h"
#include "BleAdapter_cfg.h"
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_bt_defs.h>
#include <esp_bt_main.h>
#include "esp_gatt_common_api.h"
#include <esp_log.h>


using namespace BLE;
#define BLE_MAX_PROFILE_HANDLE 10u

BleAdapter BleAdapter::bleAdapter;

BleAdapter::BleAdapter()
{
	this->bleAdapterState = BLEADAPTER_NO_INIT;

}

BleAdapter::~BleAdapter()
{

}

esp_err_t BleAdapter::Init(void)
{
	esp_err_t ret = ESP_OK;


	if(this->bleAdapterState == BLEADAPTER_NO_INIT)
	{
		esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();


		ret = esp_bt_controller_init(&bt_cfg);
		 if (ret) {
				ESP_LOGE(BLE_MODULE_NAME, "%s initialize controller failed\n", __func__);
				return ret;
			}


		ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
		if (ret) {
			ESP_LOGE(BLE_MODULE_NAME, "%s enable controller failed\n", __func__);
			return ret;
		}


	   ret = esp_bluedroid_init();
		if (ret) {
			ESP_LOGE(BLE_MODULE_NAME, "%s init bluetooth failed\n", __func__);
			return ret;
		}

		ret = esp_bluedroid_enable();
		if (ret) {
			ESP_LOGE(BLE_MODULE_NAME, "%s enable bluetooth failed\n", __func__);
			return ret;
		}


	    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
	    if (local_mtu_ret){
	        ESP_LOGE(BLE_MODULE_NAME, "set local  MTU failed, error code = %x", local_mtu_ret);
	    }
		this->bleAdapterState = BLEADAPTER_INIT;
		ESP_LOGI(BLE_MODULE_NAME, "Ble adapter init success");
	}
	else
	{
		ESP_LOGW(BLE_MODULE_NAME, "Ble adapter already was init success");
	}

	return ret;
}


bool BleAdapter::IsBleStackActive(void)
{
	return this->bleAdapterState == BLEADAPTER_NO_INIT ? false : true;
}

BleAdapter& BleAdapter::getInstance(void)
{
	return BleAdapter::bleAdapter;
}