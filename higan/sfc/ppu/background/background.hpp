struct Background {
  Background(uint id) : id(id) {}

  alwaysinline auto voffset() const -> uint16;
  alwaysinline auto hoffset() const -> uint16;

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
    uint4 mosaic;
    bool tileSize;

    uint mode;
    uint priority[2];

    bool aboveEnable;
    bool belowEnable;

    uint16 hoffset;
    uint16 voffset;
  } io;

  struct Latch {
    uint16 hoffset;
    uint16 voffset;
  } latch;

  struct Output {
    struct Pixel {
      uint priority;  //0 = none (transparent)
      uint8 palette;
      uint16 tile;
    } above, below;
  } output;

  struct Mosaic : Output::Pixel {
    uint vcounter;
    uint voffset;
    uint hcounter;
    uint hoffset;
  } mosaic;

  struct {
    int x;
    int y;

    uint tileCounter;
    uint tile;
    uint priority;
    uint paletteNumber;
    uint paletteIndex;
    uint32 data[2];
  };

  friend class PPU;
};
