#include <Arduino.h>
#include <ArduinoJson.h>

#include "sensor.h"
#include "myServer.h"

SensorData sensorData;

float readUltrasonic()
{
  long duration;

  // Clears the trigPin
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHOPIN, HIGH);

  // Calculate the distance
  return duration * SOUND_SPEED / 2;
}

// Read sensors into sensorData struct
void readSensors()
{
  sensorData.distance = readUltrasonic();

  // reads the input on analog pin A0 (value between 0 and 1023)
  sensorData.light = analogRead(LDRPIN);
}

void sensorSetup()
{
  pinMode(PROXPOWERPIN, OUTPUT);
  pinMode(TRIGPIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHOPIN, INPUT);  // Sets the echoPin as an Input
  pinMode(LDRPIN, INPUT);

  digitalWrite(PROXPOWERPIN, HIGH); // set as power source
}

void printSensorData()
{
  printf("Distance: %f\n", sensorData.distance);
  printf("Light: %d\n\n", sensorData.light);
}

void sensorLoop()
{
  readSensors();
  printSensorData();
  if (ws.getClients().length() > 0)
  {
    StaticJsonDocument<JSONBUFFERSIZE> namedJson = sensorData.toNamedJSON();
    char buffer[JSONBUFFERSIZE];
    size_t len = serializeJson(namedJson, buffer);
    ws.textAll(buffer, len);
  }
}