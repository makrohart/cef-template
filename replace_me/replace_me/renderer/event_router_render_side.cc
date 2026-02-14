// Copyright (c) 2024 replace_me Authors. All rights reserved.

#include "replace_me/renderer/event_router_render_side.h"

#include <limits>
#include <map>
#include <set>
#include <string>

#include "include/base/cef_callback.h"
#include "include/base/cef_logging.h"
#include "include/cef_task.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "libcef_dll/wrapper/cef_browser_info_map.h"
#include "../common/event.h"

namespace {

    const char kEventMessageName[] = "CEFEventRouterMsg";

    // ID value reserved for internal use.
    const int kReservedId = 0;

    // Helper class for generating ID values.
    template <typename T>
    class IdGenerator {
    public:
        IdGenerator() : next_id_(kReservedId) {}

        IdGenerator(const IdGenerator&) = delete;
        IdGenerator& operator=(const IdGenerator&) = delete;

        T GetNextId() {
            if (next_id_ == std::numeric_limits<T>::max()) {
                next_id_ = kReservedId;
            }
            return ++next_id_;
        }

    private:
        T next_id_;
    };

    // Structure representing an event listener.
    struct EventListener {
        CefRefPtr<CefV8Value> handler;
        int listener_id;
        int context_id;
    };

    // Renderer-side router implementation.
    // TODO: Current event router only support global event handling across browser boundary.
    // We may consider support global, browser-oriented event handling from js to native and vice versa.
    class EventRouterRenderSideImpl : public CefMessageRouterRendererSide {
    public:
        class V8HandlerImpl : public CefV8Handler {
        public:
            V8HandlerImpl(CefRefPtr<EventRouterRenderSideImpl> router, const CefEventRouterConfig& config)
                : router_(router)
                , config_(config)
            {
            }

            V8HandlerImpl(const V8HandlerImpl&) = delete;
            V8HandlerImpl& operator=(const V8HandlerImpl&) = delete;

            bool Execute(const CefString& name,
                         CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& exception) override
            {
                CEF_REQUIRE_RENDERER_THREAD();

                if (name == config_.js_event_on_function) {
                    if (arguments.size() != 2
                        || !arguments[0]->IsString()
                        || !arguments[1]->IsFunction()) {
                        exception = "Invalid arguments; expecting (string, function)";
                        return true;
                    }

                    const std::string eventName = arguments[0]->GetStringValue();
                    CefRefPtr<CefV8Value> handler = arguments[1];

                    CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
                    const int id_render_side = router_->OnEvent(context->GetBrowser(), context->GetFrame(), eventName, handler);

                    retval = CefV8Value::CreateInt(id_render_side);
                    return true;
                }
                else if (name == config_.js_event_off_function) {
                    if (arguments.size() != 2
                        || !arguments[0]->IsString()
                        || !arguments[1]->IsInt()) {
                        exception = "Invalid arguments; expecting (string, int)";
                        return true;
                    }

                    const std::string eventName = arguments[0]->GetStringValue();
                    const int id_render_side = arguments[1]->GetIntValue();

                    CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
                    router_->OffEvent(context->GetBrowser(), context->GetFrame(), eventName, id_render_side);

                    retval = CefV8Value::CreateBool(true);
                    return true;
                }
                else if (name == config_.js_event_emit_function) {
                    if (arguments.size() != 2
                        || !arguments[0]->IsString()
                        || !arguments[1]->IsObject()) {
                        exception = "Invalid arguments; expecting (string, object)";
                        return true;
                    }

                    const std::string eventName = arguments[0]->GetStringValue();
                    CefRefPtr<CefV8Value> data = arguments[1];

                    CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
                    router_->EmitEvent(context->GetBrowser(), context->GetFrame(), /*fromBrowserSide=*/false, eventName, data);

                    retval = CefV8Value::CreateBool(true);
                    return true;
                }

                return false;
            }

        private:
            CefRefPtr<EventRouterRenderSideImpl> router_;
            const CefEventRouterConfig config_;

            IMPLEMENT_REFCOUNTING(V8HandlerImpl);
        };

        EventRouterRenderSideImpl(const CefEventRouterConfig& config)
            : config_(config)
        {
        }

        EventRouterRenderSideImpl(const EventRouterRenderSideImpl&) = delete;
        EventRouterRenderSideImpl& operator=(const EventRouterRenderSideImpl&) = delete;

        int GetPendingCount(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefV8Context> context) override {
            CEF_REQUIRE_RENDERER_THREAD();

            return 0;
        }

        void OnContextCreated(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefV8Context> context) override {
            CEF_REQUIRE_RENDERER_THREAD();

            // Register function handlers with the 'window' object.
            CefRefPtr<CefV8Value> window = context->GetGlobal();

            CefRefPtr<V8HandlerImpl> handler = new V8HandlerImpl(this, config_);
            CefV8Value::PropertyAttribute attributes =
                static_cast<CefV8Value::PropertyAttribute>(
                    V8_PROPERTY_ATTRIBUTE_READONLY | V8_PROPERTY_ATTRIBUTE_DONTENUM |
                    V8_PROPERTY_ATTRIBUTE_DONTDELETE);

            // Add the event functions.
            CefRefPtr<CefV8Value> on_func =
                CefV8Value::CreateFunction(config_.js_event_on_function, handler.get());
            window->SetValue(config_.js_event_on_function, on_func, attributes);

            CefRefPtr<CefV8Value> off_func =
                CefV8Value::CreateFunction(config_.js_event_off_function, handler.get());
            window->SetValue(config_.js_event_off_function, off_func, attributes);

            CefRefPtr<CefV8Value> emit_func =
                CefV8Value::CreateFunction(config_.js_event_emit_function, handler.get());
            window->SetValue(config_.js_event_emit_function, emit_func, attributes);
        }

        void OnContextReleased(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) override {
            CEF_REQUIRE_RENDERER_THREAD();
        }

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefProcessId source_process,
                                     CefRefPtr<CefProcessMessage> message) override {
            CEF_REQUIRE_RENDERER_THREAD();

            if (message->GetName() != config_.js_event_emit_function) {
                return false;
            }

            CefRefPtr<CefListValue> args = message->GetArgumentList();
            if (args->GetSize() < 2) {
                return false;
            }

            const std::string event_name = args->GetString(0);
            const CefRefPtr<CefV8Value> event_data = CefV8Value::CreateString(args->GetString(1));
            EmitEvent(browser, frame, /*fromBrowserSide=*/true, event_name, event_data);
            return true;
        }

    private:

        int OnEvent(CefRefPtr<CefBrowser> browser,
                    CefRefPtr<CefFrame> frame,
                    const std::string& event_name,
                    CefRefPtr<CefV8Value> handler) {
            CEF_REQUIRE_RENDERER_THREAD();

            CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();

            const int id_render_side = events_.on(event_name, [context, handler](CefRefPtr<CefV8Value> event_data) {
                CEF_REQUIRE_RENDERER_THREAD();

                CefV8ValueList args;
                args.push_back(event_data);
                handler->ExecuteFunctionWithContext(context, nullptr, args);
            });

            // Forward to browser side
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(config_.js_event_on_function);
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            args->SetString(0, event_name);
            args->SetInt(1, id_render_side);
            frame->SendProcessMessage(PID_BROWSER, message);

            return id_render_side;
        }

        void OffEvent(CefRefPtr<CefBrowser> browser,
                      CefRefPtr<CefFrame> frame,
                      const std::string& event_name,
                      const int id_render_side) {
            CEF_REQUIRE_RENDERER_THREAD();
            events_.off(event_name, id_render_side);

            // Forward to browser side
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(config_.js_event_off_function);
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            args->SetString(0, event_name);
            args->SetInt(1, id_render_side);
            frame->SendProcessMessage(PID_BROWSER, message);
        }

        void OffEvent(CefRefPtr<CefBrowser> browser,
                      CefRefPtr<CefFrame> frame,
                      const int id_render_side) {
            CEF_REQUIRE_RENDERER_THREAD();
            events_.off(id_render_side);

            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(config_.js_event_off_function);
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            args->SetInt(0, id_render_side);
            frame->SendProcessMessage(PID_BROWSER, message);
        }

        void EmitEvent(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       const bool fromBrowserSide,
                       const std::string& event_name,
                       CefRefPtr<CefV8Value> data) {
            CEF_REQUIRE_RENDERER_THREAD();

            events_.emit(event_name, data);

            if (fromBrowserSide)
                return;

            // Convert V8 value to JSON string.
            std::string event_data;
            if (data->IsString()) {
                event_data = data->GetStringValue();
            }
            else if (data->IsObject()) {
                // Try to stringify the object.
                CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
                CefRefPtr<CefV8Value> json = context->GetGlobal()->GetValue("JSON");
                if (json && json->IsObject()) {
                    CefRefPtr<CefV8Value> stringify = json->GetValue("stringify");
                    if (stringify && stringify->IsFunction()) {
                        CefV8ValueList args;
                        args.push_back(data);
                        CefRefPtr<CefV8Value> result =
                            stringify->ExecuteFunction(json, args);
                        if (result && result->IsString()) {
                            event_data = result->GetStringValue();
                        }
                    }
                }
            }

            // Forward to browser side
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(config_.js_event_emit_function);
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            args->SetString(0, event_name);
            args->SetString(1, event_data);
            frame->SendProcessMessage(PID_BROWSER, message);
        }

        const CefEventRouterConfig config_;
        event::Events<> events_;
    };

}  // namespace

CefEventRouterConfig::CefEventRouterConfig()
    : js_event_on_function("cefEventOn")
    , js_event_off_function("cefEventOff")
    , js_event_emit_function("cefEventEmit")
{
}

// static
CefRefPtr<CefMessageRouterRendererSide> EventRouterRenderSide::Create(const CefEventRouterConfig& config)
{
    return new EventRouterRenderSideImpl(config);
}