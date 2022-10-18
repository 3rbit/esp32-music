#include <Arduino.h>
#include <DHT.h>
#include <ArduinoJson.h>

#include "sensor.h"
#include "myServer.h"

SensorData sensorData;

DHT dht(DHTPIN, DHT11);

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
  sensorData.distanceCm = readUltrasonic();

  // reads the input on analog pin A0 (value between 0 and 1023)
  sensorData.ldr = analogRead(LDRPIN);

  sensorData.pirStat = digitalRead(PIRPIN);

  sensorData.humidity = dht.readHumidity();
  sensorData.temperature = dht.readTemperature();
  sensorData.heatIndex = dht.computeHeatIndex(sensorData.temperature, sensorData.humidity, false);
}

void sensorSetup()
{
  pinMode(TRIGPIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHOPIN, INPUT);  // Sets the echoPin as an Input
  pinMode(PIRPIN, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(LDRPIN, INPUT);

  dht.begin();
}

StaticJsonDocument<JSONBUFFERSIZE> sensorDataToJSON(SensorData sensorData)
{
  StaticJsonDocument<JSONBUFFERSIZE> json;
  json["distance"] = sensorData.distanceCm;
  json["ldr"] = sensorData.ldr;
  json["pir"] = sensorData.pirStat;
  json["humidity"] = sensorData.humidity;
  json["temperature"] = sensorData.temperature;
  json["heatIndex"] = sensorData.heatIndex;
  return json;
}

void sensorLoop()
{
  readSensors();

  if (ws.getClients().length() > 0)
  {
    StaticJsonDocument<JSONBUFFERSIZE> json = sensorDataToJSON(sensorData);
    StaticJsonDocument<JSONBUFFERSIZE> namedJson;
    char buffer[JSONBUFFERSIZE];
    namedJson["target"] = "sensors";
    namedJson["data"] = json;

    size_t len = serializeJson(namedJson, buffer);

    ws.textAll(buffer, len);
  }
}