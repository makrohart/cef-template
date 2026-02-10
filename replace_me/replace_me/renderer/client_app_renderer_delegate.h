// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_RENDERER_CLIENT_APP_RENDERER_DELEGATE_H_
#define CEF_RENDERER_CLIENT_APP_RENDERER_DELEGATE_H_
#pragma once

#include "include/cef_base.h"
#include "include/wrapper/cef_message_router.h"
#include "replace_me/renderer/client_app_renderer.h"

namespace client::renderer {

    class ClientAppRenderDelegate : public ClientAppRenderer::Delegate {
    public:
        ClientAppRenderDelegate() = default;

        void OnWebKitInitialized(CefRefPtr<ClientAppRenderer> app) override;

        void OnContextCreated(CefRefPtr<ClientAppRenderer> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override;

        void OnContextReleased(CefRefPtr<ClientAppRenderer> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override;

        void OnFocusedNodeChanged(CefRefPtr<ClientAppRenderer> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefDOMNode> node) override;

        bool OnProcessMessageReceived(CefRefPtr<ClientAppRenderer> app,
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) override;

    private:
        bool last_node_is_editable_ = false;

        // Handles the renderer side of query routing.
        std::set<CefRefPtr<CefMessageRouterRendererSide>> message_routers_;

        DISALLOW_COPY_AND_ASSIGN(ClientAppRenderDelegate);
        IMPLEMENT_REFCOUNTING(ClientAppRenderDelegate);
    };

}  // namespace client::renderer

#endif  // CEF_RENDERER_CLIENT_APP_RENDERER_DELEGATE_H_
