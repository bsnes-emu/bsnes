struct Background {
  struct ID { enum : uint { BG1, BG2, BG3, BG4 }; };
  const uint id;

  struct Mode { enum : uint { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenSize { enum : uint { Size32x32, Size32x64, Size64x32, Size64x64 }; };
  struct TileSize { enum : uint { Size8x8, Size16x16 }; };
  struct Screen { enum : uint { Main, Sub }; };

  Background(PPU& self, uint id);

  struct Regs {
    uint16 tiledata_addr;
    uint16 screen_addr;
    uint2 screen_size;
    uint4 mosaic;
    bool tile_size;

    uint mode;
    uint priority0;
    uint priority1;

    bool main_enable;
    bool sub_enable;

    uint16 hoffset;
    uint16 voffset;
  } regs;

  struct Cache {
    uint16 hoffset;
    uint16 voffset;
  } cache;

  alwaysinline auto voffset() const -> uint;
  alwaysinline auto hoffset() const -> uint;

  struct Output {
    struct Pixel {
      uint priority;  //0 = none (transparent)
      uint8 palette;
      uint16 tile;
    } main, sub;
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

    uint tile_counter;
    uint tile;
    uint priority;
    uint palette_number;
    uint palette_index;
    uint8 data[8];
  };

  auto frame() -> void;
  auto scanline() -> void;
  auto begin() -> void;
  auto run(bool screen) -> void;
  auto reset() -> void;

  auto get_tile() -> void;
  auto get_tile_color() -> uint;
  auto get_tile(uint x, uint y) -> uint;
  alwaysinline auto clip(int n) -> int;
  auto begin_mode7() -> void;
  auto run_mode7() -> void;

  auto serialize(serializer&) -> void;

  PPU& self;
  friend class PPU;
};
