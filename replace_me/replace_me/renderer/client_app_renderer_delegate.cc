// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "replace_me/renderer/client_app_renderer_delegate.h"

#include <sstream>
#include <string>

#include "include/cef_crash_util.h"
#include "include/cef_dom.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"
#include "progress_message_router_render_side.h"
#include "replace_me/renderer/event_router_render_side.h"

namespace client::renderer {

	namespace
    {
		// Must match the value in client_handler.cc.
		const char kFocusedNodeChangedMessage[] = "ClientRenderer.FocusedNodeChanged";

	}

	void client::renderer::ClientAppRenderDelegate::OnWebKitInitialized(CefRefPtr<ClientAppRenderer> app)
    {
        if (CefCrashReportingEnabled()) {
            // Set some crash keys for testing purposes. Keys must be defined in the
            // "crash_reporter.cfg" file. See cef_crash_util.h for details.
            CefSetCrashKeyValue("testkey_small1", "value1_small_renderer");
            CefSetCrashKeyValue("testkey_small2", "value2_small_renderer");
            CefSetCrashKeyValue("testkey_medium1", "value1_medium_renderer");
            CefSetCrashKeyValue("testkey_medium2", "value2_medium_renderer");
            CefSetCrashKeyValue("testkey_large1", "value1_large_renderer");
            CefSetCrashKeyValue("testkey_large2", "value2_large_renderer");
        }

        // Create the renderer-side router for query handling.
        // message_routers_.insert(CefMessageRouterRenderSide::Create(CefMessageRouterConfig{}));
        message_routers_.insert(ProgressMessageRouterRendererSide::Create(CefMessageRouterConfig{}));
        message_routers_.insert(EventRouterRenderSide::Create(CefEventRouterConfig{}));
    }
    void ClientAppRenderDelegate::OnContextCreated(CefRefPtr<ClientAppRenderer> app, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
    {
        for (auto& message_router : message_routers_)
            message_router->OnContextCreated(browser, frame, context);
    }
    void ClientAppRenderDelegate::OnContextReleased(CefRefPtr<ClientAppRenderer> app, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
    {
        for (auto& message_router : message_routers_)
            message_router->OnContextReleased(browser, frame, context);
    }
    void ClientAppRenderDelegate::OnFocusedNodeChanged(CefRefPtr<ClientAppRenderer> app, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
    {
        bool is_editable = (node.get() && node->IsEditable());
        if (is_editable != last_node_is_editable_) {
            // Notify the browser of the change in focused element type.
            last_node_is_editable_ = is_editable;
            CefRefPtr<CefProcessMessage> message =
                CefProcessMessage::Create(kFocusedNodeChangedMessage);
            message->GetArgumentList()->SetBool(0, is_editable);
            frame->SendProcessMessage(PID_BROWSER, message);
        }
    }
    bool ClientAppRenderDelegate::OnProcessMessageReceived(CefRefPtr<ClientAppRenderer> app, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
    {
        // Try message router first.
        for (auto& message_router : message_routers_)
        {
            if (message_router->OnProcessMessageReceived(browser, frame, source_process, message))
            {
                return true;
            }
        }

        return false;
    }
	// namespace
}  // namespace client::renderer
