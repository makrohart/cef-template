#pragma once

#include <variant>

#include "include/wrapper/cef_message_router.h"
#include "libcef_dll/wrapper/cef_message_router_utils.h"
#include <libcef_dll/wrapper/cef_message_router_utils.h>

struct ProgressBrowserMessage {
    int context_id;
    int request_id;
    bool is_success;
    bool is_progress;
    int error_code;
    std::variant<CefString, CefRefPtr<CefBinaryBuffer>> payload;
};

struct RendererMessage {
    int context_id;
    int request_id;
    bool is_persistent;
    std::variant<CefString, CefRefPtr<const CefBinaryBuffer>> payload;
};

#ifndef CEF_V8_ENABLE_SANDBOX
class BinaryValueABRCallback final : public CefV8ArrayBufferReleaseCallback {
public:
    explicit BinaryValueABRCallback(CefRefPtr<CefBinaryBuffer> value)
        : value_(std::move(value)) {
    }
    BinaryValueABRCallback(const BinaryValueABRCallback&) = delete;
    BinaryValueABRCallback& operator=(const BinaryValueABRCallback&) = delete;

    void ReleaseBuffer(void* buffer) override {}

private:
    const CefRefPtr<CefBinaryBuffer> value_;

    IMPLEMENT_REFCOUNTING(BinaryValueABRCallback);
};
#endif

CefRefPtr<cef_message_router_utils::BrowserResponseBuilder> CreateBrowserResponseBuilder(
    size_t threshold,
    const std::string& name,
    const CefString& response);

CefRefPtr<cef_message_router_utils::BrowserResponseBuilder> CreateBrowserResponseBuilder(
    size_t threshold,
    const std::string& name,
    const void* data,
    size_t size);

ProgressBrowserMessage ParseBrowserMessage(const CefRefPtr<CefProcessMessage>& message);

RendererMessage ParseRendererMessage(const CefRefPtr<CefProcessMessage>& message);
