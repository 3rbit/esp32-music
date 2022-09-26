#ifndef SIGNAL_CHAIN_H
#define SIGNAL_CHAIN_H

#include <Oscil.h>
#include "..\include\global.h"

class SignalChain
{
public:
  SignalChain(Oscil<1024, AUDIO_RATE> o, Envelope env, unsigned int delay_ms);
  void updateControl();
  AudioOutput_t updateAudio();
};

#endif