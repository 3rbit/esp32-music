#ifndef GLOBAL_H
#define GLOBAL_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define WIFI_SSID "PONG"
#define WIFI_PASSWORD "curlyhair"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

#endif