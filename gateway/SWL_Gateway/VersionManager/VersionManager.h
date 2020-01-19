#ifndef VERSION_MANGER_VERSION_MANGER_H_
#define VERSION_MANGER_VERSION_MANGER_H_
#include "../../nvs_adapter/NvsAdapter.h"
#include <stdint.h>
using namespace NVS_ADAPTER;
namespace SWL_GATEWAY
{
#define MAX_PATH_LEN 50u
#define VERSION_KEY  "version"
#define FILEPATH_KEY "filepath"
#define VERSION_MANAGER_TAG "VersionManager"
#define VERSION_DEFAULT_PATH "XXXXX"
#define VERSION_DEFAULT_PATH_SIZE 5u
#define VERSION_DEFAULT UINT32_C(0xFFFFFFFF)
    class VersionManager
    {

        public:
        uint32_t latest_version;
        char path[MAX_PATH_LEN];
        size_t path_len;
        NVSAdapter &nvs = NVSAdapter::getInstance();
        VersionManager();
        ~VersionManager();

        void Init(void);
        uint32_t GetLatestVersion(void);
        void UpdateVersion(uint32_t version_value, char *filename);
        char* GetUpdateFileName(void);
    };
}

#endif