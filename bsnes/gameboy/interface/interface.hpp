class Interface {
public:
  virtual void lcdScanline();
  virtual void joypWrite(bool p15, bool p14);

  virtual void videoRefresh(const uint8_t *data);
  virtual void audioSample(int16_t center, int16_t left, int16_t right);
  virtual bool inputPoll(unsigned id);

  virtual void initialize(Interface*);

  virtual bool cartridgeLoaded();
  virtual void loadCartridge(const string &xml, const uint8_t *data, unsigned size);
  virtual void unloadCartridge();

  virtual void power();
  virtual void run();

  virtual serializer serialize();
  virtual bool unserialize(serializer&);

  virtual void setCheats(const lstring &list = lstring{});

  virtual void message(const string &text);
};

extern Interface *interface;
