struct Palette {
  static const uint8_t gammaRamp[32];
  uint32_t color[32768];
  void update();
  Palette();
};

struct Interface : public SNES::Interface {
  int16_t state[Scancode::Limit];
  void video_refresh(const uint16_t *data, unsigned width, unsigned height);
  void audio_sample(uint16_t left, uint16_t right);
  void input_poll();
  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id);
};

extern Palette palette;
extern Interface interface;
