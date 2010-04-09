class Background {
public:
  sPPU &self;
  struct ID { enum { BG1, BG2, BG3, BG4 }; };
  unsigned id;

  struct Mode { enum  { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenSize { enum { Size32x32, Size32x64, Size64x32, Size64x64 }; };
  struct TileSize { enum { Size8x8, Size16x16 }; };

  struct {
    unsigned x;
    unsigned mosaic_y;
    unsigned mosaic_countdown;
  } state;

  struct {
    unsigned tiledata_addr;
    unsigned screen_addr;
    unsigned screen_size;
    unsigned mosaic;
    bool tile_size;

    unsigned mode;
    unsigned priority0;
    unsigned priority1;

    bool main_enabled;
    bool sub_enabled;

    unsigned hoffset;
    unsigned voffset;
  } regs;

  struct {
    struct {
      unsigned priority;  //0 = none (transparent)
      unsigned palette;   //(direct_color_bits << 8) + index
    } main, sub;
  } output;

  void scanline();
  void run();
  unsigned get_color(unsigned x, unsigned y, uint16 offset);
  void reset();
  Background(sPPU &self, unsigned id);

private:
  static uint16 mosaic_table[16][4096];

  //mode7.cpp
  signed clip(signed n);
  void run_mode7(unsigned x, unsigned y);
};
