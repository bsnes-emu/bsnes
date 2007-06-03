class Audio {
public:
uint frequency, latency;
  virtual void sample(uint16 l_sample, uint16 r_sample) {}
  virtual void update_frequency();
  virtual void clear_audio() {}
  virtual void init() {}
  virtual void term() {}

/*
  uint bind_range(uint min, uint max, uint index);
  void resample_point  (uint32 *output, uint32 *input, uint output_samples, uint input_samples);
  void resample_linear (uint32 *output, uint32 *input, uint output_samples, uint input_samples);
  void resample_cosine (uint32 *output, uint32 *input, uint output_samples, uint input_samples);
  void resample_cubic  (uint32 *output, uint32 *input, uint output_samples, uint input_samples);
  void resample_hermite(uint32 *output, uint32 *input, uint output_samples, uint input_samples);
*/

  Audio();
} *uiAudio;
