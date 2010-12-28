class Interface {
public:
  virtual void video_refresh(const uint8_t *data) {}
  virtual void audio_sample(signed left, signed right) {}
  virtual void input_poll() {}
  virtual bool input_poll(unsigned id) {}

  virtual void message(const string &text) { print(text, "\n"); }
};
