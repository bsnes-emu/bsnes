struct Palette {
  static const uint8_t gammaRamp[32];
  uint32_t color[32768];
  uint8_t contrastAdjust(uint8_t);
  uint8_t brightnessAdjust(uint8_t);
  uint8_t gammaAdjust(uint8_t);
  void update();
};

struct Filter : public library {
  function<void (unsigned&, unsigned&)> dl_size;
  function<void (uint32_t*, uint32_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned)> dl_render;
  void size(unsigned &width, unsigned &height);
  void render(uint32_t *output, unsigned outpitch, const uint16_t *input, unsigned pitch, unsigned width, unsigned height);
};

struct Interface : public SNES::Interface {
  void video_refresh(const uint16_t *data, unsigned width, unsigned height);
  void audio_sample(uint16_t left, uint16_t right);
  void input_poll();
  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id);
};

extern Palette palette;
extern Filter filter;
extern Interface interface;
