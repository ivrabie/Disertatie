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
#include "../etl/observer.h"
using namespace WIFI_ADAPTER;
using namespace etl;
extern etl::ifunction<esp_http_client_event_t *>* http_event_callback;
namespace HTTP_CLIENT
{

#define HTTP_CLIENT_TAG "HttpClient"
#define HTTP_MAX_OBSERVERS UINT8_C(2u)
typedef etl::observer<esp_http_client_event_t *> Http_Observer;
	class HttpClient:public etl::observable<Http_Observer, HTTP_MAX_OBSERVERS>
	{
	private:
		
		esp_http_client_config_t httpClientConfig;
		WifiAdapter &wifiAdapter = WifiAdapter::getInstance();
		esp_http_client_handle_t clientHandle;
		
		HttpClient();
		~HttpClient();
	public:

		static HttpClient http;
		bool PerformRequest(const char *url);
		void RequestStreamData(void);
		void httpEventHandler(esp_http_client_event_t *evt);
		static HttpClient& getInstance(void);

	};

}


#endif /* HTTPCLIENT_HTTPCLIENT_H_ */
