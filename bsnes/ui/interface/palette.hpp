struct Palette {
  alwaysinline uint32_t operator[](unsigned n);

  uint8_t contrastAdjust(uint8_t);
  uint8_t brightnessAdjust(uint8_t);
  uint8_t gammaAdjust(uint8_t);
  void update();

private:
  static const uint8_t gammaRamp[32];
  uint32_t color[32768];
};

extern Palette palette;
