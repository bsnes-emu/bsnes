#ifndef AUDIO_H
#define AUDIO_H

class Audio {
public:
uint frequency, latency;
  virtual void sample(uint16 l_sample, uint16 r_sample) {}
  virtual void update_frequency();
  virtual void clear_audio() {}
  virtual void init() {}
  virtual void term() {}

  Audio();
} *uiAudio;

#endif
