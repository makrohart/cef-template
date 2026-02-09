#pragma once

#include <string>

namespace replace_me::services
{
    class IService
    {
        public:
        
        virtual int onQuery(const std::string& action, const std::string& request, std::string& response, std::string& message) = 0;
    };
}