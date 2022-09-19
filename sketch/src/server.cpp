/** Web server code */

#include <Arduino.h>
#include <aWOT.h>
#include "..\include\StaticFiles.h"
#include "..\include\server.h"
#include "..\include\global.h"

Application app;

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

  server.begin();
}

void webServerLoop()
{
  WiFiClient client = server.available();

  if (client.connected())
    app.process(&client);

}