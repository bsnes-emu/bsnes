struct InterfaceSNES : InterfaceCore, SNES::Interface {
  void initialize();

  void setController(bool port, unsigned device);

  bool cartridgeLoaded();
  bool loadFile(string &filename, uint8_t *&data, unsigned &size);
  bool loadCartridge(string basename);
  bool loadSatellaviewSlottedCartridge(string basename, string slotname);
  bool loadSatellaviewCartridge(string basename, string slotname);
  bool loadSufamiTurboCartridge(string basename, string slotAname, string slotBname);
  bool loadSuperGameBoyCartridge(string basename, string slotname);
  void unloadCartridge();

  void power();
  void reset();
  void run();

  void loadMemory();
  void saveMemory();

  serializer serialize();
  bool unserialize(serializer&);

  void setCheats(const lstring &list = lstring{});

  void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id);

  string path(SNES::Cartridge::Slot slot, const string &hint);
  void message(const string &text);
};
