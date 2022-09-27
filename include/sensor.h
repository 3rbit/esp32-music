#ifndef SENSORS_H
#define SENSORS_H

// Sensor Config
#define SOUND_SPEED 0.034

#define TRIGPIN 5
#define ECHOPIN 18
#define PIRPIN 14
#define DHTPIN 13
#define LDRPIN 35

#define JSONBUFFERSIZE 256


void sensorSetup();
void sensorLoop();

struct SensorData
{
  float distanceCm; // ultrasonic
  int ldr;
  int pirStat;
  float humidity;
  float temperature;
  float heatIndex;
};

extern SensorData sensorData;

#endif