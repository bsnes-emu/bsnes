struct InterfaceSNES : SNES::Interface {
  bool loadCartridge(const string &filename);
  void unloadCartridge();

  bool saveState(const string &filename);
  bool loadState(const string &filename);

  void videoRefresh(const uint16_t *data, bool hires, bool interlace, bool overscan);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id);

  string path(SNES::Cartridge::Slot slot, const string &hint);
};
