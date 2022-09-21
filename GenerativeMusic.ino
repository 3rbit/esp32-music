#include <MozziGuts.h>
#include <Oscil.h>               // oscil for audio sig
#include <tables/sin2048_int8.h> // sine table for pan oscillator

#include "include\task.h"

TaskHandle_t Task;

// use: Oscil <table_size, update_rate> oscilName (wavetable)
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aNoise(SIN2048_DATA);

Oscil<SIN2048_NUM_CELLS, CONTROL_RATE> kPan(SIN2048_DATA);

byte ampA, ampB; // convey amplitudes from updateControl() to updateAudioStereo();

void setup()
{
  Serial.begin(115200);

// 
  xTaskCreatePinnedToCore(
      taskCore, /* Function to implement the task */
      "WEB+SENSORS",   /* Name of the task */
      10000,       /* Stack size in words */
      NULL,        /* Task input parameter */
      0,           /* Priority of the task */
      &Task,      /* Task handle. */
      0);          /* Core where the task should run */

  startMozzi();
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
  // Serial.print(ampA);
  // Serial.print("  ");
  // Serial.println(ampB);
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