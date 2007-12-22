#ifndef AUDIO_DIRECTSOUND_H
#define AUDIO_DIRECTSOUND_H

#include "../audio.h"

class pAudioDS;

class AudioDS : public Audio {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  void sample(uint16 left, uint16 right);
  void clear_audio();
  void init();
  void term();

  AudioDS();
  ~AudioDS();

private:
  pAudioDS &p;
};

#endif //ifndef AUDIO_DIRECTSOUND_H
