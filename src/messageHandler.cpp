/** On Message Handlers */
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include "..\include\server.h"
#include "..\include\global.h"

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
    int volumePct = json["data"];
    volume = (byte)map(volumePct, 0, 100, 0, 255);
  }

  void updateEnvelope(AsyncWebSocketClient *client, DynamicJsonDocument json)
  {
    envelope0.attackLevel = json["data"]["attackLevel"];
    envelope0.decayLevel = json["data"]["decayLevel"];
    envelope0.sustainLevel = json["data"]["sustainLevel"];
    envelope0.releaseLevel = json["data"]["releaseLevel"];
    envelope0.idleLevel = json["data"]["idleLevel"];
    envelope0.attackTime = json["data"]["attackTime"];
    envelope0.decayTime = json["data"]["decayTime"];
    envelope0.sustainTime = json["data"]["sustainTime"];
    envelope0.releaseTime = json["data"]["releaseTime"];
    envelope0.idleTime = json["data"]["idleTime"];
  }
}