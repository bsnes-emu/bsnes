class Background {
  struct ID { enum { BG1, BG2, BG3, BG4 }; };
  unsigned id;

  struct Mode { enum  { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenSize { enum { Size32x32, Size32x64, Size64x32, Size64x64 }; };
  struct TileSize { enum { Size8x8, Size16x16 }; };

  struct Regs {
    unsigned tiledata_addr;
    unsigned screen_addr;
    unsigned screen_size;
    unsigned mosaic;
    bool tile_size;

    unsigned mode;
    unsigned priority0;
    unsigned priority1;

    bool main_enable;
    bool sub_enable;

    unsigned hoffset;
    unsigned voffset;
  } regs;

  struct Output {
    struct Pixel {
      unsigned priority;  //0 = none (transparent)
      unsigned palette;
      unsigned tile;
    } main, sub;
  } output;

  struct {
    signed x;
    signed y;
    signed edge;

    unsigned mosaic_vcounter;
    unsigned mosaic_voffset;
    unsigned mosaic_hcounter;
    unsigned mosaic_hoffset;
    unsigned mosaic_palette;

    unsigned tile;
    unsigned priority;
    unsigned palette_number;
    unsigned palette_index;
    uint8 data[8];
  };

  void frame();
  void scanline();
  void run();
  void reset();

  void get_tile();
  unsigned get_tile_color();
  unsigned get_tile(unsigned x, unsigned y);
  signed clip(signed n);
  void run_mode7();

  void serialize(serializer&);
  Background(PPU &self, unsigned id);

  PPU &self;
  friend class PPU;
};
