#include <Arduino.h>
#include <MozziGuts.h>
#include <Oscil.h>               // oscil for audio sig
#include <tables/sin1024_int8.h> // sine table for pan oscillator

#include "task.h"
#include "sensor.h"
#include "global.h"
#include "Beat.h"

TaskHandle_t Task;
byte globalVolume = 255;
unsigned int tempo = 120;

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> oscilLight(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> oscilDistance(SIN1024_DATA);
Beat beat1(300, (bool[BEATS_PER_BAR]){1, 0, 0, 1, 1, 0, 0, 1});
Beat beat2(600, (bool[BEATS_PER_BAR]){0, 1, 1, 0, 0, 0, 0, 0});

void setup()
{
  Serial.begin(115200);

  // Run websocket server and sensors on other core
  xTaskCreatePinnedToCore(taskCore, "WEB+SENSORS", 10000, NULL, 0, &Task, 0);

  startMozzi();

  oscilLight.setFreq(500);
  oscilDistance.setFreq(500);
}

void updateControl()
{
  readSensors();

  beat1.updateControl();
  beat2.updateControl();

  oscilLight.setFreq(sensorData.light);
  oscilDistance.setFreq(sensorData.distance * 1000);
}

AudioOutput_t updateAudio()
{
  // return AudioOutput::from8Bit(oscilLight.next() + oscilDistance.next() >> 1);
  // return AudioOutput::from8Bit(beat1.next());
  // return AudioOutput::from8Bit(beat1.next() + beat2.next() >> 1);
  return AudioOutput::from16Bit(globalVolume * (beat1.next() + beat2.next() + oscilLight.next() + oscilDistance.next()) >> 2);
}

void loop()
{
  audioHook(); // required here
}