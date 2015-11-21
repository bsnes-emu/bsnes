struct PPU : Thread, MMIO {
  static auto Main() -> void;
  auto main() -> void;
  auto add_clocks(uint clocks) -> void;
  auto scanline() -> void;
  auto frame() -> void;

  auto hflip(uint data) const -> uint;

  //mmio.cpp
  auto vram_addr(uint16 addr) const -> uint;
  auto mmio_read(uint16 addr) -> uint8;
  auto mmio_write(uint16 addr, uint8 data) -> void;

  //dmg.cpp
  auto dmg_read_tile(bool select, uint x, uint y, uint& data) -> void;
  auto dmg_scanline() -> void;
  auto dmg_run() -> void;
  auto dmg_run_bg() -> void;
  auto dmg_run_window() -> void;
  auto dmg_run_ob() -> void;

  //cgb.cpp
  auto cgb_read_tile(bool select, uint x, uint y, uint& attr, uint& data) -> void;
  auto cgb_scanline() -> void;
  auto cgb_run() -> void;
  auto cgb_run_bg() -> void;
  auto cgb_run_window() -> void;
  auto cgb_run_ob() -> void;

  auto power() -> void;

  auto serialize(serializer&) -> void;

  uint8 vram[16384];  //GB = 8192, GBC = 16384
  uint8 oam[160];
  uint8 bgp[4];
  uint8 obp[2][4];
  uint8 bgpd[64];
  uint8 obpd[64];

  struct Status {
    uint lx;

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
    uint x;
    uint y;
    uint tile;
    uint attr;
    uint data;
  };
  Sprite sprite[10];
  uint sprites;

  uint px;

  struct Background {
    uint attr;
    uint data;
  };
  Background background;
  Background window;
};

extern PPU ppu;
