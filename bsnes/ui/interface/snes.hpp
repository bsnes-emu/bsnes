struct InterfaceSNES : SNES::Interface {
  void setController(bool port, unsigned device);

  bool loadCartridge(const string &filename);
  bool loadSatellaviewSlottedCartridge(const string &basename, const string &slotname);
  bool loadSatellaviewCartridge(const string &basename, const string &slotname);
  bool loadSufamiTurboCartridge(const string &basename, const string &slotAname, const string &slotBname);
  bool loadSuperGameBoyCartridge(const string &basename, const string &slotname);
  void unloadCartridge();

  void loadMemory();
  void saveMemory();

  bool saveState(const string &filename);
  bool loadState(const string &filename);

  void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id);

  string path(SNES::Cartridge::Slot slot, const string &hint);
  void message(const string &text);
};
