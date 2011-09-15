struct InterfaceNES : NES::Interface {
  bool loadCartridge(const string &filename);
  void unloadCartridge();

  void videoRefresh(const uint16_t *data);
  void audioSample(int16_t sample);
  int16_t inputPoll(bool port, unsigned device, unsigned id);

  InterfaceNES();

private:
  unsigned palette[512];
};
