#include <Arduino.h>
#include <DHT.h>

#include "..\include\sensor.h"

#define SOUND_SPEED 0.034

const int trigPin = 5;
const int echoPin = 18;
const int pirPin = 14;
const int dhtPin = 13;
const int ldrPin = 35;

DHT dht(dhtPin, DHT11);

void readUltrasonic()
{
  long duration;
  float distanceCm;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
}

void readPIR()
{
  int pirStat = 0;
  pirStat = digitalRead(pirPin);

  Serial.print("PIR: ");
  Serial.println(pirStat);
}

void readDHT()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}

void readLDR() {
  // reads the input on analog pin A0 (value between 0 and 1023)
  int analogValue = analogRead(ldrPin);

  Serial.print("Analog reading: ");
  Serial.print(analogValue);   // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 10) {
    Serial.println(" - Dark");
  } else if (analogValue < 200) {
    Serial.println(" - Dim");
  } else if (analogValue < 500) {
    Serial.println(" - Light");
  } else if (analogValue < 800) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }

}

namespace Sensors
{
  void setup()
  {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
    pinMode(pirPin, INPUT);
    pinMode(dhtPin, INPUT);
    pinMode(ldrPin, INPUT);

    dht.begin();

    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());
  }

  void loop()
  {
    readUltrasonic();
    readPIR();
    readDHT();
    readLDR();
  }
}

void sensorsTask(void *parameter)
{
  Sensors::setup();
  for (;;)
  {
    Sensors::loop();
  }
}