#ifndef SWL_OLED_FILE_H_
#define SWL_OLED_FILE_H_

#include "../../Ssd1306/ssd1306.hpp"
#include "../../etl/cstring.h"
namespace SWL_OLED
{
    #define SWLOLED_MAX_STR_SIZE UINT8_C(128)
    class SwlOled
    {
        private:
            OLED &oled;
            uint8_t updateAppStatusPixels  = 0u;
            uint8_t updateAppProgresPixels = 0u;
            uint8_t updateAppVersionPixels = 0u;
            
            uint8_t gwAppStatusPixels  = 0u;
            uint8_t gwAppProgresPixels = 0u;
            uint8_t gwAppDevInfoPixels = 0u;
            void intToString(uint32_t val, etl::string<4u>&str);
        public:
            SwlOled(OLED &oled);
            void Init();
            
            void UpAppWriteStatus(etl::string<SWLOLED_MAX_STR_SIZE> str);
            void UpAppWriteProgres(uint8_t val);
            void UpAppWriteVersion(uint32_t version);

            void GwUpWriteStatus(etl::string<SWLOLED_MAX_STR_SIZE> str);
            void GwUpWriteDevicesInfo(uint8_t foundDevices,uint8_t updatedDevices);
            void GwUpWriteProgess(uint8_t progress);
    };
}

#endif