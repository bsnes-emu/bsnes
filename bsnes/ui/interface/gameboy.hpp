struct InterfaceGameBoy : GameBoy::Interface {
  bool loadCartridge(const string &filename);
  void unloadCartridge();

  bool saveState(const string &filename);
  bool loadState(const string &filename);
  void setCheatCodes(const lstring &list);

  void video_refresh(const uint8_t *data);
  void audio_sample(int16_t csample, int16_t lsample, int16_t rsample);
  bool input_poll(unsigned id);
};
