struct Background {
  Background(uint id) : id(id) {}

  alwaysinline auto hires() const -> bool;
  alwaysinline auto hoffset() const -> uint16;
  alwaysinline auto voffset() const -> uint16;

  auto frame() -> void;
  auto scanline() -> void;
  auto begin() -> void;
  auto run(bool screen) -> void;
  auto power() -> void;

  auto getTile() -> void;
  auto getTileColor() -> uint;
  auto getTile(uint x, uint y) -> uint;
  alwaysinline auto clip(int n) -> int;
  auto beginMode7() -> void;
  auto runMode7() -> void;

  auto serialize(serializer&) -> void;

  struct ID { enum : uint { BG1, BG2, BG3, BG4 }; };
  const uint id;

  struct Mode { enum : uint { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenSize { enum : uint { Size32x32, Size32x64, Size64x32, Size64x64 }; };
  struct TileSize { enum : uint { Size8x8, Size16x16 }; };
  struct Screen { enum : uint { Above, Below }; };

  struct IO {
    uint16 tiledataAddress;
    uint16 screenAddress;
    uint2 screenSize;
    uint1 tileSize;

    uint8 mode;
    uint8 priority[2];

    uint1 aboveEnable;
    uint1 belowEnable;

    uint16 hoffset;
    uint16 voffset;
  } io;

  struct Latch {
    uint16 hoffset;
    uint16 voffset;
  } latch;

  struct Pixel {
    uint8 priority;  //0 = none (transparent)
    uint8 palette;
    uint16 tile;
  } above, below;

  struct Output {
    Pixel above;
    Pixel below;
  } output;

  struct Mosaic {
    static uint4 size;
    uint1 enable;

    uint16 vcounter;
    uint16 hcounter;

    uint16 voffset;
    uint16 hoffset;

    Pixel pixel;
  } mosaic;

  int x;
  int y;

  uint3 tileCounter;
  uint16 tile;
  uint8 priority;
  uint3 paletteNumber;
  uint8 paletteIndex;
  uint32 data[2];

  friend class PPU;
};
