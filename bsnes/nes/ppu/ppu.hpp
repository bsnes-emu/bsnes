struct PPU : Processor {
  static void Main();
  void main();
  void add_clocks(unsigned clocks);

  void power();
  void reset();

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  uint8 bus_read(uint16 addr);
  void bus_write(uint16 addr, uint8 data);

  void render_scanline();

  bool raster_enable() const;
  unsigned nametable_addr() const;
  unsigned scrollx() const;
  unsigned scrolly() const;

  struct Status {
    uint8 mdr;

    unsigned ly;
    unsigned lx;

    uint8 bus_data;

    bool address_latch;

    uint15 vaddr;
    uint15 taddr;
    uint8  xaddr;

    //$2000
    bool nmi_enable;
    bool master_select;
    bool sprite_size;
    unsigned bg_addr;
    unsigned sprite_addr;
    unsigned vram_increment;

    //$2001
    bool intensify_blue;
    bool intensify_green;
    bool intensify_red;
    bool sprite_enable;
    bool bg_enable;
    bool sprite_edge_enable;
    bool bg_edge_enable;
    bool grayscale;

    //$2002
    bool nmi;
    bool sprite_zero_hit;
    bool sprite_overflow;

    //$2003
    uint8 oam_addr;
  } status;

  uint32 buffer[256 * 240];
  uint32 paletteRGB[64];
  uint8 nram[2048];
  uint8 pram[32];
  uint8 sram[256];
};

extern PPU ppu;
