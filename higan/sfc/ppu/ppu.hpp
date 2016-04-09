#include "video.hpp"

struct PPU : Thread, PPUcounter {
  alwaysinline auto interlace() const -> bool { return display.interlace; }
  alwaysinline auto overscan() const -> bool { return display.overscan; }
  alwaysinline auto vdisp() const -> uint { return !regs.overscan ? 225 : 240; }

  PPU();
  ~PPU();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeCPU() -> void;

  static auto Enter() -> void;
  auto main() -> void;
  auto enable() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  //memory.cpp
  alwaysinline auto getVramAddress() -> uint16;
  alwaysinline auto vramRead(uint addr) -> uint8;
  alwaysinline auto vramWrite(uint addr, uint8 data) -> void;
  alwaysinline auto oamRead(uint addr) -> uint8;
  alwaysinline auto oamWrite(uint addr, uint8 data) -> void;
  alwaysinline auto cgramRead(uint addr) -> uint8;
  alwaysinline auto cgramWrite(uint addr, uint8 data) -> void;

  //mmio.cpp
  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;
  auto latchCounters() -> void;
  auto updateVideoMode() -> void;

  uint8 vram[64 * 1024];
  uint8 oam[544];
  uint8 cgram[512];

privileged:
  uint ppu1_version = 1;  //allowed: 1
  uint ppu2_version = 3;  //allowed: 1, 2, 3

  uint32* output = nullptr;

  struct {
    bool interlace;
    bool overscan;
  } display;

  alwaysinline auto addClocks(uint) -> void;

  auto scanline() -> void;
  auto frame() -> void;

  struct Registers {
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

    uint10 oam_iaddr;
    uint9 cgram_iaddr;

    //$2100  INIDISP
    bool display_disable;
    uint4 display_brightness;

    //$2102  OAMADDL
    //$2103  OAMADDH
    uint10 oam_baseaddr;
    uint10 oam_addr;
    bool oam_priority;

    //$2105  BGMODE
    bool bg3_priority;
    uint8 bgmode;

    //$210d  BG1HOFS
    uint16 mode7_hoffset;

    //$210e  BG1VOFS
    uint16 mode7_voffset;

    //$2115  VMAIN
    bool vram_incmode;
    uint2 vram_mapping;
    uint8 vram_incsize;

    //$2116  VMADDL
    //$2117  VMADDH
    uint16 vram_addr;

    //$211a  M7SEL
    uint2 mode7_repeat;
    bool mode7_vflip;
    bool mode7_hflip;

    //$211b  M7A
    uint16 m7a;

    //$211c  M7B
    uint16 m7b;

    //$211d  M7C
    uint16 m7c;

    //$211e  M7D
    uint16 m7d;

    //$211f  M7X
    uint16 m7x;

    //$2120  M7Y
    uint16 m7y;

    //$2121  CGADD
    uint9 cgram_addr;

    //$2133  SETINI
    bool mode7_extbg;
    bool pseudo_hires;
    bool overscan;
    bool interlace;

    //$213c  OPHCT
    uint16 hcounter;

    //$213d  OPVCT
    uint16 vcounter;
  } regs;

  #include "background/background.hpp"
  #include "screen/screen.hpp"
  #include "sprite/sprite.hpp"
  #include "window/window.hpp"

  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  Sprite sprite;
  Window window;
  Screen screen;

  friend class PPU::Background;
  friend class PPU::Sprite;
  friend class PPU::Window;
  friend class PPU::Screen;
  friend class Video;

  struct Debugger {
    hook<auto (uint16, uint8) -> void> vram_read;
    hook<auto (uint16, uint8) -> void> oam_read;
    hook<auto (uint16, uint8) -> void> cgram_read;
    hook<auto (uint16, uint8) -> void> vram_write;
    hook<auto (uint16, uint8) -> void> oam_write;
    hook<auto (uint16, uint8) -> void> cgram_write;
  } debugger;
};

extern PPU ppu;
