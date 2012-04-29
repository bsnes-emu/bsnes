struct InterfaceNES : InterfaceCore, FC::Interface {
  void initialize();

  string markup();

  void setController(bool port, unsigned device);

  bool cartridgeLoaded();
  bool loadCartridge(const string &filename);
  void unloadCartridge();

  void power();
  void reset();
  void run();

  serializer serialize();
  bool unserialize(serializer&);

  void setCheats(const lstring &list = lstring{});

  uint32_t videoColor(uint9_t source, uint16_t red, uint16_t green, uint16_t blue);
  void videoRefresh(const uint32_t *data);
  void audioSample(int16_t sample);
  int16_t inputPoll(bool port, unsigned device, unsigned id);
};
