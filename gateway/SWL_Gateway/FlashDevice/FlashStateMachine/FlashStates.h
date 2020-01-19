#ifndef FLASHSTATES_H_
#define FLASHSTATES_H_

#include "../../etl/fsm.h"
#include "FlashEvents.h"

using namespace etl;
namespace SWL_GATEWAY
{
    struct StateId
    {
        enum
        {
            DISCONNECTED,
            CONNETED,
            SERVICE_DISCOVERED,
            VERSION_RECEIVED,
            SEND_BLOCK,
            DELETE,
            FLASHED_COMPLETE,
            MAX_STATES
        };
    };
    class FlashDevice;
    class ConnectedState : public etl::fsm_state<FlashDevice, ConnectedState, StateId::CONNETED, ServiceDiscoveredEvt,TimeoutEvt>
    {
    public:

    //***********************************
        fsm_state_id_t on_event(etl::imessage_router& sender, const ServiceDiscoveredEvt& event);
        fsm_state_id_t on_event(etl::imessage_router& sender, const TimeoutEvt& event);
    //***********************************
    fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event);
    };


    class DisconnectedState : public etl::fsm_state<FlashDevice, DisconnectedState, StateId::DISCONNECTED, ConnectEvt>
    {
        public:

        //**s*********************************
        fsm_state_id_t on_event(etl::imessage_router& sender, const ConnectEvt& event);
        //***********************************
        fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event);
    };

    class ServerDiscoveredState : public etl::fsm_state<FlashDevice, ServerDiscoveredState, StateId::SERVICE_DISCOVERED, VersionRespEvt>
    {
    public:

        //***********************************
        fsm_state_id_t on_event(etl::imessage_router& sender, const VersionRespEvt& event);
        //***********************************
        fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event);
    };


    class VersionReceivedState : public etl::fsm_state<FlashDevice, VersionReceivedState, StateId::VERSION_RECEIVED, PrepReadyEvt,TimeoutEvt, CheckDevReadyEvt>
    {
        public:

        //***********************************
        fsm_state_id_t on_event(etl::imessage_router& sender, const PrepReadyEvt& event);
        fsm_state_id_t on_event(etl::imessage_router& sender, const TimeoutEvt& event);
        fsm_state_id_t on_event(etl::imessage_router& sender, const CheckDevReadyEvt& event);
        //***********************************
        fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event);
    };

    class SendBlockState : public etl::fsm_state<FlashDevice, 
    SendBlockState, StateId::SEND_BLOCK, 
    NextBlockEvt,TimeoutEvt,DeleteEvt,
    CheckBlockStatusEvt,
    FlashedCompleteEvt>{
        public:
        //***********************************
        fsm_state_id_t on_event(etl::imessage_router& sender, const NextBlockEvt& event);
        fsm_state_id_t on_event(etl::imessage_router& sender, const TimeoutEvt& event);
        fsm_state_id_t on_event(etl::imessage_router& sender, const CheckBlockStatusEvt& event);
        fsm_state_id_t on_event(etl::imessage_router& sender, const DeleteEvt& event);
        fsm_state_id_t on_event(etl::imessage_router& sender, const FlashedCompleteEvt& event);
        //***********************************
        fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event);
    };

    class DeleteState : public etl::fsm_state<FlashDevice, DeleteState, StateId::DELETE>
    {
        public:

        //**********************************
        //***********************************
        fsm_state_id_t on_enter_state();
        fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event);
    };

    class FlashedCompleteState : public etl::fsm_state<FlashDevice, FlashedCompleteState, StateId::FLASHED_COMPLETE,FlashedCompleteRespEvt>
    {
        public:

        //**********************************
        fsm_state_id_t on_event(etl::imessage_router& sender, const FlashedCompleteRespEvt& event);
        //***********************************
        fsm_state_id_t on_event_unknown(etl::imessage_router& sender, const etl::imessage& event);
    };

    
}
#endif