// Copyright (c) 2024 replace_me Authors. All rights reserved.

#include "replace_me/browser/message_handler.h"

#include <sstream>
#include "include/base/cef_logging.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_values.h"
#include "include/cef_parser.h"
#include "include/cef_dialog_handler.h"
#include "replace_me/services/setting_service/SettingService.h"
#include "replace_me/services/MessageTypes.h"

namespace client::message_handler
{
    bool MessageHandler::OnQuery(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        int64_t query_id,
        const CefString& request,
        bool persistent,
        CefRefPtr<Callback> callback)
    {
        CEF_REQUIRE_UI_THREAD();

        replace_me::json::CefQueryMessage queryMessage;
        xpack::json::decode(request.ToString(), queryMessage);

        // Handle file dialog requests asynchronously
        if (queryMessage.action == "cef:selectFolder")
        {
            HandleFileDialog(browser, queryMessage.request, callback);
        }
        else
        {
            // Handle other requests synchronously
            CefString response;
            CefString errorMessage;
            if (int errorCode = OnQueryInternal(request, response, errorMessage); errorCode == 0)
                callback->Success(response);
            else
                callback->Failure(errorCode, errorMessage);
        }

        return true;
    }

    int MessageHandler::OnQueryInternal(const CefString& request, CefString& response, CefString& error_message)
    {

        std::string resp;
        std::string errorMsg;
        int ret = 0;

        replace_me::json::CefQueryMessage queryMessage;
        xpack::json::decode(request.ToString(), queryMessage);

        if (queryMessage.action.find("setting:") != std::string::npos)
        {
            ret = replace_me::services::SettingService::getInstance().onQuery(queryMessage.action, queryMessage.request, resp, errorMsg);
            response = std::move(resp);
            error_message = std::move(errorMsg);
            return ret;
        }

        ret = -1;
        error_message = "Unknown action: " + queryMessage.action;
        return ret;
    }

    void MessageHandler::HandleFileDialog(CefRefPtr<CefBrowser> browser, const std::string& request, CefRefPtr<Callback> callback)
    {
        CEF_REQUIRE_UI_THREAD();
        
        replace_me::json::CefFileDialogRequest req;
        if (!request.empty())
        {
            xpack::json::decode(request, req);
        }

        if (req.title.empty())
        {
            req.title = "Select Repository Folder";
        }

        // Create file dialog callback
        class FileDialogCallback : public CefRunFileDialogCallback {
        public:
            FileDialogCallback(CefRefPtr<Callback> query_callback) : query_callback_(query_callback) {}

            void OnFileDialogDismissed(const std::vector<CefString>& file_paths) override
            {
                CEF_REQUIRE_UI_THREAD();

                replace_me::json::CefFileDialogResponse resp;
                if (!file_paths.empty())
                {
                    resp.selectedPath = file_paths[0];
                }

                std::string response = xpack::json::encode(resp);
                query_callback_->Success(response);
            }

        private:
            CefRefPtr<Callback> query_callback_;
            IMPLEMENT_REFCOUNTING(FileDialogCallback);
        };

        CefRefPtr<FileDialogCallback> file_callback = new FileDialogCallback(callback);

        // Run file dialog
        browser->GetHost()->RunFileDialog(static_cast<cef_file_dialog_mode_t>(req.mode),
                                          req.title,
                                          req.defaultPath,
                                          std::vector<CefString>(),
                                          file_callback);
    }
}