#ifndef SENSORS_H
#define SENSORS_H

#include <ArduinoJson.h>

// Sensor Config
#define SOUND_SPEED 0.034

#define TRIGPIN 18
#define ECHOPIN 5
#define PROXPOWERPIN 19
#define LDRPIN 35

#define JSONBUFFERSIZE 256

void sensorSetup();
void sensorLoop();

struct SensorData
{
  float distance; // ultrasonic
  int light;

  StaticJsonDocument<JSONBUFFERSIZE> toNamedJSON()
  {
    StaticJsonDocument<JSONBUFFERSIZE> json;
    json["target"] = "sensors";
    JsonObject jsonData = json.createNestedObject("data");
    jsonData["distance"] = distance;
    jsonData["light"] = light;
    return json;
  }
};

extern SensorData sensorData;

#endif