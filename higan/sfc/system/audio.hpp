struct Audio {
  auto coprocessor_enable(bool state) -> void;
  auto coprocessor_frequency(double frequency) -> void;
  auto sample(int16 lsample, int16 rsample) -> void;
  auto coprocessor_sample(int16 lsample, int16 rsample) -> void;

private:
  auto flush() -> void;

  nall::DSP dspaudio;
  bool coprocessor;
  enum : uint { buffer_size = 256, buffer_mask = buffer_size - 1 };
  uint32 dsp_buffer[buffer_size], cop_buffer[buffer_size];
  uint dsp_rdoffset, cop_rdoffset;
  uint dsp_wroffset, cop_wroffset;
  uint dsp_length, cop_length;
};

extern Audio audio;
