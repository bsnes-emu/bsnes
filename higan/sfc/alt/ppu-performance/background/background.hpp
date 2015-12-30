class Background {
  struct ID { enum : uint { BG1, BG2, BG3, BG4 }; };
  struct Mode { enum : uint { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenSize { enum : uint { Size32x32, Size32x64, Size64x32, Size64x64 }; };
  struct TileSize { enum : uint { Size8x8, Size16x16 }; };

  Background(PPU& self, uint id);
  ~Background();

  alwaysinline auto get_tile(uint hoffset, uint voffset) -> uint;
  auto offset_per_tile(uint x, uint y, uint& hoffset, uint& voffset) -> void;
  auto scanline() -> void;
  auto render() -> void;
  auto render_mode7() -> void;

  auto serialize(serializer&) -> void;

  PPU& self;

  bool priority0_enable;
  bool priority1_enable;

  struct Regs {
    uint mode;
    uint priority0;
    uint priority1;

    bool tile_size;
    uint mosaic;

    uint screen_addr;
    uint screen_size;
    uint tiledata_addr;

    uint hoffset;
    uint voffset;

    bool main_enable;
    bool sub_enable;
  } regs;

  uint16** mosaic_table;

  const uint id;
  uint opt_valid_bit;

  bool hires;
  signed width;

  uint tile_width;
  uint tile_height;

  uint mask_x;
  uint mask_y;

  uint scx;
  uint scy;

  uint hscroll;
  uint vscroll;

  uint mosaic_vcounter;
  uint mosaic_voffset;

  LayerWindow window;

  friend class PPU;
};
