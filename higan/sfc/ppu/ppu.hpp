struct PPU : Thread, PPUcounter {
  alwaysinline auto interlace() const -> bool { return display.interlace; }
  alwaysinline auto overscan() const -> bool { return display.overscan; }
  alwaysinline auto vdisp() const -> uint { return io.overscan ? 240 : 225; }

  PPU();
  ~PPU();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeCPU() -> void;

  static auto Enter() -> void;
  auto main() -> void;
  auto load(Markup::Node) -> bool;
  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  //io.cpp
  alwaysinline auto getVramAddress() -> uint16;
  alwaysinline auto vramAccessible() const -> bool;
  alwaysinline auto oamWrite(uint addr, uint8 data) -> void;
  auto readIO(uint24 addr, uint8 data) -> uint8;
  auto writeIO(uint24 addr, uint8 data) -> void;
  auto latchCounters() -> void;
  auto updateVideoMode() -> void;

privileged:
  struct VRAM {
    auto& operator[](uint offset) { return data[offset & mask]; }
    uint16 data[64 * 1024];
    uint mask = 0x7fff;
  } vram;

  uint32* output = nullptr;

  struct {
    bool interlace;
    bool overscan;
  } display;

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
    uint8 cgramAddress;
  } latch;

  struct IO {
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
    uint8 cgramAddress;
    uint1 cgramAddressLatch;

    //$2133  SETINI
    bool extbg;
    bool pseudoHires;
    bool overscan;
    bool interlace;

    //$213c  OPHCT
    uint16 hcounter;

    //$213d  OPVCT
    uint16 vcounter;
  } io;

  #include "background/background.hpp"
  #include "object/object.hpp"
  #include "window/window.hpp"
  #include "screen/screen.hpp"

  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  Object obj;
  Window window;
  Screen screen;

  friend class PPU::Background;
  friend class PPU::Object;
  friend class PPU::Window;
  friend class PPU::Screen;
  friend class Scheduler;
};

extern PPU ppu;
