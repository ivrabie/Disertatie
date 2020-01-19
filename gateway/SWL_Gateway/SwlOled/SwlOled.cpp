#include "SwlOled.h"

#define SWL_OLED_POSITION_X_POS 0u
#define SWL_OLED_POSITION_Y_POS_UPAPP_STATUS 0u
#define SWL_OLED_POSITION_Y_POS_UPAPP_PROGRESS 11u
#define SWL_OLED_POSITION_Y_POS_UPAPP_VERSION 22u
#define SWL_OLED_POSITION_Y_POS_LINE 32u
#define SWL_OLED_POSITION_Y_POS_GWUP_STATUS 33u
#define SWL_OLED_POSITION_Y_POS_GWUP_DEVICE 44u
#define SWL_OLED_POSITION_Y_POS_GWUP_PROGRESS 55u
#define SWL_OLED_SPACE_LINE " "
SWL_OLED::SwlOled::SwlOled(OLED &oled):oled(oled)
{

}
void SWL_OLED::SwlOled::Init()
{

    etl::string<SWLOLED_MAX_STR_SIZE> tempStr("UpApp:Status:  ");
    this->oled.init();
    this->oled.select_font(1u);

    this->updateAppStatusPixels = this->oled.draw_string(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_UPAPP_STATUS,tempStr,WHITE,BLACK);
    tempStr.clear();
    tempStr += "UpApp:Progress:  ";
    this->updateAppProgresPixels = this->oled.draw_string(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_UPAPP_PROGRESS,tempStr,WHITE,BLACK);
    tempStr.clear();
    tempStr += "UpApp:Version:  ";
    this->updateAppVersionPixels = this->oled.draw_string(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_UPAPP_VERSION,tempStr,WHITE,BLACK);


    tempStr.clear();
    tempStr += "GwApp:Status: ";
    this->gwAppStatusPixels = this->oled.draw_string(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_GWUP_STATUS,tempStr,WHITE,BLACK);
    tempStr.clear();
    tempStr += "GwApp:DevInfo: ";
    this->gwAppDevInfoPixels = this->oled.draw_string(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_GWUP_DEVICE,tempStr,WHITE,BLACK);
    tempStr.clear();
    tempStr += "GwApp:Progress: ";
    this->gwAppProgresPixels = this->oled.draw_string(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_GWUP_PROGRESS,tempStr,WHITE,BLACK);
    this->oled.refresh(true);
    this->UpAppWriteStatus("IDLE");
    this->UpAppWriteProgres(0);
    this->UpAppWriteVersion(0);
    this->oled.draw_hline(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_LINE,128u,WHITE);
    this->GwUpWriteStatus("IDLE");
    this->GwUpWriteDevicesInfo(0,0);
    this->GwUpWriteProgess(0);
}
/*==============================Up Update============================================================*/
void SWL_OLED::SwlOled::UpAppWriteStatus(etl::string<SWLOLED_MAX_STR_SIZE> str)
{
    etl::string<SWLOLED_MAX_STR_SIZE> tempStrSpaces;
    uint8_t x = 128 - this->updateAppStatusPixels;
    for(int i = 0; i < x; i++)
    {
        tempStrSpaces += " ";
    }
    this->oled.draw_string(this->updateAppStatusPixels,SWL_OLED_POSITION_Y_POS_UPAPP_STATUS,tempStrSpaces,WHITE,BLACK);
    this->oled.draw_string(this->updateAppStatusPixels,SWL_OLED_POSITION_Y_POS_UPAPP_STATUS,str,WHITE,BLACK);
    this->oled.refresh(true);
}
void SWL_OLED::SwlOled::UpAppWriteProgres(uint8_t val)
{
    etl::string<4> tempProg;
    etl::string<SWLOLED_MAX_STR_SIZE> tempStr;
    uint8_t x = 128 - this->updateAppProgresPixels;
    for(int i = 0; i < x; i++)
    {
        tempStr += " ";
    }
    this->oled.draw_string(this->updateAppProgresPixels,SWL_OLED_POSITION_Y_POS_UPAPP_PROGRESS,tempStr,WHITE,BLACK);
    tempStr.clear();
    this->intToString(val,tempProg);
    tempStr += tempProg;
    tempStr += "/100";

    
    this->oled.draw_string(this->updateAppProgresPixels,SWL_OLED_POSITION_Y_POS_UPAPP_PROGRESS,tempStr,WHITE,BLACK);
    this->oled.refresh(true);
}
void SWL_OLED::SwlOled::UpAppWriteVersion(uint32_t version)
{
    uint8_t majorVers;
    uint16_t minVers;
    uint8_t pathVers;
    etl::string<SWLOLED_MAX_STR_SIZE> tempStr;
    etl::string<4> majStr;
    etl::string<4> minStr;
    etl::string<4> patchStr;

    uint8_t x = 128 - this->updateAppVersionPixels;
    for(int i = 0; i < x; i++)
    {
        tempStr += " ";
    }
    this->oled.draw_string(this->updateAppVersionPixels,SWL_OLED_POSITION_Y_POS_UPAPP_VERSION,tempStr,WHITE,BLACK);
    majorVers = (uint8_t)(version>>24u);
    minVers = (uint8_t)(version>>16u);
    minVers |= (uint8_t)(version>>8u);
    pathVers = (uint8_t)version;
    tempStr.clear();
    this->intToString(majorVers,majStr);
    this->intToString(minVers,minStr);
    this->intToString(pathVers,patchStr);
    tempStr += majStr;
    tempStr += ".";
    tempStr += minStr;
    tempStr += ".";
    tempStr += patchStr;

    this->oled.draw_string(this->updateAppVersionPixels,SWL_OLED_POSITION_Y_POS_UPAPP_VERSION,tempStr,WHITE,BLACK);
    this->oled.draw_hline(SWL_OLED_POSITION_X_POS,SWL_OLED_POSITION_Y_POS_LINE,128,WHITE);
    this->oled.refresh(true);
}
/*==============================GW Update============================================================*/
void SWL_OLED::SwlOled::GwUpWriteStatus(etl::string<SWLOLED_MAX_STR_SIZE> str)
{
    etl::string<SWLOLED_MAX_STR_SIZE> tempStr;
    uint8_t x = 128 - this->gwAppStatusPixels;
    for(int i = 0; i < x; i++)
    {
        tempStr += " ";
    }
    this->oled.draw_string(this->gwAppStatusPixels,SWL_OLED_POSITION_Y_POS_GWUP_STATUS,tempStr,WHITE,BLACK);
    this->oled.draw_string(this->gwAppStatusPixels,SWL_OLED_POSITION_Y_POS_GWUP_STATUS,str,WHITE,BLACK);
    this->oled.refresh(true);
}
void SWL_OLED::SwlOled::GwUpWriteDevicesInfo(uint8_t foundDevices,uint8_t updatedDevices)
{

    etl::string<4> tempFoundDev;
    etl::string<4> tempUpdatedDev;
    etl::string<SWLOLED_MAX_STR_SIZE> tempStr;
    uint8_t x = 128 - this->gwAppDevInfoPixels;
    for(int i = 0; i < x; i++)
    {
        tempStr += " ";
    }
    this->oled.draw_string(this->gwAppDevInfoPixels,SWL_OLED_POSITION_Y_POS_GWUP_DEVICE,tempStr,WHITE,BLACK);
    this->intToString(foundDevices,tempFoundDev);
    this->intToString(updatedDevices,tempUpdatedDev);
    tempStr.clear();
    tempStr += tempUpdatedDev;
    tempStr += " / ";
    tempStr += tempFoundDev;
    this->oled.draw_string(this->gwAppDevInfoPixels,SWL_OLED_POSITION_Y_POS_GWUP_DEVICE,tempStr,WHITE,BLACK);
    this->oled.refresh(true);
}

void SWL_OLED::SwlOled::GwUpWriteProgess(uint8_t progress)
{

    etl::string<4u> tempProg;
    etl::string<SWLOLED_MAX_STR_SIZE> tempStr;
    uint8_t x = 128 - this->gwAppProgresPixels;
    for(int i = 0; i < x; i++)
    {
        tempStr += " ";
    }
    this->oled.draw_string(this->gwAppProgresPixels,SWL_OLED_POSITION_Y_POS_GWUP_PROGRESS,tempStr,WHITE,BLACK);
    this->intToString(progress,tempProg);
    tempStr.clear();
    tempStr += tempProg;
    tempStr += " / 100";
    this->oled.draw_string(this->gwAppProgresPixels,SWL_OLED_POSITION_Y_POS_GWUP_PROGRESS,tempStr,WHITE,BLACK);
    this->oled.refresh(true);
}

void SWL_OLED::SwlOled::intToString(uint32_t val, etl::string<4>&str)
{
    str.clear();
    if(val != 0)
    {
        while (val != 0)
        {
            uint8_t chr_val = val % 10;
            chr_val = chr_val + '0';
            val = val/10;
            str += chr_val;
        }
        int n = str.length(); 
        // Swap character starting from two 
        // corners 
        for (int i = 0; i < n / 2; i++)
        {
            char chr_val = str[i];
            str[i] = str[n - i - 1];
            str[n - i - 1] = chr_val;
        }
    }
    else
    {
        str += '0';
    }
}