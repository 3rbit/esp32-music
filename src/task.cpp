#include <Arduino.h>
#include "..\include\sensor.h"
#include "..\include\server.h"

/** task to be run on 2nd core of esp32 */
void taskCore(void *parameter)
{
  webServerSetup();
  sensorSetup();
  for (;;)
  {
    webServerLoop();
    sensorLoop();
  }
}