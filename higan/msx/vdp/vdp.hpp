//Texas Instruments TMS9918A

struct VDP : Thread {
  //vdp.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto power() -> void;

  //io.cpp
  auto status() -> uint8;
  auto data() -> uint8;

  auto data(uint8) -> void;
  auto control(uint8) -> void;

  //background.cpp
  auto background(uint8 hoffset, uint8 voffset) -> void;
  auto text1(uint8 hoffset, uint8 voffset) -> void;
  auto graphics1(uint8 hoffset, uint8 voffset) -> void;
  auto graphics2(uint8 hoffset, uint8 voffset) -> void;
  auto multicolor(uint8 hoffset, uint8 voffset) -> void;

  //sprites.cpp
  auto sprite(uint8 voffset) -> void;
  auto sprite(uint8 hoffset, uint8 voffset) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  uint32 buffer[256 * 192];
  Emulator::Memory::Writable<uint8> vram;

  struct IO {
    uint vcounter = 0;
    uint hcounter = 0;

    uint1  controlLatch;
    uint16 controlValue;
    uint8  vramLatch;

    uint5 spriteOverflowIndex;
    uint1 spriteCollision;
    uint1 spriteOverflow;
    uint1 irqLine;

    uint1 externalInput;
    uint3 videoMode;
    uint1 spriteZoom;
    uint1 spriteSize;
    uint1 irqEnable;
    uint1 displayEnable;
    uint1 ramMode = 1;  //0 = 4KB; 1 = 16KB
    uint4 nameTableAddress;
    uint8 colorTableAddress;
    uint3 patternTableAddress;
    uint7 spriteAttributeTableAddress;
    uint3 spritePatternTableAddress;
    uint4 colorBackground;
    uint4 colorForeground;
  } io;

  struct Sprite {
    uint8 x;
    uint8 y;
    uint8 pattern;
    uint4 color;
  } sprites[4];

  struct Output {
    uint4 color;
  } output;
};

extern VDP vdp;
