//
// Created by Hubert on 22.06.2023.
//

#ifndef CARINFO_WEBSERVERHANDLER_H
#define CARINFO_WEBSERVERHANDLER_H

#include <WiFi.h>
#include <WebServer.h>
#include <string>
#include <bootlegUniquePtr.h>

class WebServerHandler
{
public:
    static std::unique_ptr<WebServerHandler> gen(const char* ssid, const char* password);
    WebServerHandler(const char* ssid, const char* password);
    ~WebServerHandler();

    bool isConnected();

    static void pushData(const std::string& data);

private:
    bool mIsConnected = false;

    static void handleRoot();
};
#endif //CARINFO_WEBSERVERHANDLER_H
