struct Palette {
  alwaysinline uint8_t operator[](uint8_t color);

  uint8_t contrastAdjust(uint8_t);
  uint8_t brightnessAdjust(uint8_t);
  uint8_t gammaAdjust(uint8_t);
  void update();

private:
  uint32_t color[256];
};

extern Palette palette;
