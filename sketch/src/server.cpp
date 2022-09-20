/** Web server code */

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketServer.h>
#include <aWOT.h>
#include "..\include\StaticFiles.h"
#include "..\include\server.h"
#include "..\include\global.h"
using namespace net;

WiFiServer server(80);
WebSocketServer socketServer{3000};

Application app;

void webSocketSetup()
{
  socketServer.onConnection([](WebSocket &ws)
                            {
    const char message[]{ "Hello from Arduino server!" };
    ws.send(WebSocket::DataType::TEXT, message, strlen(message));

    ws.onClose([](WebSocket &ws, const WebSocket::CloseCode code,
                 const char *reason, uint16_t length) {
      // ...
    });
    ws.onMessage([](WebSocket &ws, const WebSocket::DataType dataType,
                   const char *message, uint16_t length) {
      Serial.println(message);

      if (strcmp(message, "ping")) {
        ws.send(WebSocket::DataType::TEXT, "pong", 4);
      }
    }); });
}

void webServerSetup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  app.use(staticFiles()); // serve static files

  webSocketSetup();

  socketServer.begin();
  server.begin();
}

void webServerLoop()
{
  socketServer.listen();

  WiFiClient client = server.available();

  if (client.connected())
    app.process(&client);
}