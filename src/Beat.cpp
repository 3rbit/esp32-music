#include <Oscil.h> // oscil for audio sig
#include <ADSR.h>
#include <EventDelay.h>
#include <tables/sin1024_int8.h> // sine table for pan oscillator
#include "Beat.h"

Beat::Beat(int frequency, unsigned char noteOnBeats[4])
{
  for (int i = 0; i < 4; i++)
  {
    onBeats[i] = noteOnBeats[i];
  }

  noise = Oscil<SIN1024_NUM_CELLS, AUDIO_RATE>(SIN1024_DATA);
  noise.setFreq(frequency);

  envelope.setLevels(255, 0, 0, 0);
  envelope.setTimes(100, 200, 10000, 100000);

  delay.set(500);
}

void Beat::updateControl()
{
  if (delay.ready())
  {
    if (onBeats[beatIndex++])
      envelope.noteOn();

    if (beatIndex >= 4)
      beatIndex = 0;

    delay.start();
  }
}

unsigned char Beat::next()
{
  envelope.update();
  return envelope.next() * noise.next() >> 8;
}