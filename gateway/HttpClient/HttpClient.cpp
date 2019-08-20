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

HttpClient HttpClient::httpClient;
HttpClient::HttpClient()
{
	this->wifiAdapter = WifiAdapter::getInstance();
}


HttpClient::~HttpClient()
{

}
HttpClient* HttpClient::getInstance(void)
{
	return &httpClient;
}


esp_err_t HttpClient::httpEventHandler(esp_http_client_event_t *evt)
{
	 switch(evt->event_id) {
	        case HTTP_EVENT_ERROR:
	            ESP_LOGI(HTTP_CLIENT_TAG, "HTTP_EVENT_ERROR");
	            break;
	        case HTTP_EVENT_ON_CONNECTED:
	            ESP_LOGI(HTTP_CLIENT_TAG, "HTTP_EVENT_ON_CONNECTED");
	            break;
	        case HTTP_EVENT_HEADER_SENT:
	            ESP_LOGI(HTTP_CLIENT_TAG, "Header sent");
	            break;
	        case HTTP_EVENT_ON_HEADER:
	            ESP_LOGI(HTTP_CLIENT_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
	            break;
	        case HTTP_EVENT_ON_DATA:
	            ESP_LOGI(HTTP_CLIENT_TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
	            if (!esp_http_client_is_chunked_response(evt->client)) {
	                // Write out data
	                // printf("%.*s", evt->data_len, (char*)evt->data);
	            }

	            break;
	        case HTTP_EVENT_ON_FINISH:
	            ESP_LOGI(HTTP_CLIENT_TAG, "HTTP_EVENT_ON_FINISH");
	            break;
	        case HTTP_EVENT_DISCONNECTED:
	            ESP_LOGI(HTTP_CLIENT_TAG, "HTTP_EVENT_DISCONNECTED");
	            int mbedtls_err = 0;
	            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
	            if (err != 0) {
	                ESP_LOGI(HTTP_CLIENT_TAG, "Last esp error code: 0x%x", err);
	                ESP_LOGI(HTTP_CLIENT_TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
	            }
	            break;
	    }
	    return ESP_OK;
}

void HttpClient::PerformRequest(void)
{
	if(this->wifiAdapter != NULL && WifiAdapter::state == WIFI_STATE_CONNECTED)
	{
		this->httpClientConfig.url = "http://192.168.100.2:8000/users/";
		this->httpClientConfig.transport_type = HTTP_TRANSPORT_OVER_TCP;
		this->httpClientConfig.event_handler = HttpClient::httpEventHandler;

		this->clientHandle = esp_http_client_init(&this->httpClientConfig);

		 esp_err_t err = esp_http_client_perform(this->clientHandle);
		    if (err == ESP_OK) {
		        ESP_LOGI(HTTP_CLIENT_TAG, "HTTP GET Status = %d, content_length = %d",
		                esp_http_client_get_status_code(this->clientHandle),
		                esp_http_client_get_content_length(this->clientHandle));
		    } else {
		        ESP_LOGE(HTTP_CLIENT_TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
		    }
		esp_http_client_cleanup(this->clientHandle);
	}
}




