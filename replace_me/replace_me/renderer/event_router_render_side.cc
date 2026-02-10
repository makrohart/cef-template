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
                , context_id_(kReservedId)
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
                    const int listener_id = router_->RegisterListener(eventName, handler);

                    retval = CefV8Value::CreateInt(listener_id);
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
                    const int listener_id = arguments[1]->GetIntValue();

                    CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
                    router_->UnregisterListener(eventName, listener_id);

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
                    router_->EmitEvent(context->GetFrame(), eventName, data);

                    retval = CefV8Value::CreateBool(true);
                    return true;
                }

                return false;
            }

        private:
            // Don't create the context ID until it's actually needed.
            int GetIDForContext(CefRefPtr<CefV8Context> context) {
                if (context_id_ == kReservedId) {
                    context_id_ = router_->CreateIDForContext(context);
                }
                return context_id_;
            }

            CefRefPtr<EventRouterRenderSideImpl> router_;
            const CefEventRouterConfig config_;
            int context_id_;

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

            // Store context for later use.
            const int context_id = GetIDForContext(context);
            context_map_[context_id] = context;
        }

        void OnContextReleased(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) override {
            CEF_REQUIRE_RENDERER_THREAD();

            // Get the context ID and remove the context from the map.
            const int context_id = GetIDForContext(context, true);
            if (context_id != kReservedId) {
                // Remove all listeners for this context.
                RemoveListenersForContext(context_id);
                context_map_.erase(context_id);
            }
        }

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefProcessId source_process,
                                     CefRefPtr<CefProcessMessage> message) override {
            CEF_REQUIRE_RENDERER_THREAD();

            if (message->GetName() != kEventMessageName) {
                return false;
            }

            CefRefPtr<CefListValue> args = message->GetArgumentList();
            if (args->GetSize() < 2) {
                return false;
            }

            const std::string event_name = args->GetString(0);
            const std::string event_data = args->GetString(1);

            CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();

            // Parse JSON data if possible.
            CefRefPtr<CefV8Value> data_value;
            CefRefPtr<CefV8Value> json = context->GetGlobal()->GetValue("JSON");
            if (json && json->IsObject()) {
                CefRefPtr<CefV8Value> parse = json->GetValue("parse");
                if (parse && parse->IsFunction()) {
                    CefV8ValueList parse_args;
                    parse_args.push_back(CefV8Value::CreateString(event_data));
                    CefRefPtr<CefV8Value> parsed =
                        parse->ExecuteFunction(json, parse_args);
                    if (parsed) {
                        data_value = parsed;
                    }
                }
            }
            if (!data_value) {
                data_value = CefV8Value::CreateString(event_data);
            }

            // Trigger all listeners for this event.
            TriggerEvent(event_name, data_value);

            return true;
        }

    private:
        int CreateIDForContext(CefRefPtr<CefV8Context> context) {
            CEF_REQUIRE_RENDERER_THREAD();

            // The context should not already have an associated ID.
            DCHECK_EQ(GetIDForContext(context, false), kReservedId);

            const int context_id = context_id_generator_.GetNextId();
            context_map_.insert(std::make_pair(context_id, context));
            return context_id;
        }

        // Retrieves the existing ID value associated with the specified |context|.
        // If |remove| is true the context will also be removed from the map.
        int GetIDForContext(CefRefPtr<CefV8Context> context, bool remove = false) {
            CEF_REQUIRE_RENDERER_THREAD();

            ContextMap::iterator it = context_map_.begin();
            for (; it != context_map_.end(); ++it) {
                if (it->second->IsSame(context)) {
                    int context_id = it->first;
                    if (remove) {
                        context_map_.erase(it);
                    }
                    return context_id;
                }
            }

            return kReservedId;
        }

        CefRefPtr<CefV8Context> GetContextByID(int context_id) {
            CEF_REQUIRE_RENDERER_THREAD();

            ContextMap::const_iterator it = context_map_.find(context_id);
            if (it != context_map_.end()) {
                return it->second;
            }
            return nullptr;
        }

        int RegisterListener(const std::string& event_name, CefRefPtr<CefV8Value> handler) {
            CEF_REQUIRE_RENDERER_THREAD();

            const int context_id = GetIDForContext(CefV8Context::GetCurrentContext());
            const int listener_id = listener_id_generator_.GetNextId();
            listener_map_.emplace(listener_id, EventListener{ handler, listener_id, context_id });

            return listener_id;
        }

        void UnregisterListener(const std::string& event_name,
            int listener_id) {
            CEF_REQUIRE_RENDERER_THREAD();

            if (listener_map_.count(event_name) && listener_map_[event_name].count(listener_id)) {
                listener_map_[event_name].erase(listener_id);
            }
        }

        void EmitEvent(CefRefPtr<CefFrame> frame,
                       const std::string& event_name,
                       CefRefPtr<CefV8Value> data) {
            CEF_REQUIRE_RENDERER_THREAD();

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

            // Trigger event on render side.
            TriggerEvent(event_name, data);

            // Send event to browser process.
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(kEventMessageName);
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            args->SetString(0, event_name);
            args->SetString(1, event_data);
            frame->SendProcessMessage(PID_BROWSER, message);
        }

        void TriggerEvent(const std::string& event_name, CefRefPtr<CefV8Value> event_data) {
            CEF_REQUIRE_RENDERER_THREAD();

            if (!listener_map_.count(event_name)) {
                return;
            }

            // Find all listeners for this event.
            for (auto& [listener_id, listener] : listener_map_[event_name]) {
                CefRefPtr<CefV8Context> context = GetContextForListener(listener.context_id);
                if (context && listener.handler) {
                    CefV8ValueList args;
                    args.push_back(event_data);
                    listener.handler->ExecuteFunctionWithContext(context, nullptr, args);
                }
            }

        }

        CefRefPtr<CefV8Context> GetContextForListener(const int context_id) {
            if (auto find = context_map_.find(context_id); find != context_map_.cend())
                return find->second;
            return nullptr;
        }

        void RemoveListenersForContext(int context_id) {
            CEF_REQUIRE_RENDERER_THREAD();

            for (auto& [event_name, listenerMap] : listener_map_)
            {
                for (auto it = listenerMap.begin(); it != listenerMap.end();)
                {
                    if (it->second.context_id == context_id)
                        it = listenerMap.erase(it);
                    else
                        ++it;
                }
            }
        }

        const CefEventRouterConfig config_;

        IdGenerator<int> context_id_generator_;
        IdGenerator<int> listener_id_generator_;

        // Map of context ID to CefV8Context for existing contexts. An entry is added
        // when a bound function is executed for the first time in the context and
        // removed when the context is released.
        using ContextMap = std::map<int, CefRefPtr<CefV8Context>>;
        ContextMap context_map_;

        // Map of listener ID to EventListener.
        using ListenerMap = std::map<int, EventListener>;
        // Map of eventName to ListenerMap.
        using EventListenerMap = std::map<std::string, ListenerMap>;
        EventListenerMap listener_map_;
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