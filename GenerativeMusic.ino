#include <MozziGuts.h>
#include <Oscil.h> // oscil for audio sig
#include <EventDelay.h>
#include <ADSR.h>
#include <tables/sin2048_int8.h> // sine table for pan oscillator
#include <mozzi_rand.h>

#include "include\task.h"
#include "include\sensor.h"
#include "include\global.h"

TaskHandle_t Task;

// use: Oscil <table_size, update_rate> oscilName (wavetable)
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aNoise(SIN2048_DATA);
ADSR<AUDIO_RATE, AUDIO_RATE, unsigned long> envelope;

EventDelay noteDelay;
byte volume = 255;
Envelope envelope0;

float notes[] = {261.63, 293.66, 329.63, 392.00, 440.00, 523.25};

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

  delay(200);
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());

  randSeed(); // fresh random
  startMozzi();
  noteDelay.set(2000); // 2 second countdown
  aNoise.setFreq(220); // set the frequency with an unsigned int or a float
}

void updateControl()
{
  envelope.setLevels(envelope0.attackLevel, envelope0.decayLevel, envelope0.sustainLevel, envelope0.releaseLevel);
  envelope.setTimes(envelope0.attackTime, envelope0.decayTime, envelope0.sustainTime, envelope0.releaseTime);

  if (noteDelay.ready())
  {
    aNoise.setFreq(notes[rand(sizeof(notes) / sizeof(float))]);
    envelope.noteOn();
    noteDelay.start();
  }
}

AudioOutput_t updateAudio()
{
  envelope.update();
  u_int8_t env = envelope.next();
  int asig = aNoise.next();
  return MonoOutput::fromNBit(24, volume * env * asig);
}

void loop()
{
  audioHook(); // required here
}