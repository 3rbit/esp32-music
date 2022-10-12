#ifndef GLOBAL_H
#define GLOBAL_H

extern byte volume;
extern unsigned int tempo;

struct Envelope
{
  byte attackLevel = 255;
  byte decayLevel = 255;
  byte sustainLevel = 255;
  byte releaseLevel = 255;
  byte idleLevel = 255;
  unsigned int attackTime = 500;
  unsigned int decayTime = 500;
  unsigned int sustainTime = 500;
  unsigned int releaseTime = 500;
  unsigned int idleTime = 500;
};

extern Envelope envelope0;

#endif