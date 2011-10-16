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
  unsigned paletteColor(
    unsigned color, double saturation = 1.0, double hue = 0.0,
    double contrast = 1.0, double brightness = 1.0, double gamma = 1.8
  );
};
