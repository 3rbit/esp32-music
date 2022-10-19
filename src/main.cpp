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
byte globalVolume = 100;
unsigned int tempo = 120;


Beat beat1(300, (unsigned char[BEATS_PER_BAR]){1, 0, 0, 1, 1, 0, 0, 1});
Beat beat2(600, (unsigned char[BEATS_PER_BAR]){0, 1, 1, 0, 0, 0, 0, 0});

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