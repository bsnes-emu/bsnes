struct Interface : public GameBoy::Interface {
  int16_t inputState[Scancode::Limit];

  void video_refresh(const uint8_t *data);
  void audio_sample(int16_t center, int16_t left, int16_t right);
  void input_poll();
  bool input_poll(unsigned id);

  void message(const string &text);
};

extern Interface interface;
