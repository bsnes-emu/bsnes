struct InterfaceSNES : SNES::Interface {
  bool loadCartridge(const string &filename);
  void unloadCartridge();

  void video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan);
  void audio_sample(int16_t lsample, int16_t rsample);
  int16_t input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id);

  string path(SNES::Cartridge::Slot slot, const string &hint);
};
