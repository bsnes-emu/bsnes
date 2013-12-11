struct PPU : Thread, MMIO {
  uint8 vram[16384];  //GB = 8192, GBC = 16384
  uint8 oam[160];
  uint8 bgp[4];
  uint8 obp[2][4];
  uint8 bgpd[64];
  uint8 obpd[64];

  struct Status {
    unsigned lx;

    //$ff40  LCDC
    bool display_enable;
    bool window_tilemap_select;
    bool window_display_enable;
    bool bg_tiledata_select;
    bool bg_tilemap_select;
    bool ob_size;
    bool ob_enable;
    bool bg_enable;

    //$ff41  STAT
    bool interrupt_lyc;
    bool interrupt_oam;
    bool interrupt_vblank;
    bool interrupt_hblank;

    //$ff42  SCY
    uint8 scy;

    //$ff43  SCX
    uint8 scx;

    //$ff44  LY
    uint8 ly;

    //$ff45  LYC
    uint8 lyc;

    //$ff4a  WY
    uint8 wy;

    //$ff4b  WX
    uint8 wx;

    //$ff4f  VBK
    bool vram_bank;

    //$ff68  BGPI
    bool bgpi_increment;
    uint6 bgpi;

    //$ff6a  OBPI
    bool obpi_increment;
    uint8 obpi;
  } status;

  uint32 screen[160 * 144];

  struct Pixel {
    uint16 color;
    uint8 palette;
    bool priority;
  };
  Pixel bg;
  Pixel ob;

  struct Sprite {
    unsigned x;
    unsigned y;
    unsigned tile;
    unsigned attr;
    unsigned data;
  };
  Sprite sprite[10];
  unsigned sprites;

  unsigned px;

  struct Background {
    unsigned attr;
    unsigned data;
  };
  Background background;
  Background window;

  static void Main();
  void main();
  void add_clocks(unsigned clocks);
  void scanline();
  void frame();

  unsigned hflip(unsigned data) const;

  //mmio.cpp
  unsigned vram_addr(uint16 addr) const;
  uint8 mmio_read(uint16 addr);
  void mmio_write(uint16 addr, uint8 data);

  //dmg.cpp
  void dmg_read_tile(bool select, unsigned x, unsigned y, unsigned& data);
  void dmg_scanline();
  void dmg_run();
  void dmg_run_bg();
  void dmg_run_window();
  void dmg_run_ob();

  //cgb.cpp
  void cgb_read_tile(bool select, unsigned x, unsigned y, unsigned& attr, unsigned& data);
  void cgb_scanline();
  void cgb_run();
  void cgb_run_bg();
  void cgb_run_window();
  void cgb_run_ob();

  void power();

  void serialize(serializer&);
  PPU();
};

extern PPU ppu;
