/** On Message Handlers */
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include "myServer.h"
#include "global.h"

namespace MessageHandler
{
  // Message handler for ping message
  void ping(AsyncWebSocketClient *client)
  {
    StaticJsonDocument<24> send;
    char buffer[24];
    send["target"] = "pong";
    send["data"] = "";
    size_t size = serializeJson(send, buffer);
    client->text(buffer, size);
    return;
  }

  void updateVolume(AsyncWebSocketClient *client, DynamicJsonDocument json)
  {
    globalVolume = (byte)json["data"];
  }

  void updateTempo(DynamicJsonDocument json)
  {
    unsigned int tempo = (unsigned int)json["data"];
    beat1.updateTempo(tempo);
    beat2.updateTempo(tempo);
  }
}