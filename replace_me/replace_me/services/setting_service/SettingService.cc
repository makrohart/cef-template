// Copyright (c) 2024 replace_me Authors. All rights reserved.

#include "replace_me/services/setting_service/SettingService.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

namespace replace_me::services
{
    namespace fs = std::filesystem;

    SettingService& SettingService::getInstance()
    {
        static SettingService s_instance;
        return s_instance;
    }

    SettingService::SettingService()
    {
    }

    SettingService::~SettingService()
    {
    }

    std::string SettingService::GetExecutableDirectory()
    {
#ifdef _WIN32
        char szPath[MAX_PATH];
        if (GetModuleFileNameA(nullptr, szPath, MAX_PATH) != 0)
        {
            fs::path exePath(szPath);
            return exePath.parent_path().string();
        }
#else
        char szPath[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", szPath, PATH_MAX);
        if (count != -1)
        {
            szPath[count] = '\0';
            fs::path exePath(szPath);
            return exePath.parent_path().string();
        }
#endif
        // Fallback to current directory
        return fs::current_path().string();
    }

    std::string SettingService::GetConfigFilePath()
    {
        std::string exeDir = GetExecutableDirectory();
        fs::path configPath(exeDir);
        configPath /= "replace_me.json";
        return configPath.string();
    }

    bool SettingService::LoadConfig(Setting& config)
    {
        std::string configPath = GetConfigFilePath();
        
        if (!fs::exists(configPath))
        {
            // Return empty config if file doesn't exist
            config.general.language = "en";
            config.general.theme = "light";
            config.user.userName = "";
            config.user.userEmail = "";
            config.user.sshKey = "";
            config.user.sshConfig = "";
            config.network.proxy = "";
            config.about.autoUpdate = true;
            config.about.logEnabled = true;
            config.about.logPath = "";
            return true;
        }

        try
        {
            std::ifstream file(configPath);
            if (!file.is_open())
            {
                return false;
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();

            if (content.empty())
            {
                config.general.language = "en";
                config.general.theme = "light";
                config.user.userName = "";
                config.user.userEmail = "";
                config.user.sshKey = "";
                config.user.sshConfig = "";
                config.network.proxy = "";
                config.about.autoUpdate = true;
                config.about.logEnabled = true;
                config.about.logPath = "";
                return true;
            }

            xpack::json::decode(content, config);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    bool SettingService::SaveConfig(const Setting& config)
    {
        std::string configPath = GetConfigFilePath();
        
        try
        {
            // Create directory if it doesn't exist
            fs::path path(configPath);
            fs::path dir = path.parent_path();
            if (!dir.empty() && !fs::exists(dir))
            {
                fs::create_directories(dir);
            }

            std::string content = xpack::json::encode(config);
            
            std::ofstream file(configPath);
            if (!file.is_open())
            {
                return false;
            }

            file << content;
            file.close();
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    int SettingService::onQuery(const std::string& action, const std::string& request, std::string& response, std::string& message)
    {
        try
        {
            if (action == "setting:get")
            {
                Setting config;
                if (!LoadConfig(config))
                {
                    message = "Failed to load config file";
                    return -1;
                }

                response = xpack::json::encode(config);
                return 0;
            }
            else if (action == "setting:set")
            {
                Setting config;
                xpack::json::decode(request, config);

                if (!SaveConfig(config))
                {
                    message = "Failed to save config file";
                    return -1;
                }
                return 0;
            }
            else
            {
                message = "Unknown action: " + action;
                return -1;
            }
        }
        catch (const std::exception& e)
        {
            message = "Exception: " + std::string(e.what());
            return -1;
        }
    }

}  // namespace replace_me::services


