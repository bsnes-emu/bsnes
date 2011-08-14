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
  void video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan);
  void audio_sample(int16_t left, int16_t right);
  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id);

  void message(const string &text);
  string path(SNES::Cartridge::Slot slot, const string &hint);
  Interface();

  bool captureScreenshot;
};

extern Palette palette;
extern Filter filter;
extern Interface interface;
