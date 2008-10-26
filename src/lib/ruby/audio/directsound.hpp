/*
  audio.directsound (2007-12-26)
  author: byuu
*/

class pAudioDS;

class AudioDS : public Audio {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  void sample(uint16_t left, uint16_t right);
  void clear();
  bool init();
  void term();

  AudioDS();
  ~AudioDS();

private:
  pAudioDS &p;
};
