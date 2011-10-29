struct Interface {
  virtual void videoRefresh(const uint16_t *data);
  virtual void audioSample(int16_t sample);
  virtual int16_t inputPoll(bool port, unsigned device, unsigned id);

  virtual void message(const string &text);
};

extern Interface *interface;
