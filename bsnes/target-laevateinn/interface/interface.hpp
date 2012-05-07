struct Interface : Emulator::Interface::Bind {
  string pathName;

  bool loadCartridge(const string &foldername);
  void loadMemory();
  void saveMemory();
  bool loadState(unsigned slot);
  bool saveState(unsigned slot);

  void loadRequest(unsigned id, const string &path) {}
  void loadRequest(unsigned id, const string &name, const string &type, const string &path) {}
  uint32_t videoColor(unsigned source, uint16_t red, uint16_t green, uint16_t blue);
  void videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(unsigned port, unsigned device, unsigned index);
  unsigned dipSettings(const XML::Node &node) { return 0u; }
  string path(unsigned group) { return ""; }

  string path(SFC::Cartridge::Slot slot, const string &hint);
  void message(const string &text);

  Interface();

private:
  SFC::Interface *instance;
};

extern Interface *interface;
