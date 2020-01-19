/*
 * HttpClient.cpp
 *
 *  Created on: Aug 19, 2019
 *      Author: ion
 */


#include "HttpClient.h"
#include <esp_log.h>
#include <esp_err.h>
using namespace HTTP_CLIENT;


HttpClient HttpClient::http;
etl::ifunction<esp_http_client_event_t *>* http_event_callback = NULL;
etl::function_imp<HttpClient, esp_http_client_event_t*, HttpClient::http, &HttpClient::httpEventHandler> http_callback;
extern "C"
{
	esp_err_t httpEventHandlerCbk(esp_http_client_event_t *evt)
	{
		(*http_event_callback)(evt);
		return ESP_OK;
	}
}



HttpClient::HttpClient()
{
	 
}


HttpClient::~HttpClient()
{

}

void HttpClient::httpEventHandler(esp_http_client_event_t *evt)
{
	this->notify_observers(evt);
}

bool HttpClient::PerformRequest(const char *url)
{
	bool ret = false;
	if(WifiAdapter::state == WIFI_STATE_CONNECTED)
	{
		this->httpClientConfig.url = url;  //"http://192.168.100.2:8000/download/";
		this->httpClientConfig.transport_type = HTTP_TRANSPORT_OVER_TCP;
		this->httpClientConfig.event_handler = httpEventHandlerCbk;

		this->clientHandle = esp_http_client_init(&this->httpClientConfig);

		esp_err_t err = esp_http_client_perform(this->clientHandle);
		if (err == ESP_OK) {
			ESP_LOGI(HTTP_CLIENT_TAG, "HTTP GET Status = %d, content_length = %d",
					esp_http_client_get_status_code(this->clientHandle),
					esp_http_client_get_content_length(this->clientHandle));
			ret = true;
		} else {
			ESP_LOGE(HTTP_CLIENT_TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
		}
		esp_http_client_cleanup(this->clientHandle);
	}
	return ret;
}

HttpClient& HttpClient::getInstance(void)
{
	if(http_event_callback == NULL)
	{
		http_event_callback = &http_callback;
	}
	return HttpClient::http;
}