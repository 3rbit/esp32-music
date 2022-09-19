/*  Example of constant power panning to test stereo output hack,
    using Mozzi sonification library.

    Tests stereo output.  This requires #define AUDIO_CHANNELS STEREO in mozzi_config.h

    Circuit: Audio outputs on digital pins 9 and 10.

    Mozzi documentation/API
    https://sensorium.github.io/Mozzi/doc/html/index.html

    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2017.
    This example code is in the public domain.
*/
#include <WiFi.h>
#include "aWOT.h"
#include "StaticFiles.h"

#include <MozziGuts.h>
#include <Oscil.h>               // oscil for audio sig
#include <tables/sin2048_int8.h> // sine table for pan oscillator

#define WIFI_SSID "PONG"
#define WIFI_PASSWORD "curlyhair"
#define SOUND_SPEED 0.034

WiFiServer server(80);
Application app;
TaskHandle_t Task1;

// use: Oscil <table_size, update_rate> oscilName (wavetable)
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aNoise(SIN2048_DATA);

Oscil<SIN2048_NUM_CELLS, CONTROL_RATE> kPan(SIN2048_DATA);

byte ampA, ampB; // convey amplitudes from updateControl() to updateAudioStereo();

const int trigPin = 5;
const int echoPin = 18;

long duration;
float distanceCm;

void Task1code(void *parameter)
{
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;)
  {
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
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  app.use(staticFiles());

  server.begin();

  kPan.setFreq(0.25f);
  aNoise.setFreq(220); // set the frequency with an unsigned int or a float
  startMozzi();        // :)

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1",   /* Name of the task */
      10000,     /* Stack size in words */
      NULL,      /* Task input parameter */
      0,         /* Priority of the task */
      &Task1,    /* Task handle. */
      0);        /* Core where the task should run */
}

void updateControl()
{
  WiFiClient client = server.available();

  if (client.connected())
    app.process(&client);

  // angle 0-90 deg (in rads) (https://dsp.stackexchange.com/questions/21691/algorithm-to-pan-audio/21736)
  float pan_angle = (float)(kPan.next() + 128) * (1.571f / 256.f);
  // cheap equal power panning from above site, put into 0-255 range for fast audio calcs
  ampA = (char)(sin(pan_angle) * 255);
  ampB = (char)(cos(pan_angle) * 255);
  Serial.print(ampA);
  Serial.print("  ");
  Serial.println(ampB);
}

AudioOutput_t updateAudio()
{
  int asig = aNoise.next();
  return StereoOutput::from16Bit(asig * ampA, asig * ampB);
}

void loop()
{
  audioHook(); // required here
}