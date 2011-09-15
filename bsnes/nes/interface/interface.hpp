struct Interface {
  virtual void videoRefresh(const uint16_t *data);
  virtual void audioSample(int16_t sample);
  virtual int16_t inputPoll(bool port, unsigned device, unsigned id);

  virtual void initialize(Interface*);

  virtual bool cartridgeLoaded();
  virtual void loadCartridge(const string &xml, const uint8_t *data, unsigned size);
  virtual void unloadCartridge();

  virtual void power();
  virtual void reset();
  virtual void run();

  virtual void setCheats(const lstring &list = lstring{});

  virtual void message(const string &text);
};

extern Interface *interface;
