class Audio {
public:
uint frequency, latency;
  virtual void tick() {}
  virtual void run(uint32 sample) {}
  virtual void set_frequency(uint freq);
  virtual void clear_audio() {}
  virtual void init() {}
  virtual void term() {}

  void resample_point(uint32 *output, uint32 *input, uint output_samples, uint input_samples);

  Audio();
} *uiAudio;
