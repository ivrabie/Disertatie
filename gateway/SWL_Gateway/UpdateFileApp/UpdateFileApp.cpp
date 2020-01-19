
#include "UpdateFileApp.h"

#define URL_VERSION "http://192.168.100.2:8000/version/"
#define URL_FILE    "http://192.168.100.2:8000/download/"
#define UPDATEREQVERSION_TIME 10000u
#define UPDATEFILE_TYPE_VERSION "version"
#define UPDATEFILE_TYPE_FILE "file"
#define UPDATEFILE_TYPE_NONE "none"
#define UPDATE_FILE_NAME "sw"
using namespace SWL_GATEWAY;
UpdateFileApp::UpdateFileApp(SdCard &sdCard,VersionManager &vsMng,SWL_OLED::SwlOled &swlOled):sdCard(sdCard),vsMng(vsMng),swlOled(swlOled)
{
    this->updateFileState = UPDATE_FILE_IDLE;
    this->timerReqVersion = 0u;
    this->type.clear();
    this->type.assign(UPDATEFILE_TYPE_NONE);
    this->httpClient.add_observer(*this);
}


UpdateFileApp::~UpdateFileApp()
{

}

void UpdateFileApp::Init()
{
    this->swlOled.UpAppWriteVersion(this->vsMng.GetLatestVersion());
}
bool UpdateFileApp::RequestUpdateFile(void)
{
    bool ret = false;
    if(this->updateFileState == UPDATE_FILE_NEW_REQ)
    {
        ESP_LOGI(UPDATE_APP_TAG, "Reqesut performed");
        
        ret = true;
    }
    return ret;
}



void UpdateFileApp::UpdateFileAppMainFunction(uint16_t task_ms)
{
    // ESP_LOGI(UPDATE_APP_TAG, "App state %d", this->updateFileState);
    switch(this->updateFileState)
    {
        case UPDATE_FILE_IDLE:
            if(this->timerReqVersion < task_ms)
            {
                this->swlOled.UpAppWriteStatus("VERSION");
                this->httpClient.PerformRequest(URL_VERSION);
                this->timerReqVersion = UPDATEREQVERSION_TIME;
                this->swlOled.UpAppWriteStatus("IDLE");
            }
            else
            {
                this->timerReqVersion -= task_ms;
                
            }
        break;

        case UPDATE_FILE_NEW_REQ:
            ESP_LOGI(UPDATE_APP_TAG, "Try to request file");
            this->swlOled.UpAppWriteStatus("DOWNLOAD");
            this->httpClient.PerformRequest(URL_FILE);
            
        break;
        case UPDATE_FILE_IN_PROGRESS:
        case UDPDATE_FILE_REQUESTED:
        case UPDATE_FILE_REQ_VERSION:
        case UPDATE_FILE_DEINIT:
        break;
    }
}

bool UpdateFileApp::EnableUpdateFileApp(bool val)
{
    bool ret = false;
    if(this->updateFileState == UPDATE_FILE_IDLE ||
        this->updateFileState == UPDATE_FILE_DEINIT)
    {
        if(val == false)
        {
            this->updateFileState = UPDATE_FILE_DEINIT;
            this->swlOled.UpAppWriteStatus("STOPPED");
        }
        else
        {
            this->swlOled.UpAppWriteStatus("IDLE");
            this->updateFileState = UPDATE_FILE_IDLE;
        }
        ret = true;
    }
    return ret;
}


void UpdateFileApp::notification(esp_http_client_event_t * evt)
{
    etl::string<UPDATE_MAX_TYPE_STR> str_content("Content-Length");
    etl::string<UPDATE_MAX_TYPE_STR> str_vers;
    size_t bytesWrote = 0;
    static bool fileDownloadFailed = false;
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(UPDATE_APP_TAG, "Header sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            if(this->updateFileState ==   UPDATE_FILE_NEW_REQ)
            {
                if(str_content.compare(evt->header_key) == 0)
                {
                    this->receivedFileLen = 0u;
                    this->totalFileLen = (uint32_t)atoi(evt->header_value);
                    ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_ON_HEADER, total file len:%d",this->totalFileLen);
                    this->updateFileState = UPDATE_FILE_IN_PROGRESS;
                    fileDownloadFailed = false;
                }
            }
    
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if(this->updateFileState == UPDATE_FILE_IDLE)
            {
                str_vers.clear();
                str_vers.append((char*)evt->data,evt->data_len);
                uint32_t version = (uint32_t)atoi(str_vers.c_str());
                ESP_LOGI(UPDATE_APP_TAG, "Server version %s", str_vers.c_str());
                ESP_LOGI(UPDATE_APP_TAG, "Server version 0x%08x", version);
                if(this->vsMng.GetLatestVersion() < version)
                {
                    this->updateFileState = UPDATE_FILE_NEW_REQ;
                    this->lastVersionReceived = version;
                    this->fileName.clear();
                    uint8_t major = (uint8_t)(version >> 24);
                    uint16_t minor = (uint16_t)(version >> 8);
                    uint8_t patch = (uint8_t)version;
                    etl::string<5> s;
                    this->toArray(major, s);
                    this->fileName.append(s);
                    this->toArray(minor, s);
                    this->fileName.append(s);
                    this->toArray(patch, s);
                    this->fileName.append(s);
                    this->fileName.append(".bin");
                    
                    this->sdCard.DeleteFile((char*)this->fileName.c_str());
                    fileDownloadFailed = false;
                    ESP_LOGI(UPDATE_APP_TAG, "The new file will be %s len %d", this->fileName.c_str(),this->fileName.length());
                    
                }
                else
                {
                    this->updateFileState = UPDATE_FILE_IDLE;
                    ESP_LOGI(UPDATE_APP_TAG, "No new version, current version 0x%08x", this->vsMng.GetLatestVersion());
                }
            }
            else if(this->updateFileState == UPDATE_FILE_IN_PROGRESS && fileDownloadFailed == false)
            {
                this->receivedFileLen += evt->data_len;
                ESP_LOGI(UPDATE_APP_TAG,"Len recived %d", this->receivedFileLen);
                bytesWrote = this->sdCard.WriteBinaryFile((char*)this->fileName.c_str(),(uint8_t*)evt->data,evt->data_len);
                uint8_t progress = (uint8_t)((((float)this->receivedFileLen) / ((float)this->totalFileLen)) * ((float)100));
                ESP_LOGI(UPDATE_APP_TAG,"Len recived %d", progress);
                this->swlOled.UpAppWriteProgres(progress);
                fileDownloadFailed = (evt->data_len != bytesWrote) ? true : false;
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_ON_FINISH");
            
            if(this->updateFileState == UPDATE_FILE_IN_PROGRESS)
            {
                if(this->totalFileLen == this->receivedFileLen)
                {
                    ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_ON_FINISH, File updated successfuly");
                    this->vsMng.UpdateVersion(this->lastVersionReceived,(char*)this->fileName.c_str());
                    this->swlOled.UpAppWriteVersion(this->lastVersionReceived);
                    
                }
                this->updateFileState = UPDATE_FILE_IDLE;
                fileDownloadFailed = false;
                this->swlOled.UpAppWriteProgres(0);
            }
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(UPDATE_APP_TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(UPDATE_APP_TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(UPDATE_APP_TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            this->swlOled.UpAppWriteProgres(0);
            break;
    }
}

void UpdateFileApp::toArray(uint16_t val, etl::string<5> &s)
{
    s.clear();
    int i = 0;
    while (val != 0)
    {
        uint8_t v = val % 10;
        s.push_back(v+'0');
        val = val/10;
    }
    for (i = 0; i < s.length() / 2; i++) 
    {
        char c = s[i];
        s[i] = s[s.length() - i - 1];
        s[s.length() - i - 1] = c;
    }
}