/*
 * SWL_Gateway.c
 *
 *  Created on: Jul 20, 2019
 *      Author: ion
 */

//#include <GapAdapter.h>
//#include <GapAdapter_cfg.h>
//#include <BleRemoteDevice.h>
//#include <BleRemoteDevice_types.h>
#include "SWL_Gateway_cfg.h"
#include "SWL_Gateway.h"
#include <esp_err.h>
#include <esp_log.h>
#include <string.h>
#include "../FlashDevice/FlashDevice.h"
using namespace SWL_GATEWAY;

#define MANUFACTURE_DATA_SIZE 6u

SwlGateway::SwlGateway(uint16_t appId,
						SdCard &sd,
						UpdateFileApp& updateFileApp,
						VersionManager &vers):
appId(appId),
sd(sd),
updateFileApp(updateFileApp),
vers(vers)
{
	this->gapAdapter.add_observer(*this);
	this->gattc.add_observer(*this);
}

SwlGateway::~SwlGateway()
{
	
}


void SwlGateway:: notification(GapEventInfo info)
{
esp_gap_ble_cb_event_t event  = info.event;
esp_ble_gap_cb_param_t *param = info.param;
ESP_LOGI(SWLGW_NAME, "Event SWL %d", event);
		switch(event)
		{
		case ESP_GAP_BLE_SCAN_RESULT_EVT:
			if(param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT)
			{
				ESP_LOGI(SWLGW_NAME, "Try create conenctions");
				this->triggerConnetion();
			}
			else
			{
				this->processScanData(param);
			}

			break;
		case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:

			break;
		default:
			break;
		}

}



void SwlGateway::Init(void)
{
	uint8_t manufacture_data[MANUFACTURE_DATA_SIZE];
	uint8_t len;

	this->gapAdapter.Init();
	this->gattc.Init();
	this->gattc.RegisterApp(this->appId); // @suppress("Invalid arguments")
	this->buildManufactureDataBuffer(manufacture_data,len);
	this->gapAdapter.SetDeviceName((uint8_t*)SWLGW_NAME,sizeof(SWLGW_NAME));
	this->gapAdapter.StartAdvertising(manufacture_data,len);
	this->wifiAdapter.Init();
	this->wifiAdapter.Connect();
	this->gapAdapter.StartScan(5u);

}

void SwlGateway::processRawData(uint8_t *data, uint8_t len, FlashDevice *flashDev)
{
		uint8_t idx = 0;
		uint8_t typeLen = 0;
		while(idx < len)
		{
			typeLen = data[idx];
			idx += 1u;
			// ESP_LOGI(SWLGW_NAME, "len: %d type %d",typeLen,data[idx]);
			switch(data[idx])
			{
				case ESP_BLE_AD_TYPE_NAME_CMPL:
					if((typeLen - 1u) == BLE_DEVICEINFO_EXPECTED_LEN_NAME)
					{
						memcpy(flashDev->deviceName, &data[idx + 1], BLE_DEVICEINFO_EXPECTED_LEN_NAME);
					}
					break;
				case ESP_BLE_AD_MANUFACTURER_SPECIFIC_TYPE:
					// Application specific
					if((typeLen - 1u) == (sizeof(uint16_t)))
					{
						flashDev->flashMagicNumber = *((uint16_t*)&data[idx + 1]);
					}
					break;
			}
			idx += typeLen;
		}
}


void SwlGateway::notification(GattcEventInfo info)
{
		esp_gattc_cb_event_t event = info.event;
		esp_gatt_if_t gattc_if = info.gattc_if;
		esp_ble_gattc_cb_param_t *param = info.param;
		esp_err_t ret;

		if(event == ESP_GATTC_REG_EVT)
		{
			if(param->reg.status == ESP_GATT_OK)
			{
				if(this->appId == param->reg.app_id)
				{
					ESP_LOGI(SWLGW_NAME, "Profile %d registered with status %d",param->reg.app_id ,param->reg.status);
					this->gattc_if = gattc_if;
				}
			}
		}
		else if(this->gattc_if == gattc_if)
		{
			switch(event)
			{
				case ESP_GATTC_REG_EVT:
					
					break;

				case ESP_GATTC_UNREG_EVT:
					break;
				case ESP_GATTC_OPEN_EVT:
					if(param->open.status == ESP_GATT_OK)
					{
						// this->flashDeviceManager.UpdateDevice(param->open.remote_bda,param->open.conn_id,DEVICE_UPDATEFIELD_CONECTION_ID);
						// this->flashDeviceManager.UpdateDevice(param->open.remote_bda,param->open.status,DEVICE_UPDATEFIELD_CONECTION_STATUS);
						// this->flashDeviceManager.UpdateDevice(param->open.remote_bda,param->open.mtu,DEVICE_UPDATEFIELD_CONECTION_MTU);

						ConnectEvt connectEvt(param->open.status,param->open.conn_id,param->open.mtu);
						this->flashDeviceManager.UpdateState(connectEvt,param->open.remote_bda);
						ret = esp_ble_gattc_send_mtu_req(gattc_if,param->open.conn_id);
						
						if(ret){
								ESP_LOGE(SWLGW_NAME, "%s gattc req mtu failed, error code = %x\n", __func__, ret);
								return;
							}
					}
					else
					{
						ESP_LOGE(SWLGW_NAME, "Conn open failed with status %d",param->open.status);
					}
					break;
				case ESP_GATTC_READ_CHAR_EVT:
					ESP_LOGI(SWLGW_NAME,
											"Read chr status %d\n"
											"Read chr conn_id: %d \n"
											"Read chr hdl: %d \n"
											"Read chr value_len %d",
											param->read.status,
											param->read.conn_id,
											param->read.handle,
											param->read.value_len);
					esp_log_buffer_hex(SWLGW_NAME, param->read.value, param->read.value_len);
					this->flashDeviceManager.NotifyReadEvtAttr(param->read.conn_id,param->read.handle,param->read.value,param->read.value_len);
					break;
				case ESP_GATTC_WRITE_CHAR_EVT:
					this->flashDeviceManager.NotifyWriteEvtAttr(param->write.conn_id, param->write.handle);
					break;
				case ESP_GATTC_CLOSE_EVT:
					this->flashDeviceManager.NotifyDisconnect(param->close.remote_bda);
					ESP_LOGI(SWLGW_NAME, "Try to disconnect");
					break;
				case ESP_GATTC_SEARCH_CMPL_EVT:
					ESP_LOGI(SWLGW_NAME, "Search result id: %d \n"
											"Search conn id %d\n"
											"Searched service source %d",param->search_cmpl.status,
											param->search_cmpl.conn_id,
											param->search_cmpl.searched_service_source);
					break;
				case ESP_GATTC_SEARCH_RES_EVT:
			//			ESP_LOGI(SWLGW_NAME,
			//								    "Search conn id %d\n"
			//					 	 	 	 	"Service start hdl: %d \n"
			//									"Service end hdl: %d \n"
			//								    "Service is primary %d\n"
			//								    "Service is len %d\n"
			//									"Service inst id %d\n"
			//									"Service uuid %d",
			//									param->search_res.conn_id,
			//									param->search_res.start_handle,
			//									param->search_res.end_handle,
			//									param->search_res.is_primary,
			//									param->search_res.srvc_id.uuid.len,
			//									param->search_res.srvc_id.inst_id,
			//									param->search_res.srvc_id.uuid.uuid.uuid16);

					break;
				case ESP_GATTC_READ_DESCR_EVT:
					ESP_LOGI(SWLGW_NAME,
							"Read desc status %d\n"
							"Read desc conn_id: %d \n"
							"Read desc hdl: %d \n"
							"Read desc value_len %d",
							param->read.status,
							param->read.conn_id,
							param->read.handle,
							param->read.value_len);
					esp_log_buffer_hex(SWLGW_NAME, param->read.value, param->read.value_len);
					this->flashDeviceManager.NotifyReadEvtAttr(param->read.conn_id,param->read.handle,param->read.value,param->read.value_len);
					break;
				case ESP_GATTC_WRITE_DESCR_EVT:
					this->flashDeviceManager.NotifyWriteEvtAttr(param->write.conn_id, param->write.handle);
					break;
				case ESP_GATTC_NOTIFY_EVT:
					ESP_LOGI(SWLGW_NAME,
							"Read notify status %d\n"
							"Read notify conn_id: %d \n"
							"Read notify hdl: %d \n"
							"Read notify value_len %d",
							param->notify.is_notify,
							param->notify.conn_id,
							param->notify.handle,
							param->notify.value_len);
					break;
				case ESP_GATTC_PREP_WRITE_EVT:
					break;
				case ESP_GATTC_EXEC_EVT:
					break;
				case ESP_GATTC_ACL_EVT:
					break;
				case ESP_GATTC_CANCEL_OPEN_EVT:
					break;
				case ESP_GATTC_SRVC_CHG_EVT:
					break;
				case ESP_GATTC_ENC_CMPL_CB_EVT:
					break;
				case ESP_GATTC_CFG_MTU_EVT:
					if(param->cfg_mtu.status == ESP_OK)
					{
						ESP_LOGI(SWLGW_NAME, "Mtu cfg status: %d \n"
												"Conn id %d\n"
												"Conn mtu %d",param->cfg_mtu.status,
												param->cfg_mtu.conn_id,
												param->cfg_mtu.mtu);
						ESP_LOGI(SWLGW_NAME, "Search service");
						this->flashDeviceManager.RefreshServiceDiscover(param->connect.conn_id);

						this->flashDeviceManager.UpdateDevice(param->cfg_mtu.conn_id,param->cfg_mtu.mtu,DEVICE_UPDATEFIELD_CONECTION_MTU);
					}
					break;
				case ESP_GATTC_ADV_DATA_EVT:
					break;
				case ESP_GATTC_MULT_ADV_ENB_EVT:
					break;
				case ESP_GATTC_MULT_ADV_UPD_EVT:
					break;
				case ESP_GATTC_MULT_ADV_DATA_EVT:
					break;
				case ESP_GATTC_MULT_ADV_DIS_EVT:
					break;
				case ESP_GATTC_CONGEST_EVT:
					break;
				case ESP_GATTC_BTH_SCAN_ENB_EVT:
					break;
				case ESP_GATTC_BTH_SCAN_CFG_EVT:
					break;
				case ESP_GATTC_BTH_SCAN_RD_EVT:
					break;
				case ESP_GATTC_BTH_SCAN_THR_EVT:
					break;
				case ESP_GATTC_BTH_SCAN_PARAM_EVT:
					break;
				case ESP_GATTC_BTH_SCAN_DIS_EVT:
					break;
				case ESP_GATTC_SCAN_FLT_CFG_EVT:
					break;
				case ESP_GATTC_SCAN_FLT_PARAM_EVT:
					break;
				case ESP_GATTC_SCAN_FLT_STATUS_EVT:
					break;
				case ESP_GATTC_ADV_VSC_EVT:
					break;
				case ESP_GATTC_REG_FOR_NOTIFY_EVT:
					break;
				case ESP_GATTC_UNREG_FOR_NOTIFY_EVT:
					break;
				case ESP_GATTC_CONNECT_EVT:
					ESP_LOGI(SWLGW_NAME, "Conn id: %d \n"
							"Conn params interval %d\n"
							"Conn params latency %d\n"
							"Conn params timeout %d",param->connect.conn_id,
							param->connect.conn_params.interval,
							param->connect.conn_params.latency,
							param->connect.conn_params.timeout);
					break;
				case ESP_GATTC_DISCONNECT_EVT:
					break;
				case ESP_GATTC_READ_MULTIPLE_EVT:
					break;
				case ESP_GATTC_QUEUE_FULL_EVT:
					break;
				case ESP_GATTC_SET_ASSOC_EVT:
					break;
				case ESP_GATTC_GET_ADDR_LIST_EVT:
					break;
				case ESP_GATTC_DIS_SRVC_CMPL_EVT:
					ESP_LOGI(SWLGW_NAME, "Conn id: %d \n"
											"Conn disc status %d"
											,param->dis_srvc_cmpl.conn_id,
											param->dis_srvc_cmpl.status);
					this->flashDeviceManager.NotifyDiscoverComplete(param->dis_srvc_cmpl.conn_id);
					break;
			}
		}
	
}

void SwlGateway::buildManufactureDataBuffer(uint8_t *data, uint8_t &len)
{
	data[0] = (uint8_t)SWLGW_MAGIC_NUMBER;
	data[1] = (uint8_t)(SWLGW_MAGIC_NUMBER>>8u);
	uint32_t version = this->vers.GetLatestVersion();
	memcpy((void*)&data[2u], (void*)&version,sizeof(uint32_t));
	len = 6;
}


void SwlGateway::processScanData(esp_ble_gap_cb_param_t *param)
{
//	ESP_LOGI(GAP_NAME, "Scan event: %d \n"
//			"Device type %d\n"
//			"Ble event type %d\n"
//			"Ble addr type %d\n"
//			"Rssi: %d\n"
//			"Adv len: %d\n"
//			"Resp len: %d\n"
//			"Packet discarded: %d\n"
//			"Scan result: %d\n,"
//			"Flags: %d\n", param->scan_rst.search_evt,param->scan_rst.dev_type,
//			param->scan_rst.ble_evt_type,param->scan_rst.ble_addr_type,param->scan_rst.rssi,
//			param->scan_rst.adv_data_len,param->scan_rst.scan_rsp_len,param->scan_rst.num_dis,
//			param->scan_rst.num_resps, param->scan_rst.flag);
//	esp_log_buffer_hex(GAP_NAME, param->scan_rst.bda, 6);



		if (param->scan_rst.adv_data_len > 0) {
			static message_id_t router_id = 0;
//			esp_log_buffer_hex(GAP_NAME, &param->scan_rst.ble_adv[0], param->scan_rst.adv_data_len);
			FlashDevice flashDev(router_id,&this->vers,&this->sd,this->gattc_if);
			this->processRawData(&param->scan_rst.ble_adv[0],param->scan_rst.adv_data_len,&flashDev);

			if(SWLGW_MAGIC_NUMBER == flashDev.flashMagicNumber)
			{
				ESP_LOGI(GAP_NAME, "Register device");
				flashDev.ble_addr_type = param->scan_rst.ble_addr_type;
				flashDev.gattc_if = this->gattc_if;
				memcpy((void *)flashDev.bda,(void *)param->scan_rst.bda,ESP_BD_ADDR_LEN);
				this->flashDeviceManager.RegisterDevice(&flashDev);
				router_id = router_id + 1u;
			}
		}
}

void SwlGateway::triggerConnetion(void)
{
	ESP_LOGI(SWLGW_NAME, "Registred devices %d",flashDeviceManager.listOfDevice.size());
	this->flashDeviceManager.OpenAllConenction();
}


void SwlGateway::notification(esp_http_client_event_t * evt)
{
}

void SwlGateway::TaskMain_100ms(void)
{
	this->updateFileApp.UpdateFileAppMainFunction(100);
}
