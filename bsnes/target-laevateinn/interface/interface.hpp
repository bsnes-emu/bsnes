struct Interface : SFC::Interface {
  string pathName;

  bool loadCartridge(const string &foldername);
  void loadMemory();
  void saveMemory();
  bool loadState(unsigned slot);
  bool saveState(unsigned slot);

  void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(bool port, SFC::Input::Device device, unsigned index, unsigned id);

  string path(SFC::Cartridge::Slot slot, const string &hint);
  void message(const string &text);

  Interface();
};

extern Interface *interface;
