// Copyright (c) 2024 replace_me Authors. All rights reserved.

#ifndef REPLACE_ME_SERVICES_SETTING_SETTING_SERVICE_H_
#define REPLACE_ME_SERVICES_SETTING_SETTING_SERVICE_H_
#pragma once

#include <string>
#include <vector>
#include "../MessageTypes.h"
#include "../IService.h"

namespace replace_me::services
{
    struct GeneralSettings
    {
        std::string language;  // Language option
        std::string theme;     // Theme option
        XPACK(O(language, theme));
    };

    struct UserSettings
    {
        std::string userName;   // User name
        std::string userEmail;  // User email
        std::string sshKey;     // SSH key path
        std::string sshConfig;  // Other SSH configuration
        XPACK(O(userName, userEmail, sshKey, sshConfig));
    };

    struct NetworkSettings
    {
        std::string proxy;      // Proxy configuration
        XPACK(O(proxy));
    };

    struct AboutSettings
    {
        bool autoUpdate;        // Auto update enabled
        bool logEnabled;        // Log enabled
        std::string logPath;    // Log file path
        XPACK(O(autoUpdate, logEnabled, logPath));
    };

    struct Setting
    {
        GeneralSettings general;
        UserSettings user;
        NetworkSettings network;
        AboutSettings about;
        XPACK(O(general, user, network, about));
    };

    class SettingService : public IService
    {
    public:
        static SettingService& getInstance();

        int onQuery(const std::string& action, const std::string& request, std::string& response, std::string& message) override;

    private:
        SettingService();
        ~SettingService();

        // Get the path to the executable directory
        std::string GetExecutableDirectory();

        // Get the path to the config file
        std::string GetConfigFilePath();

        // Load config from file
        bool LoadConfig(Setting& config);

        // Save config to file
        bool SaveConfig(const Setting& config);
    };

}  // namespace replace_me::services

#endif  // REPLACE_ME_SERVICES_SETTING_SETTING_SERVICE_H_


