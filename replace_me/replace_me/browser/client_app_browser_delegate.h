// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_BROWSER_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_BROWSER_H_
#pragma once

#include "include/cef_base.h"
#include "replace_me/browser/client_app_browser.h"

namespace client::browser {

    class ClientBrowserDelegate : public ClientAppBrowser::Delegate {
    public:
        ClientBrowserDelegate() = default;

        void OnRegisterCustomPreferences(
            CefRefPtr<client::ClientAppBrowser> app,
            cef_preferences_type_t type,
            CefRawPtr<CefPreferenceRegistrar> registrar) override;

        void OnContextInitialized(CefRefPtr<ClientAppBrowser> app) override;

        void OnBeforeCommandLineProcessing(
            CefRefPtr<ClientAppBrowser> app,
            CefRefPtr<CefCommandLine> command_line) override;

        bool OnAlreadyRunningAppRelaunch(
            CefRefPtr<ClientAppBrowser> app,
            CefRefPtr<CefCommandLine> command_line,
            const CefString& current_directory) override;

        CefRefPtr<CefClient> GetDefaultClient(
            CefRefPtr<ClientAppBrowser> app) override;

    private:
        DISALLOW_COPY_AND_ASSIGN(ClientBrowserDelegate);
        IMPLEMENT_REFCOUNTING(ClientBrowserDelegate);
    };

}  // namespace client::browser

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_BROWSER_H_
