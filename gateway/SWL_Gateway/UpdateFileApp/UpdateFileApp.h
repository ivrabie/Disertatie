#ifndef UPDATE_FILE_UPDATE_FILE_H_
#define UPDATE_FILE_UPDATE_FILE_H_
#include <esp_log.h>
#include "../../HttpClient/HttpClient.h"
#include "../../sdcard/sdcard.h"
#include "../VersionManager/VersionManager.h"
#include <../../etl/cstring.h>
#include "../../SwlOled/SwlOled.h"
using namespace HTTP_CLIENT;
using namespace SDCARD;
using namespace etl;
namespace SWL_GATEWAY
{
#define UPDATE_MAX_TYPE_STR 50u
#define UPDATE_APP_TAG "UDATEFILE_APP"

    class UpdateFileApp:public Http_Observer
    {
enum UPDATE_FILE_STATE
{
    UPDATE_FILE_DEINIT,
    UPDATE_FILE_IDLE,
    UPDATE_FILE_REQ_VERSION,
    UPDATE_FILE_NEW_REQ,
    UDPDATE_FILE_REQUESTED,
    UPDATE_FILE_IN_PROGRESS,
};
        public:
        
        SdCard &sdCard;
        VersionManager &vsMng;
        SWL_OLED::SwlOled &swlOled;
        HttpClient &httpClient = HttpClient::getInstance();
        UPDATE_FILE_STATE updateFileState;
        uint32_t timerReqVersion = 0;
        uint32_t lastVersionReceived = 0;
        uint32_t totalFileLen = 0; 
        uint32_t receivedFileLen =  0; 
        etl::string<UPDATE_MAX_TYPE_STR> type;
        etl::string<UPDATE_MAX_TYPE_STR> fileName;
        UpdateFileApp(SdCard &sdCard,VersionManager &vsMng,SWL_OLED::SwlOled &swlOled);
        ~UpdateFileApp();

        void Init();

        void UpdateFileAppMainFunction(uint16_t task_ms);
        bool RequestUpdateFile(void);
        bool RequestLatestVersion(void);
        void notification(esp_http_client_event_t * evt);
        bool EnableUpdateFileApp(bool val);
        private:
        void toArray(uint16_t val, etl::string<5> &s);
    };
}

#endif