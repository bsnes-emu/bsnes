/*
  audio.openal (2007-12-26)
  author: Nach
  contributors: byuu, wertigon, _willow_
*/

class pAudioOpenAL;

class AudioOpenAL : public Audio {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  void sample(uint16_t sl, uint16_t sr);
  bool init();
  void term();

  AudioOpenAL();
  ~AudioOpenAL();

private:
  pAudioOpenAL &p;
};
