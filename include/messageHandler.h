#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <ESPAsyncWebServer.h>

namespace MessageHandler
{
  void ping(AsyncWebSocketClient *client);
  void updateVolume(AsyncWebSocketClient *client, DynamicJsonDocument json);
  void updateTempo(DynamicJsonDocument json);
}

#endif