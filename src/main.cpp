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
#include "SignalChain.h"

TaskHandle_t Task;

// use: Oscil <table_size, update_rate> oscilName (wavetable)
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> aNoise(SIN1024_DATA);
Envelope envelope0;
SignalChain chain(aNoise, envelope0, 2000);
byte volume = 255;

void setup()
{
  Serial.begin(115200);

  // Run websocket server and sensors on other core
  xTaskCreatePinnedToCore(
      taskCore,      /* Function to implement the task */
      "WEB+SENSORS", /* Name of the task */
      10000,         /* Stack size in words */
      NULL,          /* Task input parameter */
      0,             /* Priority of the task */
      &Task,         /* Task handle. */
      0);            /* Core where the task should run */

  // delay(200);
  // Serial.print("setup() running on core ");
  // Serial.println(xPortGetCoreID());

  randSeed(); // fresh random
  startMozzi();
}

void updateControl()
{
  chain.updateControl();
}

AudioOutput_t updateAudio()
{
  AudioOutput_t chainOutput = chain.updateAudio();
  return AudioOutput::from16Bit(chainOutput * volume);
}

void loop()
{
  audioHook(); // required here
}