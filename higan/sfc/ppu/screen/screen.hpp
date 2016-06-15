struct Screen {
  auto scanline() -> void;
  alwaysinline auto run() -> void;
  auto reset() -> void;

  auto below(bool hires) -> uint16;
  auto above() -> uint16;

  auto blend(uint x, uint y) const -> uint16;
  alwaysinline auto paletteColor(uint palette) const -> uint16;
  alwaysinline auto directColor(uint palette, uint tile) const -> uint16;
  alwaysinline auto fixedColor() const -> uint16;

  auto serialize(serializer&) -> void;

  uint32* lineA;
  uint32* lineB;

  struct Registers {
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
  } r;

  struct Math {
    struct Screen {
      uint16 color;
      bool colorEnable;
    } above, below;
    bool transparent;
    bool blendMode;
    bool colorHalve;
  } math;

  friend class PPU;
};
