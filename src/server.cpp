/** Web server code */
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "myServer.h"
#include "wifiConfig.h"
#include "messageHandler.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    DynamicJsonDocument json(1024);
    deserializeJson(json, data, len);
    const char *target = json["target"].as<const char *>();

    if (strcmp(target, "ping") == 0)
      MessageHandler::ping(client);
    else if (strcmp(target, "updateVolume") == 0)
      MessageHandler::updateVolume(client, json);
    else if (strcmp(target, "updateEnvelope") == 0) 
      MessageHandler::updateEnvelope(client, json);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    data[len] = 0;
    Serial.printf("Message: %s\n", data);
    handleWebSocketMessage(client, arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void webServerSetup()
{
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Serve files in directory "/www/" when request url starts with "/"
  // Request to the root or none existing files will try to server the defualt
  // file name "index.htm" if exists
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html"); });

  // Start server
  server.begin();
}

void webServerLoop()
{
  ws.cleanupClients();
}