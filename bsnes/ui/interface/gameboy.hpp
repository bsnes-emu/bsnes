struct InterfaceGameBoy : GameBoy::Interface {
  bool loadCartridge(const string &filename);
  void unloadCartridge();

  bool saveState(const string &filename);
  bool loadState(const string &filename);

  void videoRefresh(const uint8_t *data);
  void audioSample(int16_t csample, int16_t lsample, int16_t rsample);
  bool inputPoll(unsigned id);
};
