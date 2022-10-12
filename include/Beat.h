#ifndef BEAT_H
#define BEAT_H

#include <Oscil.h> // oscil for audio sig
#include <ADSR.h>
#include <EventDelay.h>
#include <tables/sin1024_int8.h> // sine table for pan oscillator

class Beat
{
private:
  Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> noise;
  ADSR<AUDIO_RATE, AUDIO_RATE, unsigned long> envelope;
  EventDelay delay;
  unsigned char onBeats[4];
  unsigned char beatIndex = 0;

public:
  Beat(int frequency, unsigned char noteOnBeats[4]);
  void updateControl();
  AudioOutput_t next();
};

#endif