struct Interface {
  virtual uint32_t videoColor(uint15_t source, uint16_t red, uint16_t green, uint16_t blue);
  virtual void videoRefresh(const uint32_t *data);
  virtual void audioSample(int16_t lsample, int16_t rsample);
  virtual bool inputPoll(unsigned id);

  virtual void message(const string &text);
};

extern Interface *interface;
