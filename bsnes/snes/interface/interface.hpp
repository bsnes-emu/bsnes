class Interface {
public:
  virtual void videoRefresh(const uint16_t *data, bool hires, bool interlace, bool overscan);
  virtual void audioSample(int16_t lsample, int16_t rsample);
  virtual int16_t inputPoll(bool port, Input::Device device, unsigned index, unsigned id);

  virtual void initialize(Interface*);

  virtual void connect(bool port, Input::Device device);

  virtual bool cartridgeLoaded();
  virtual void loadCartridge(const string &xml, const uint8_t *data, unsigned size);
  virtual void unloadCartridge();

  virtual void power();
  virtual void reset();
  virtual void run();

  virtual serializer serialize();
  virtual bool unserialize(serializer&);

  void setCheats(const lstring &list = lstring{});

  virtual void message(const string &text);
  virtual string path(Cartridge::Slot slot, const string &hint) = 0;
};

extern Interface *interface;
