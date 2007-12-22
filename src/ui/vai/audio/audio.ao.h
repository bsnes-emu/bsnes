#ifndef AUDIO_AO_H
#define AUDIO_AO_H

#include "../audio.h"

class pAudioAO;

class AudioAO : public Audio {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  void sample(uint16 left, uint16 right);
  void init();
  void term();

  AudioAO();
  ~AudioAO();

private:
  pAudioAO &p;
};

#endif //ifndef AUDIO_AO_H
