
#ifndef SERVER_H
#define SERVER_H

#include <ESPAsyncWebServer.h>

void webServerSetup();
void webServerLoop();

extern AsyncWebServer server;
extern AsyncWebSocket ws;

#endif