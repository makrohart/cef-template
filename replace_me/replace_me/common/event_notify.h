#pragma once
#include "notify.h"
#include "event.h"
#include <unordered_set>

namespace event
{
    struct EventNotifier : event::Events<>
    {
        static EventNotifier& getInstance()
        {
            static EventNotifier s_eventNotifier;
            return s_eventNotifier;
        }
       
    private:
        EventNotifier() = default;
        ~EventNotifier() = default;
    };
}