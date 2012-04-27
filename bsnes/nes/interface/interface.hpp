struct Interface {
  virtual uint32_t videoColor(uint9_t source, uint16_t red, uint16_t green, uint16_t blue);
  virtual void videoRefresh(const uint32_t *data);
  virtual void audioSample(int16_t sample);
  virtual int16_t inputPoll(bool port, unsigned device, unsigned id);
  virtual void message(const string &text);

  void loadCartridge(const string &markup, const stream &memory);
};

extern Interface *interface;
