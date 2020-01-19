#ifndef FLASHEVENTS_H_
#define FLASHEVENTS_H_

#include "../../etl/fsm.h"
#include "esp_gatt_defs.h"
using namespace etl;

namespace SWL_GATEWAY
{


    struct EventId
    {
        enum
        {
            CONNECT,
            DISCONNET,
            SERVICE_DISCOVERED,
            VERSION_RESP,
            CHECK_DEV_READY,
            PREP_READY,
            NEXT_BLOCK,
            CHECK_BLOCK_STATUS,
            FLASHED_COMPLETED,
            FLASHED_COMPLETED_RESP,
            TIMEOUT,
            DELETE,
        };
    };



    class ConnectEvt : public message<EventId::CONNECT>
    {
        public:
        	esp_gatt_status_t status;       /*!< Operation status */
		    uint16_t conn_id;               /*!< Connection id */
		    uint16_t mtu;
            ConnectEvt(       	esp_gatt_status_t status,       /*!< Operation status */
		    uint16_t conn_id,               /*!< Connection id */
		    uint16_t mtu):
            status(status),
            conn_id(conn_id),
            mtu(mtu)
            {

            }
    };

    class DisconnectEvt : public message<EventId::DISCONNET>
    {
    };

    
    class FlashedCompleteEvt : public message<EventId::FLASHED_COMPLETED>
    {
    };

    class FlashedCompleteRespEvt : public message<EventId::FLASHED_COMPLETED_RESP>
    {
    };

    class ServiceDiscoveredEvt : public message<EventId::SERVICE_DISCOVERED>
    {
    };

    class VersionRespEvt : public message<EventId::VERSION_RESP>
    {
        public:
            uint32_t version;
            VersionRespEvt(uint32_t version):
            version(version){}
    };

    class PrepReadyEvt : public message<EventId::PREP_READY>
    {
    };

    class NextBlockEvt : public message<EventId::NEXT_BLOCK>
    {
    };

    class CheckBlockStatusEvt : public message<EventId::CHECK_BLOCK_STATUS>
    {
    };

    class CheckDevReadyEvt : public message<EventId::CHECK_DEV_READY>
    {
    };


    class TimeoutEvt : public message<EventId::TIMEOUT>
    {
    };

    class DeleteEvt : public message<EventId::DELETE>
    {
    };

}

#endif