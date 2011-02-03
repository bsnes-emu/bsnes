class Audio {
public:
  void coprocessor_enable(bool state);
  void coprocessor_frequency(double frequency);
  void sample(int16 left, int16 right);
  void coprocessor_sample(int16 left, int16 right);
  void init();

private:
  bool coprocessor;
  enum : unsigned { buffer_size = 32768, buffer_mask = buffer_size - 1 };
  uint32 dsp_buffer[buffer_size], cop_buffer[buffer_size];
  unsigned dsp_rdoffset, cop_rdoffset;
  unsigned dsp_wroffset, cop_wroffset;
  unsigned dsp_length, cop_length;

  double r_step, r_frac;
  int r_sum_l, r_sum_r;

  void flush();
};

extern Audio audio;
