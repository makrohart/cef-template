// Copyright (c) 2024 replace_me Authors. All rights reserved.

#ifndef REPLACE_ME_BROWSER_MESSAGE_HANDLER_H_
#define REPLACE_ME_BROWSER_MESSAGE_HANDLER_H_
#pragma once

#include <functional>
#include <string>

#include "include/wrapper/cef_message_router.h"

namespace client::message_handler {

    class MessageHandler : public CefMessageRouterBrowserSide::Handler {
    public:
        MessageHandler() = default;

        bool OnQuery(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            int64_t query_id,
            const CefString& request,
            bool persistent,
            CefRefPtr<Callback> callback) override;

    private:

        int OnQueryInternal(const CefString& request, CefString& response, CefString& error_message);
        
        // Handle file dialog request (async)
        void HandleFileDialog(CefRefPtr<CefBrowser> browser,
            const std::string& request,
            CefRefPtr<Callback> callback);

        DISALLOW_COPY_AND_ASSIGN(MessageHandler);
    };

}  // namespace client::message_handler

#endif  // REPLACE_ME_BROWSER_MESSAGE_HANDLER_H_

