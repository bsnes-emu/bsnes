public:
  auto mmio_read(uint addr, uint8 data) -> uint8;
  auto mmio_write(uint addr, uint8 data) -> void;

private:

struct Regs {
  //internal
  uint8 ppu1_mdr;
  uint8 ppu2_mdr;

  uint16 vram_readbuffer;
  uint8 oam_latchdata;
  uint8 cgram_latchdata;
  uint8 bgofs_latchdata;
  uint8 mode7_latchdata;

  bool counters_latched;
  bool latch_hcounter;
  bool latch_vcounter;

  //$2100
  bool display_disable;
  uint display_brightness;

  //$2102-$2103
  uint16 oam_baseaddr;
  uint16 oam_addr;
  bool oam_priority;

  //$2105
  bool bg3_priority;
  uint bgmode;

  //$210d
  uint16 mode7_hoffset;

  //$210e
  uint16 mode7_voffset;

  //$2115
  bool vram_incmode;
  uint vram_mapping;
  uint vram_incsize;

  //$2116-$2117
  uint16 vram_addr;

  //$211a
  uint mode7_repeat;
  bool mode7_vflip;
  bool mode7_hflip;

  //$211b-$2120
  uint16 m7a;
  uint16 m7b;
  uint16 m7c;
  uint16 m7d;
  uint16 m7x;
  uint16 m7y;

  //$2121
  uint16 cgram_addr;

  //$2126-$212a
  uint window_one_left;
  uint window_one_right;
  uint window_two_left;
  uint window_two_right;

  //$2133
  bool mode7_extbg;
  bool pseudo_hires;
  bool overscan;
  bool interlace;

  //$213c
  uint16 hcounter;

  //$213d
  uint16 vcounter;
} regs;

auto get_vram_addr() -> uint16;
auto vram_read(uint addr) -> uint8;
auto vram_write(uint addr, uint8 data) -> void;

auto oam_read(uint addr) -> uint8;
auto oam_write(uint addr, uint8 data) -> void;

auto cgram_read(uint addr) -> uint8;
auto cgram_write(uint addr, uint8 data) -> void;

auto mmio_update_video_mode() -> void;
auto mmio_reset() -> void;
