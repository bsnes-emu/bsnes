struct Screen {
  auto scanline() -> void;
  alwaysinline auto run() -> void;
  auto power() -> void;

  auto below(bool hires) -> uint16;
  auto above() -> uint16;

  auto blend(uint x, uint y) const -> uint15;
  alwaysinline auto paletteColor(uint8 palette) const -> uint15;
  alwaysinline auto directColor(uint palette, uint tile) const -> uint15;
  alwaysinline auto fixedColor() const -> uint15;

  auto serialize(serializer&) -> void;

  uint16_t* lineA;
  uint16_t* lineB;

  uint15 cgram[256];

  struct IO {
    bool blendMode;
    bool directColor;

    bool colorMode;
    bool colorHalve;
    struct Layer {
      bool colorEnable;
    } bg1, bg2, bg3, bg4, obj, back;

    uint5 colorBlue;
    uint5 colorGreen;
    uint5 colorRed;
  } io;

  struct Math {
    struct Screen {
      uint15 color;
      bool colorEnable;
    } above, below;
    bool transparent;
    bool blendMode;
    bool colorHalve;
  } math;

  friend class PPU;
};
