


#include "VersionManager.h"
#include <esp_log.h>
#include "../../etl/basic_string.h"
using namespace SWL_GATEWAY;


VersionManager::VersionManager()
{
    this->path_len = VERSION_DEFAULT_PATH_SIZE;
}

VersionManager::~VersionManager()
{

}
void VersionManager::Init(void)
{
    esp_err_t err;
    this->nvs.Init();
    err = this->nvs.Read(VERSION_KEY,&this->latest_version);
    if(err != ESP_OK)
    {
        ESP_LOGI(VERSION_MANAGER_TAG, "Store default data");
        this->nvs.Write(VERSION_KEY,(uint32_t)VERSION_DEFAULT);
        err = this->nvs.Read(VERSION_KEY,&this->latest_version);
    }
    ESP_LOGI(VERSION_MANAGER_TAG, "Latest version is %d", this->latest_version);
    this->path_len = MAX_PATH_LEN;
    err = this->nvs.Read(FILEPATH_KEY,&this->path[0],&this->path_len);
    if(err != ESP_OK)
    {

        ESP_LOGI(VERSION_MANAGER_TAG, "Store default data");
        this->nvs.Write(FILEPATH_KEY,VERSION_DEFAULT_PATH);
        err = this->nvs.Read(FILEPATH_KEY,&this->path[0],&this->path_len);
    }
    ESP_LOGI(VERSION_MANAGER_TAG, "Path %s", this->path);
    ESP_LOGI(VERSION_MANAGER_TAG, "Path len %d", this->path_len);
}


uint32_t VersionManager::GetLatestVersion(void)
{
    
    return (this->latest_version == VERSION_DEFAULT) ? 0u : this->latest_version;
}

char* VersionManager::GetUpdateFileName(void)
{
    return this->path;
}

void VersionManager::UpdateVersion(uint32_t version_value, char *fileName)
{
    esp_err_t err;
    err = this->nvs.Write(VERSION_KEY,version_value);
    if(err == ESP_OK)
    {
        this->latest_version = version_value;
    }
    err = this->nvs.Write(FILEPATH_KEY,fileName);
    if(err == ESP_OK)
    {
        memset(this->path,0,this->path_len);
        this->path_len = strlen(fileName);
        memcpy(this->path,fileName,this->path_len);
    }
}

