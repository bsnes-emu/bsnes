struct InterfaceGameBoy : GameBoy::Interface {
  bool loadCartridge(GameBoy::System::Revision revision, const string &filename);
  void unloadCartridge();

  bool saveState(const string &filename);
  bool loadState(const string &filename);

  void videoRefresh(const uint16_t *data);
  void audioSample(int16_t csample, int16_t lsample, int16_t rsample);
  bool inputPoll(unsigned id);
};
