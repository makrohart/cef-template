// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "tests/shared/browser/resource_util.h"

#include "include/base/cef_logging.h"
#include "include/cef_stream.h"
#include "include/wrapper/cef_byte_read_handler.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "tests/shared/browser/util_win.h"
#include <filesystem>
#include <fstream>

namespace client {

namespace {

bool LoadBinaryResource(int binaryId, DWORD& dwSize, LPBYTE& pBytes) {
  HINSTANCE hInst = GetCodeModuleHandle();
  HRSRC hRes =
      FindResource(hInst, MAKEINTRESOURCE(binaryId), MAKEINTRESOURCE(256));
  if (hRes) {
    HGLOBAL hGlob = LoadResource(hInst, hRes);
    if (hGlob) {
      dwSize = SizeofResource(hInst, hRes);
      pBytes = (LPBYTE)LockResource(hGlob);
      if (dwSize > 0 && pBytes) {
        return true;
      }
    }
  }

  return false;
}

// Provider of binary resources.
class BinaryResourceProvider : public CefResourceManager::Provider {
 public:
  BinaryResourceProvider(const std::string& url_path,
                         const std::string& resource_path_prefix)
      : url_path_(url_path), resource_path_prefix_(resource_path_prefix) {
    DCHECK(!url_path.empty());
    if (!resource_path_prefix_.empty() &&
        resource_path_prefix_[resource_path_prefix_.length() - 1] != '/') {
      resource_path_prefix_ += "/";
    }
  }

  bool OnRequest(scoped_refptr<CefResourceManager::Request> request) override {
    CEF_REQUIRE_IO_THREAD();

    const std::string& url = request->url();
    if (url.find(url_path_) != 0L) {
      // Not handled by this provider.
      return false;
    }

    CefRefPtr<CefResourceHandler> handler;

    std::string relative_path = url.substr(url_path_.length());
    if (!relative_path.empty()) {
      if (!resource_path_prefix_.empty()) {
        relative_path = resource_path_prefix_ + relative_path;
      }

      CefRefPtr<CefStreamReader> stream =
          GetBinaryResourceReader(relative_path.data());
      if (stream.get()) {
        handler = new CefStreamResourceHandler(
            request->mime_type_resolver().Run(url), stream);
      }
    }

    request->Continue(handler);
    return true;
  }

 private:
  std::string url_path_;
  std::string resource_path_prefix_;

  DISALLOW_COPY_AND_ASSIGN(BinaryResourceProvider);
};

// Provider of directory resources from file system.
class DirectoryResourceProvider : public CefResourceManager::Provider {
 public:
  DirectoryResourceProvider(const std::string& url_path,
                            const std::string& directory_path)
      : url_path_(url_path), directory_path_(directory_path) {
    DCHECK(!url_path.empty());
    DCHECK(!directory_path.empty());
    // Normalize directory path
    std::filesystem::path dir_path(directory_path);
    directory_path_ = std::filesystem::absolute(dir_path).string();
    // Normalize path separators for Windows
    std::replace(directory_path_.begin(), directory_path_.end(), '\\', '/');
    if (directory_path_[directory_path_.length() - 1] != '/') {
      directory_path_ += "/";
    }
  }

  bool OnRequest(scoped_refptr<CefResourceManager::Request> request) override {
    CEF_REQUIRE_IO_THREAD();

    const std::string& url = request->url();
    if (url.find(url_path_) != 0L) {
      // Not handled by this provider.
      return false;
    }

    CefRefPtr<CefResourceHandler> handler;

    std::string relative_path = url.substr(url_path_.length());
    // Remove leading slash if present
    if (!relative_path.empty() && relative_path[0] == '/') {
      relative_path = relative_path.substr(1);
    }
    // Remove trailing slash if present (e.g., "index.html/" -> "index.html")
    if (!relative_path.empty() && relative_path[relative_path.length() - 1] == '/') {
      relative_path = relative_path.substr(0, relative_path.length() - 1);
    }
    
    if (!relative_path.empty()) {
      // Build full file path using filesystem::path for proper handling
      std::filesystem::path dir_path(directory_path_);
      std::filesystem::path rel_path(relative_path);
      // Normalize path separators by using filesystem::path
      std::filesystem::path fs_path = dir_path / rel_path;
      fs_path = fs_path.lexically_normal();
      
      // Check if file exists (use try-catch to handle invalid paths gracefully)
      try {
        if (std::filesystem::exists(fs_path) && std::filesystem::is_regular_file(fs_path)) {
          // Read file content
          std::string file_path = fs_path.string();
          std::ifstream file(file_path, std::ios::binary);
          if (file.is_open()) {
            // Get file size
            file.seekg(0, std::ios::end);
            size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            // Read file content
            std::vector<char> buffer(file_size);
            file.read(buffer.data(), file_size);
            file.close();
            
            if (file.good()) {
              // Create stream from file content
              CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForHandler(
                  new CefByteReadHandler(
                      reinterpret_cast<unsigned char*>(buffer.data()),
                      file_size, nullptr));
              
              // Determine MIME type
              std::string mime_type = request->mime_type_resolver().Run(url);
              if (mime_type.empty()) {
                // Default MIME types based on extension
                std::filesystem::path ext = fs_path.extension();
                std::string ext_str = ext.string();
                if (ext_str == ".html" || ext_str == ".htm") {
                  mime_type = "text/html";
                } else if (ext_str == ".js") {
                  mime_type = "application/javascript";
                } else if (ext_str == ".css") {
                  mime_type = "text/css";
                } else if (ext_str == ".json") {
                  mime_type = "application/json";
                } else if (ext_str == ".png") {
                  mime_type = "image/png";
                } else if (ext_str == ".jpg" || ext_str == ".jpeg") {
                  mime_type = "image/jpeg";
                } else if (ext_str == ".svg") {
                  mime_type = "image/svg+xml";
                } else {
                  mime_type = "application/octet-stream";
                }
              }
              
              handler = new CefStreamResourceHandler(mime_type, stream);
            }
          }
        }
      } catch (const std::filesystem::filesystem_error&) {
        // Invalid path or filesystem error, handler remains nullptr
        handler = nullptr;
      } catch (const std::exception&) {
        // Other exceptions, handler remains nullptr
        handler = nullptr;
      }
    }

    request->Continue(handler);
    return true;
  }

 private:
  std::string url_path_;
  std::string directory_path_;

  DISALLOW_COPY_AND_ASSIGN(DirectoryResourceProvider);
};

}  // namespace

// Implemented in resource_util_win_idmap.cc.
extern int GetResourceId(const char* resource_name);

bool LoadBinaryResource(const char* resource_name, std::string& resource_data) {
  int resource_id = GetResourceId(resource_name);
  if (resource_id == 0) {
    return false;
  }

  DWORD dwSize;
  LPBYTE pBytes;

  if (LoadBinaryResource(resource_id, dwSize, pBytes)) {
    resource_data = std::string(reinterpret_cast<char*>(pBytes), dwSize);
    return true;
  }

  NOTREACHED();  // The resource should be found.
  return false;
}

CefRefPtr<CefStreamReader> GetBinaryResourceReader(const char* resource_name) {
  int resource_id = GetResourceId(resource_name);
  if (resource_id == 0) {
    return nullptr;
  }

  DWORD dwSize;
  LPBYTE pBytes;

  if (LoadBinaryResource(resource_id, dwSize, pBytes)) {
    return CefStreamReader::CreateForHandler(
        new CefByteReadHandler(pBytes, dwSize, nullptr));
  }

  NOTREACHED();  // The resource should be found.
  return nullptr;
}

CefResourceManager::Provider* CreateBinaryResourceProvider(
    const std::string& url_path,
    const std::string& resource_path_prefix) {
  return new BinaryResourceProvider(url_path, resource_path_prefix);
}

CefResourceManager::Provider* CreateDirectoryResourceProvider(
    const std::string& url_path,
    const std::string& directory_path) {
  return new DirectoryResourceProvider(url_path, directory_path);
}

}  // namespace client
