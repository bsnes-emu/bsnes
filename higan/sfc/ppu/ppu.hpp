struct PPU : Thread, PPUcounter {
  alwaysinline auto interlace() const -> bool { return display.interlace; }
  alwaysinline auto overscan() const -> bool { return display.overscan; }
  alwaysinline auto vdisp() const -> uint { return r.overscan ? 240 : 225; }

  PPU();
  ~PPU();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeCPU() -> void;

  static auto Enter() -> void;
  auto main() -> void;
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

  struct {
    uint8 vram[64 * 1024];
    uint8 oam[544];
    uint8 cgram[512];
  } memory;

privileged:
  uint32* output = nullptr;

  struct {
    bool interlace;
    bool overscan;
  } display;

  alwaysinline auto addClocks(uint) -> void;

  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  struct {
    uint version;
    uint8 mdr;
  } ppu1, ppu2;

  struct Latches {
    uint16 vram;
    uint8 oam;
    uint8 cgram;
    uint8 bgofs;
    uint8 mode7;
    bool counters;
    bool hcounter;
    bool vcounter;

    uint10 oamAddress;
    uint9 cgramAddress;
  } latch;

  struct Registers {
    //$2100  INIDISP
    bool displayDisable;
    uint4 displayBrightness;

    //$2102  OAMADDL
    //$2103  OAMADDH
    uint10 oamBaseAddress;
    uint10 oamAddress;
    bool oamPriority;

    //$2105  BGMODE
    bool bgPriority;
    uint8 bgMode;

    //$210d  BG1HOFS
    uint16 hoffsetMode7;

    //$210e  BG1VOFS
    uint16 voffsetMode7;

    //$2115  VMAIN
    bool vramIncrementMode;
    uint2 vramMapping;
    uint8 vramIncrementSize;

    //$2116  VMADDL
    //$2117  VMADDH
    uint16 vramAddress;

    //$211a  M7SEL
    uint2 repeatMode7;
    bool vflipMode7;
    bool hflipMode7;

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
    uint9 cgramAddress;

    //$2133  SETINI
    bool extbg;
    bool pseudoHires;
    bool overscan;
    bool interlace;

    //$213c  OPHCT
    uint16 hcounter;

    //$213d  OPVCT
    uint16 vcounter;
  } r;

  #include "background/background.hpp"
  #include "screen/screen.hpp"
  #include "sprite/sprite.hpp"
  #include "window/window.hpp"

  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  OAM oam;
  Window window;
  Screen screen;

  friend class PPU::Background;
  friend class PPU::OAM;
  friend class PPU::Window;
  friend class PPU::Screen;
  friend class Scheduler;

  struct Debugger {
    hook<auto (uint16, uint8) -> void> vramRead;
    hook<auto (uint16, uint8) -> void> oamRead;
    hook<auto (uint16, uint8) -> void> cgramRead;
    hook<auto (uint16, uint8) -> void> vramWrite;
    hook<auto (uint16, uint8) -> void> oamWrite;
    hook<auto (uint16, uint8) -> void> cgramWrite;
  } debugger;
};

extern PPU ppu;
