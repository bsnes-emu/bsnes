/*
  audio.alsa (2008-08-12)
  authors: Nach, RedDwarf
*/

class pAudioALSA;

class AudioALSA : public Audio {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  void sample(uint16_t left, uint16_t right);
  bool init();
  void term();

  AudioALSA();
  ~AudioALSA();

private:
  pAudioALSA &p;
};
