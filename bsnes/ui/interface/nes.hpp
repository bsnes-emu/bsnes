struct InterfaceNES : NES::Interface {
  void setController(bool port, unsigned device);

  bool loadCartridge(const string &filename);
  void unloadCartridge();

  bool saveState(const string &filename);
  bool loadState(const string &filename);

  void videoRefresh(const uint16_t *data);
  void audioSample(int16_t sample);
  int16_t inputPoll(bool port, unsigned device, unsigned id);

  InterfaceNES();

private:
  unsigned palette[512];
};
