/** Web server code */

#include <Arduino.h>
#include "..\include\aWOT.h"
#include "..\include\StaticFiles.h"
#include "..\include\server.h"
#include "..\include\global.h"

Application app;
bool ledState = false;

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    if (strcmp((char *)data, "toggle") == 0)
    {
      ledState = !ledState;
      ws.textAll(String(ledState));
    }
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
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
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

  // init websocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  server.begin();
}

void webServerLoop()
{
  WiFiClient client = server.available();

  if (client.connected())
    app.process(&client);

  ws.cleanupClients();
}