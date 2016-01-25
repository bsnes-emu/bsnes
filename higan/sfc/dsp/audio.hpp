struct Audio {
  auto sample(int16 left, int16 right) -> void;

  auto coprocessorEnable(bool enable) -> void;
  auto coprocessorFrequency(double frequency) -> void;
  auto coprocessorSample(int16 left, int16 right) -> void;
  auto flush() -> void;

private:
  nall::DSP mixer;
  bool mixerEnable = false;
  struct Buffer {
    int16 left[256];
    int16 right[256];
    uint8 read;
    uint8 write;
  } dsp, mix;
};

extern Audio audio;
