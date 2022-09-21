#include <Arduino.h>
#include "..\include\sensor.h"
#include "..\include\server.h"

/** task to be run on 2nd core of esp32 */
void taskCore(void *parameter)
{
  delay(200);
  Serial.print("taskCore() running on core ");
  Serial.println(xPortGetCoreID());
  webServerSetup();
  sensorSetup();
  for (;;)
  {
    webServerLoop();
    sensorLoop();
  }
}