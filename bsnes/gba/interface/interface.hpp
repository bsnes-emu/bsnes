struct Interface {
  virtual void videoRefresh(const uint16_t *data);
  virtual void audioSample(int16_t lsample, int16_t rsample);
  virtual bool inputPoll(unsigned id);

  virtual void message(const string &text);
};

extern Interface *interface;
