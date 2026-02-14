#pragma once
#include "iservice.h"
#include "xpack.h"
#include "json.h"
#include "replace_me/common/event_notify.h"
#include <iostream>

namespace test
{
    struct TestInvokeReq
    {
        std::string info;
        XPACK(O(info));
    };

    struct TestInvokeResp
    {
        std::string result;
        XPACK(O(result));
    };

    struct TestInvokeErrorReq
    {
        std::string info;
        int error;
        XPACK(O(info, error));
    };

    struct TestEmitEventReq
    {
        std::string eventName;
        std::string data;
        XPACK(O(eventName, data));
    };

	class TestService : public IService
	{
	public:

		static TestService& getInstance()
		{
			static TestService s_instance;
			return s_instance;
		}

		int onQuery(const std::string& action, const std::string& request, std::string& response, std::string& message) override
		{
            if (action == "test:invoke")
            {
                TestInvokeReq req;
                xpack::json::decode(request, req);
                TestInvokeResp resp{ "success" };
                response = xpack::json::encode(resp);
                return 0;
            }
            else if (action == "test:invokeError")
            {
                TestInvokeErrorReq req;
                xpack::json::decode(request, req);
                message = req.info;
                return req.error;
            }
            else if (action == "test:emitEvent")
            {
                TestEmitEventReq req;
                xpack::json::decode(request, req);
                event::EventNotifier::getInstance().emit(req.eventName, req.data);
            }

            return 0;
		}
	};
}