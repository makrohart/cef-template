// Copyright (c) 2024 replace_me Authors. All rights reserved.

#include "replace_me/browser/event_router_browser_side.h"

#include <map>
#include <set>
#include <string>

#include "include/base/cef_logging.h"
#include "include/wrapper/cef_helpers.h"
#include "libcef_dll/wrapper/cef_browser_info_map.h"
#include "../common/event_notify.h"

namespace {
    // Appended to the JS function name for related IPC messages.
    const char kCefEventOn[] = "cefEventOn";
    const char kCefEventOff[] = "cefEventOff";
    const char kCefEventEmit[] = "cefEventEmit";

    // Browser-side router implementation.
    class EventRouterBrowserSideImpl : public CefMessageRouterBrowserSide {
    public:
        EventRouterBrowserSideImpl() = default;

        EventRouterBrowserSideImpl(const EventRouterBrowserSideImpl&) = delete;
        EventRouterBrowserSideImpl& operator=(const EventRouterBrowserSideImpl&) = delete;

        bool AddHandler(Handler* handler, bool first) override
        {
            return false;
        }

        bool RemoveHandler(Handler* handler) override
        {
            return false;
        }

        void CancelPending(CefRefPtr<CefBrowser> browser, Handler* handler) override
        {
        }

        int GetPendingCount(CefRefPtr<CefBrowser> browser, Handler* handler) override
        {
            return 0;
        }

        void OnBeforeClose(CefRefPtr<CefBrowser> browser) override
        {
            for (auto& [id_render_side, id_browser_side] : id_render_to_browser_side_map_)
                event::EventNotifier::getInstance().off(id_browser_side);
        }

        void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser) override
        {
            for (auto& [id_render_side, id_browser_side] : id_render_to_browser_side_map_)
                event::EventNotifier::getInstance().off(id_browser_side);
        }

        void OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) override
        {
            for (auto& [id_render_side, id_browser_side] : id_render_to_browser_side_map_)
                event::EventNotifier::getInstance().off(id_browser_side);
        }

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override
        {
            CEF_REQUIRE_UI_THREAD();

            const std::string& message_name = message->GetName();
            if (message_name == kCefEventOn) {
                CefRefPtr<CefListValue> args = message->GetArgumentList();
                DCHECK_EQ(args->GetSize(), 2U);

                const CefString& eventName = args->GetString(0);
                const int id_render_side = args->GetInt(1);

                const int id_browser_side = event::EventNotifier::getInstance().on(eventName, [this, browser, frame, eventName](std::string data) {
                    CefString event_data(data);
                    this->SendEmitEvent(browser, frame, eventName, event_data);
                });
                id_render_to_browser_side_map_.emplace(id_render_side, id_browser_side);
                return true;
            }
            else if (message_name == kCefEventOff) {
                CefRefPtr<CefListValue> args = message->GetArgumentList();
                if (args->GetSize() == 1U)
                {
                    const int id_render_side = args->GetInt(0);
                    if (id_render_to_browser_side_map_.count(id_render_side)) {
                        event::EventNotifier::getInstance().off(id_render_to_browser_side_map_[id_render_side]);
                        id_render_to_browser_side_map_.erase(id_render_side);
                    }
                                      
                    return true;
                }
                if (args->GetSize() == 2U)
                {
                    const CefString& eventName = args->GetString(0);
                    const int id_render_side = args->GetInt(1);
                    if (id_render_to_browser_side_map_.count(id_render_side)) {
                        event::EventNotifier::getInstance().off(eventName, id_render_to_browser_side_map_[id_render_side]);
                        id_render_to_browser_side_map_.erase(id_render_side);
                    }
                    
                    return true;
                }        
            }
            else if (message_name == kCefEventEmit) {
                CefRefPtr<CefListValue> args = message->GetArgumentList();
                DCHECK_EQ(args->GetSize(), 2U);

                const CefString& eventName = args->GetString(0);
                const CefString& event_data = args->GetString(1);
                event::EventNotifier::getInstance().emit(eventName, event_data.ToString());
                return true;
            }

            return false;
        }

    public:

        void SendEmitEvent(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& event_name, const CefString& event_data) {
            auto message = CefProcessMessage::Create(kCefEventEmit);
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            args->SetString(0, event_name);
            args->SetString(1, event_data);

            frame->SendProcessMessage(PID_RENDERER, message);
        }
        std::map<int, int> id_render_to_browser_side_map_;
    };

}  // namespace

// static
CefRefPtr<CefMessageRouterBrowserSide> EventRouterBrowserSide::Create() {
    return new EventRouterBrowserSideImpl();
}

