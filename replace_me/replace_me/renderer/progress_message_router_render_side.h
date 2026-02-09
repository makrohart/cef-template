#pragma once

#include "include/wrapper/cef_message_router.h"

///
/// Implements the renderer side of query routing. The methods of this class
/// must be called on the render process main thread.
///
class ProgressMessageRouterRendererSide : public base::RefCountedThreadSafe<ProgressMessageRouterRendererSide> {
public:
    ///
    /// Create a new router with the specified configuration.
    ///
    static CefRefPtr<CefMessageRouterRendererSide> Create(const CefMessageRouterConfig& config);

protected:
    // Protect against accidental deletion of this object.
    friend class base::RefCountedThreadSafe<ProgressMessageRouterRendererSide>;
    virtual ~ProgressMessageRouterRendererSide() = default;
};