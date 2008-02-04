/*
  audio.oss (2007-12-26)
  author: Nach
*/

class pAudioOSS;

class AudioOSS : public Audio {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  void sample(uint16_t sl, uint16_t sr);
  bool init();
  void term();

  AudioOSS();
  ~AudioOSS();

private:
  pAudioOSS &p;
};
