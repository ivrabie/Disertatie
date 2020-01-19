#include "FlashStates.h"
#include "../FlashDevice.h"
#include "esp_log.h"
using namespace SWL_GATEWAY;
#define FLASH_STATES "FlashStates"
/******* Start Conected state **************/ 

  //***********************************
  fsm_state_id_t ConnectedState::on_event(etl::imessage_router& sender, const ServiceDiscoveredEvt& event)
  {
    FlashDevice &dev = this->get_fsm_context();
    ESP_LOGI(FLASH_STATES, "Version requested");
    dev.RequestVersion();

    return StateId::SERVICE_DISCOVERED;
  }
    fsm_state_id_t ConnectedState::on_event(etl::imessage_router& sender, const TimeoutEvt& event)
  {
    return StateId::DISCONNECTED;
  }

  //***********************************
  fsm_state_id_t ConnectedState::on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
  {
    this->get_fsm_context().LogUnknownEvent(event);
    return STATE_ID;
  }

/******* End Conected state **************/ 


/******* Start disconnect state **************/ 

  fsm_state_id_t DisconnectedState::on_event(etl::imessage_router& sender, const ConnectEvt& event)
  {
    FlashDevice &dev = this->get_fsm_context();
    dev.UpdateField(event.status,DEVICE_UPDATEFIELD_CONECTION_STATUS);
    dev.UpdateField(event.conn_id,DEVICE_UPDATEFIELD_CONECTION_ID);
    dev.UpdateField(event.mtu,DEVICE_UPDATEFIELD_CONECTION_MTU);
    ESP_LOGI(FLASH_STATES, "Conn id: %d \n"
                    "Conn status %d\n"
                    "Conn mtu %d",event.conn_id,
                    event.status,
                    event.mtu);
    return StateId::CONNETED;
  }
  //***********************************
  fsm_state_id_t DisconnectedState::on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
  {
    this->get_fsm_context().LogUnknownEvent(event);
    return STATE_ID;
  }

  /******* End disconnect state **************/

/******* Start service discovered state **************/ 
    //***********************************
  fsm_state_id_t ServerDiscoveredState::on_event(etl::imessage_router& sender, const VersionRespEvt& event)
  {
    FlashDevice &dev = this->get_fsm_context();
    if(event.version < dev.GetCurrentVersion() || event.version == FLASH_DEFAULT)
    {
      dev.SendFileSizeAndVersion();
      return StateId::VERSION_RECEIVED;
    }
    else
    {
      dev.Disconect();
      return StateId::DELETE;
    }
  }
  //***********************************
  fsm_state_id_t ServerDiscoveredState::on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
  {
    this->get_fsm_context().LogUnknownEvent(event);
    return STATE_ID;
  }

  /******* End service discovered state **************/ 

/********** Start version received state ***********/
    //***********************************
fsm_state_id_t VersionReceivedState::on_event(etl::imessage_router& sender, const PrepReadyEvt& event)
{
    FlashDevice &dev = this->get_fsm_context();
    dev.SendBlock();
    return StateId::SEND_BLOCK;
}
fsm_state_id_t VersionReceivedState::on_event(etl::imessage_router& sender, const TimeoutEvt& event)
{
    return StateId::DISCONNECTED;
}

fsm_state_id_t VersionReceivedState::on_event(etl::imessage_router& sender, const CheckDevReadyEvt& event)
{
    FlashDevice &dev = this->get_fsm_context();
    dev.RequestIsDevReady();
    return StateId::VERSION_RECEIVED;
}

  //***********************************
fsm_state_id_t VersionReceivedState::on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
{
    this->get_fsm_context().LogUnknownEvent(event);
    return STATE_ID;
}

/********** End version received state ***********/

/********** Start send blockd state ***********/

//***********************************
fsm_state_id_t SendBlockState::on_event(etl::imessage_router& sender, const NextBlockEvt& event)
{
  FlashDevice &dev = this->get_fsm_context();
  dev.SendBlock();
  return StateId::SEND_BLOCK;
}
fsm_state_id_t SendBlockState::on_event(etl::imessage_router& sender, const TimeoutEvt& event)
{
    return StateId::DISCONNECTED;
}
fsm_state_id_t SendBlockState::on_event(etl::imessage_router& sender, const DeleteEvt& event)
{
    return StateId::DELETE;
}

fsm_state_id_t SendBlockState::on_event(etl::imessage_router& sender, const CheckBlockStatusEvt& event)
{
  FlashDevice &dev = this->get_fsm_context();
  dev.RequestBlockStatus();
  return StateId::SEND_BLOCK;
}

fsm_state_id_t SendBlockState::on_event(etl::imessage_router& sender, const FlashedCompleteEvt& event)
{
  FlashDevice &dev = this->get_fsm_context();
  dev.ValidateFlashing();
  return StateId::FLASHED_COMPLETE;
}

//***********************************
fsm_state_id_t SendBlockState::on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
{
    
    this->get_fsm_context().LogUnknownEvent(event);
    return STATE_ID;
}

  

/********** Start delete state ***********/

fsm_state_id_t DeleteState::on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
{
    this->get_fsm_context().LogUnknownEvent(event);
    return STATE_ID;
}

fsm_state_id_t DeleteState::on_enter_state()
{
  return STATE_ID;
}


/********** End delete state ***********/

/********** Start flashed complete state ***********/

fsm_state_id_t FlashedCompleteState::on_event(etl::imessage_router& sender, const FlashedCompleteRespEvt& event)
{
  return StateId::DELETE;
}

fsm_state_id_t FlashedCompleteState::on_event_unknown(etl::imessage_router& sender, const etl::imessage& event)
{
    this->get_fsm_context().LogUnknownEvent(event);
    return STATE_ID;
}

/********** End flashed complete state ***********/