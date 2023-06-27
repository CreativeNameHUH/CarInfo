//
// Created by Hubert on 22.06.2023.
//

#include "webServerHandler.h"

std::string webpage;
WebServer* server = nullptr;

std::unique_ptr<WebServerHandler> WebServerHandler::gen(const char* ssid, const char* password)
{
    return make_unique<WebServerHandler>(ssid, password);
}

WebServerHandler::WebServerHandler(const char* ssid, const char* password)
{
    WiFi.begin(ssid, password);
    for (int timeout = 0; WiFiClass::status() != WL_CONNECTED; timeout++)
    {
        if (timeout == 50)
            return;

        delay(500);
    }

    mIsConnected = true;

    if (!server)
        server = new WebServer(80);

    server->on("/", handleRoot);
    server->begin();
}

WebServerHandler::~WebServerHandler()
{
    server->stop();
    delete server;
    server = nullptr;

    webpage = "";

    WiFi.disconnect();
}

bool WebServerHandler::isConnected()
{
    return mIsConnected;
}

void WebServerHandler::pushData(const std::string& data)
{
    server->handleClient();

    webpage = "<!DOCTYPE html><html><meta http-equiv=\"refresh\" content=\"1\">"
              "<body>";
    webpage += data + "</body></html>";
}

void WebServerHandler::handleRoot()
{
    if (server)
        server->send(200, "text/html", webpage.c_str());
}
