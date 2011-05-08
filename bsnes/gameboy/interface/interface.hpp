class Interface {
public:
  virtual void lcd_scanline() {}
  virtual void joyp_write(bool p15, bool p14) {}

  virtual void video_refresh(const uint8_t *data) {}
  virtual void audio_sample(int16_t center, int16_t left, int16_t right) {}
  virtual void input_poll() {}
  virtual bool input_poll(unsigned id) {}

  virtual void message(const string &text) { print(text, "\n"); }
};
