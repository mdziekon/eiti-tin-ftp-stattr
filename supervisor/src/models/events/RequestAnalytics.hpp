#ifndef TIN_SUPERVISOR_SRC_EVENTS_REQUESTANALYTICS_HPP
#define TIN_SUPERVISOR_SRC_EVENTS_REQUESTANALYTICS_HPP

#include <string>
#include <sys/types.h>

#include "../Event.hpp"
#include "../../utils/JSON.hpp"

namespace tin { namespace supervisor { namespace models { namespace events
{
    struct RequestAnalytics: public tin::supervisor::models::Event
    {
        const tin::utils::json::ptr requestData;
        
        virtual void accept(tin::supervisor::models::StatsVisitor& visitor);
        
        RequestAnalytics(const tin::utils::json::ptr& requestData);
    };
}}}}

#endif  /* TIN_SUPERVISOR_SRC_EVENTS_REQUESTANALYTICS_HPP */