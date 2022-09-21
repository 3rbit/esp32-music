#ifndef SENSORS_H
#define SENSORS_H

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

#endif