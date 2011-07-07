class Interface {
public:
  virtual void video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan) {}
  virtual void audio_sample(uint16_t l_sample, uint16_t r_sample) {}
  virtual int16_t input_poll(bool port, Input::Device device, unsigned index, unsigned id) { return 0; }

  virtual void message(const string &text) { print(text, "\n"); }
  virtual string path(Cartridge::Slot slot, const string &hint) = 0;
};
