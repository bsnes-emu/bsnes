struct Interface {
  virtual void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan);
  virtual void audioSample(int16_t lsample, int16_t rsample);
  virtual int16_t inputPoll(bool port, Input::Device device, unsigned index, unsigned id);

  virtual void initialize(Interface*);

  virtual void connect(bool port, Input::Device device);

  struct CartridgeData {
    string markup;
    const uint8_t *data;
    unsigned size;
  };

  virtual bool cartridgeLoaded();
  virtual void loadCartridge(const CartridgeData &base);
  virtual void loadSatellaviewSlottedCartridge(const CartridgeData &base, const CartridgeData &slot);
  virtual void loadSatellaviewCartridge(const CartridgeData &base, const CartridgeData &slot);
  virtual void loadSufamiTurboCartridge(const CartridgeData &base, const CartridgeData &slotA, const CartridgeData &slotB);
  virtual void loadSuperGameBoyCartridge(const CartridgeData &base, const CartridgeData &slot);
  virtual void unloadCartridge();

  Cartridge::Information& information();
  linear_vector<Cartridge::NonVolatileRAM>& memory();

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
