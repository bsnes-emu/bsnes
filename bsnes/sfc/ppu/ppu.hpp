struct PPU : Thread, PPUcounter {
  alwaysinline auto interlace() const -> bool { return display.interlace; }
  alwaysinline auto overscan() const -> bool { return display.overscan; }
  alwaysinline auto vdisp() const -> uint { return display.vdisp; }

  //ppu.cpp
  PPU();
  ~PPU();

  static auto Enter() -> void;
  auto main() -> void;
  auto load() -> bool;
  auto power(bool reset) -> void;

  //io.cpp
  auto latchCounters() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  //ppu.cpp
  alwaysinline auto step(uint clocks) -> void;

  //io.cpp
  alwaysinline auto addressVRAM() const -> uint16;
  alwaysinline auto readVRAM() -> uint16;
  alwaysinline auto writeVRAM(bool byte, uint8 data) -> void;
  alwaysinline auto readOAM(uint10 addr) -> uint8;
  alwaysinline auto writeOAM(uint10 addr, uint8 data) -> void;
  alwaysinline auto readCGRAM(bool byte, uint8 addr) -> uint8;
  alwaysinline auto writeCGRAM(uint8 addr, uint15 data) -> void;
  auto readIO(uint24 addr, uint8 data) -> uint8;
  auto writeIO(uint24 addr, uint8 data) -> void;
  auto updateVideoMode() -> void;

  struct VRAM {
    auto& operator[](uint addr) { return data[addr & mask]; }
    uint16 data[64 * 1024];
    uint16 mask = 0x7fff;
  } vram;

  uint32* output = nullptr;

  struct {
    bool interlace;
    bool overscan;
    uint vdisp;
  } display;

  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  struct {
    uint4 version;
    uint8 mdr;
  } ppu1, ppu2;

  struct Latch {
    uint16 vram;
     uint8 oam;
     uint8 cgram;
     uint8 bgofsPPU1;
     uint3 bgofsPPU2;
     uint8 mode7;
     uint1 counters;
     uint1 hcounter;
     uint1 vcounter;

    uint10 oamAddress;
     uint8 cgramAddress;
  } latch;

  struct IO {
    //$2100  INIDISP
     uint1 displayDisable;
     uint4 displayBrightness;

    //$2102  OAMADDL
    //$2103  OAMADDH
    uint10 oamBaseAddress;
    uint10 oamAddress;
     uint1 oamPriority;

    //$2105  BGMODE
     uint1 bgPriority;
     uint8 bgMode;

    //$210d  BG1HOFS
    uint16 hoffsetMode7;

    //$210e  BG1VOFS
    uint16 voffsetMode7;

    //$2115  VMAIN
     uint1 vramIncrementMode;
     uint2 vramMapping;
     uint8 vramIncrementSize;

    //$2116  VMADDL
    //$2117  VMADDH
    uint16 vramAddress;

    //$211a  M7SEL
     uint2 repeatMode7;
     uint1 vflipMode7;
     uint1 hflipMode7;

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
     uint1 extbg;
     uint1 pseudoHires;
     uint1 overscan;
     uint1 interlace;

    //$213c  OPHCT
    uint16 hcounter;

    //$213d  OPVCT
    uint16 vcounter;
  } io;

  #include "background.hpp"
  #include "object.hpp"
  #include "window.hpp"
  #include "screen.hpp"

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
  friend class System;
  friend class PPUfast;
};

extern PPU ppu;
