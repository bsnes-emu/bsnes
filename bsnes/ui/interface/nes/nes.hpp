struct InterfaceNES : InterfaceCore, NES::Interface {
  void initialize();
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

  void videoRefresh(const uint16_t *data);
  void audioSample(int16_t sample);
  int16_t inputPoll(bool port, unsigned device, unsigned id);
};
