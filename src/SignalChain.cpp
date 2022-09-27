#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <mozzi_rand.h>

#include "global.h"

class SignalChain
{
private:
  Oscil<1024, AUDIO_RATE> oscil;
  Envelope envSettings;
  EventDelay noteDelay;
  ADSR<AUDIO_RATE, AUDIO_RATE> envelope;
  float notes[6] = {mtof(48.0f),
                    mtof(50.0f),
                    mtof(52.0f),
                    mtof(55.0f),
                    mtof(57.0f),
                    mtof(60.0f)};
  u_int8_t notesLength = sizeof(notes) / sizeof(float);

public:
  SignalChain(Oscil<1024, AUDIO_RATE> o, Envelope env, unsigned int delay_ms);
  void updateControl();
  AudioOutput_t updateAudio();
};

SignalChain::SignalChain(Oscil<1024, AUDIO_RATE> o, Envelope env, unsigned int delay_ms)
{
  // notes = {mtof(48.0f),
  //          mtof(50.0f),
  //          mtof(52.0f),
  //          mtof(55.0f),
  //          mtof(57.0f),
  //          mtof(60.0f)};
  // notesLength = sizeof(notes) / sizeof(float);

  o.setFreq(notes[0]);
  oscil = o;
  envSettings = env;
  noteDelay.set(delay_ms);
}

void SignalChain::updateControl()
{
  envelope.setLevels(envelope0.attackLevel, envelope0.decayLevel, envelope0.sustainLevel, envelope0.releaseLevel);
  envelope.setTimes(envelope0.attackTime, envelope0.decayTime, envelope0.sustainTime, envelope0.releaseTime);

  if (noteDelay.ready())
  {
    oscil.setFreq(notes[rand(sizeof(notes) / sizeof(float))]);
    envelope.noteOn();
    noteDelay.start();
  }
}

AudioOutput_t SignalChain::updateAudio()
{
  envelope.update();
  return AudioOutput::from16Bit(envelope.next() * oscil.next());
}