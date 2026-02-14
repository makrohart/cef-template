// Copyright (c) 2024 replace_me Authors. All rights reserved.

#ifndef REPLACE_ME_BROWSER_EVENT_ROUTER_BROWSER_SIDE_H_
#define REPLACE_ME_BROWSER_EVENT_ROWSER_SIDE_H_
#pragma once

#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_process_message.h"
#include "include/wrapper/cef_message_router.h"

///
/// Implements the browser side of event routing. The methods of this class
/// must be called on the browser process UI thread.
///
class EventRouterBrowserSide : public base::RefCountedThreadSafe<EventRouterBrowserSide> {
public:
    ///
    /// Create a new router with the specified configuration.
    ///
    static CefRefPtr<CefMessageRouterBrowserSide> Create();

protected:
    // Protect against accidental deletion of this object.
    friend class base::RefCountedThreadSafe<EventRouterBrowserSide>;
    virtual ~EventRouterBrowserSide() = default;
};

#endif  // REPLACE_ME_BROWSER_EVENT_ROUTER_BROWSER_SIDE_H_
