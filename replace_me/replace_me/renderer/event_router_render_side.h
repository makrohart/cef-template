// Copyright (c) 2024 replace_me Authors. All rights reserved.

#ifndef REPLACE_ME_RENDERER_EVENT_ROUTER_RENDER_SIDE_H_
#define REPLACE_ME_RENDERER_EVENT_ROUTER_RENDER_SIDE_H_
#pragma once

#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_process_message.h"
#include "include/cef_v8.h"
#include "include/wrapper/cef_message_router.h"

///
/// Used to configure the query router. The same values must be passed to both
/// CefMessageRouterBrowserSide and CefMessageRouterRendererSide. If using
/// multiple router pairs make sure to choose values that do not conflict.
///
struct CefEventRouterConfig {
    CefEventRouterConfig();

    ///
    /// Name of the JavaScript function that will be added to the 'window' object
    /// for sending a query. The default value is "cefEventOn".
    ///
    CefString js_event_on_function;

    ///
    /// Name of the JavaScript function that will be added to the 'window' object
    /// for canceling a pending query. The default value is "cefEventOff".
    ///
    CefString js_event_off_function;

    ///
    /// Name of the JavaScript function that will be added to the 'window' object
    /// for canceling a pending query. The default value is "cefEventEmit".
    ///
    CefString js_event_emit_function;
};

///
/// Implements the renderer side of event routing. The methods of this class
/// must be called on the render process main thread.
///
class EventRouterRenderSide : public base::RefCountedThreadSafe<EventRouterRenderSide>
{
public:
    ///
    /// Create a new router with the specified configuration.
    ///
    static CefRefPtr<CefMessageRouterRendererSide> Create(const CefEventRouterConfig& config);

protected:
    // Protect against accidental deletion of this object.
    friend class base::RefCountedThreadSafe<EventRouterRenderSide>;
    virtual ~EventRouterRenderSide() = default;
};

#endif  // REPLACE_ME_RENDERER_EVENT_ROUTER_RENDER_SIDE_H_
