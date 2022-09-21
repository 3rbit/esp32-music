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
Oscil<SIN2048_NUM_CELLS, CONTROL_RATE> kPan(SIN2048_DATA);
ADSR<CONTROL_RATE, AUDIO_RATE, unsigned long> envelope;

EventDelay noteDelay;
byte ampA, ampB; // convey amplitudes from updateControl() to updateAudioStereo();
// unsigned int duration, attack, decay, sustain, release_ms;
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
  // envelope.setLevels(envelope0.attackLevel, envelope0.decayLevel, envelope0.sustainLevel, envelope0.releaseLevel);
  // envelope.setTimes(envelope0.attackTime, envelope0.decayTime, envelope0.sustainTime, envelope0.releaseTime);
  kPan.setFreq(0.25f);
  aNoise.setFreq(220); // set the frequency with an unsigned int or a float
}

void updateControl()
{
  // angle 0-90 deg (in rads) (https://dsp.stackexchange.com/questions/21691/algorithm-to-pan-audio/21736)
  float pan_angle = (float)(kPan.next() + 128) * (1.571f / 256.f);
  // cheap equal power panning from above site, put into 0-255 range for fast audio calcs
  ampA = (char)(sin(pan_angle) * 255);
  ampB = (char)(cos(pan_angle) * 255);

  envelope.setLevels(envelope0.attackLevel, envelope0.decayLevel, envelope0.sustainLevel, envelope0.releaseLevel);
  envelope.setTimes(envelope0.attackTime, envelope0.decayTime, envelope0.sustainTime, envelope0.releaseTime);

  // Serial.println(sensorData.temperature);
  // Serial.println(sensorData.ldr);
  // int noteIndex = (int)map(sensorData.ldr, 0, 2047, 0, sizeof(notes) / sizeof(float));
  // aNoise.setFreq(notes[noteIndex]);
  if (noteDelay.ready())
  {
    aNoise.setFreq(notes[rand(sizeof(notes) / sizeof(float))]);
    envelope.noteOn();
    noteDelay.start();
  }
  envelope.update();
}

AudioOutput_t updateAudio()
{
  u_int8_t env = envelope.next();
  int asig = aNoise.next();
  return StereoOutput::fromNBit(32, volume * env * asig * ampA, volume * env * asig * ampB);
}

void loop()
{
  audioHook(); // required here
}