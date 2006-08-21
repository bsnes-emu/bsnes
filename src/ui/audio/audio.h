class Audio {
public:
uint frequency;
  virtual void run(uint32 sample) {}
  virtual void set_frequency(uint new_freq) {}
  virtual void clear_audio() {}
  virtual void init() {}
  virtual void term() {}

  Audio() : frequency(32000) {}
} *uiAudio;
