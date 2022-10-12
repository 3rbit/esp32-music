#include <Arduino.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscil for audio sig
#include <EventDelay.h>
#include <ADSR.h>
#include <tables/sin1024_int8.h> // sine table for pan oscillator
#include <mozzi_rand.h>

#include "task.h"
#include "sensor.h"
#include "global.h"
#include "Beat.h"

TaskHandle_t Task;
Envelope envelope0;
byte volume;
unsigned int tempo;


Beat beat1(300, (unsigned char[4]){1, 0, 0, 1});
Beat beat2(600, (unsigned char[4]){0, 1, 1, 0});

float notes[] = {261.63, 293.66, 329.63, 392.00, 440.00, 523.25};

void setup()
{
  Serial.begin(115200);

  // Run websocket server and sensors on other core
  xTaskCreatePinnedToCore(taskCore, "WEB+SENSORS", 10000, NULL, 0, &Task, 0);

  startMozzi();
}

void updateControl()
{
  beat1.updateControl();
  beat2.updateControl();
}

AudioOutput_t updateAudio()
{
  return AudioOutput::from8Bit(beat1.next() + beat2.next());
}

void loop()
{
  audioHook(); // required here
}