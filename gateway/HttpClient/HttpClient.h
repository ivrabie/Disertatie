/*
 * HttpClient.h
 *
 *  Created on: Aug 19, 2019
 *      Author: ion
 */

#ifndef HTTPCLIENT_HTTPCLIENT_H_
#define HTTPCLIENT_HTTPCLIENT_H_
#include "../wifi/WifiAdapter.h"
#include <esp_http_client.h>
#include <esp_tls.h>
using namespace WIFI_ADAPTER;
namespace HTTP_CLIENT
{

#define HTTP_CLIENT_TAG "HttpClient"
	class HttpClient
	{
	private:
		static esp_err_t httpEventHandler(esp_http_client_event_t *evt);
		static HttpClient httpClient;
		esp_http_client_config_t httpClientConfig;
		WifiAdapter *wifiAdapter;
		HttpClient();
		~HttpClient();
		esp_http_client_handle_t clientHandle;

	public:
		static HttpClient* getInstance(void);

		void PerformRequest(void);

	};

}


#endif /* HTTPCLIENT_HTTPCLIENT_H_ */
