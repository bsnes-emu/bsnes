struct Interface : SNES::Interface {
  string fileName;
  string baseName;
  string pathName;

  bool loadCartridge(const string &filename);
  void loadMemory();
  void saveMemory();
  bool loadState(unsigned slot);
  bool saveState(unsigned slot);

  void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id);

  string path(SNES::Cartridge::Slot slot, const string &hint);
  void message(const string &text);

  Interface();
};

extern Interface *interface;
