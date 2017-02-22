//TI TMS9918A (derivative)

struct VDP : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto vlines() -> uint;
  auto vblank() -> bool;

  auto power() -> void;

  //io.cpp
  auto vcounter() -> uint8;
  auto hcounter() -> uint8;
  auto data() -> uint8;
  auto status() -> uint8;

  auto data(uint8) -> void;
  auto control(uint8) -> void;
  auto registerWrite(uint4 addr, uint8 data) -> void;

  //background.cpp
  struct Background {
    auto scanline() -> void;
    auto run() -> void;

    auto power() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    struct State {
      uint x;
      uint y;
    } state;

    struct Output {
      uint4 color;
      uint1 palette;
      uint1 priority;
    } output;
  } background;

  //sprite.cpp
  struct Sprite {
    auto scanline() -> void;
    auto run() -> void;

    auto power() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    struct Object {
      uint8 x;
      uint8 y;
      uint8 pattern;
    };

    struct State {
      uint x;
      uint y;
    } state;

    struct Output {
      uint4 color;
    } output;

    array<Object, 8> objects;
  } sprite;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  auto palette(uint5 index) -> uint12;

  uint32 buffer[256 * 264];
  uint8 vram[0x4000];
  uint8 cram[0x40];  //MS = 0x20, GG = 0x40

  struct IO {
    uint vcounter;  //vertical counter
    uint hcounter;  //horizontal counter
    uint lcounter;  //line counter

    //interrupt flags
    bool intLine;
    bool intFrame;

    //status flags
    bool spriteOverflow;
    bool spriteCollision;
    uint5 fifthSprite;

    //latches
    bool controlLatch;
    uint16 controlData;
    uint2 code;
    uint14 address;

    uint8 vramLatch;

    //$00  mode control 1
    bool externalSync;
    bool extendedHeight;
    bool mode4;
    bool spriteShift;
    bool lineInterrupts;
    bool leftClip;
    bool horizontalScrollLock;
    bool verticalScrollLock;

    //$01  mode control 2
    bool spriteDouble;
    bool spriteTile;
    bool lines240;
    bool lines224;
    bool frameInterrupts;
    bool displayEnable;

    //$02  name table base address
    uint1 nameTableMask;
    uint3 nameTableAddress;

    //$03  color table base address
    uint8 colorTableAddress;

    //$04  pattern table base address
    uint8 patternTableAddress;

    //$05  sprite attribute table base address
    uint1 spriteAttributeTableMask;
    uint6 spriteAttributeTableAddress;

    //$06  sprite pattern table base address
    uint2 spritePatternTableMask;
    uint1 spritePatternTableAddress;

    //$07  backdrop color
    uint4 backdropColor;

    //$08  horizontal scroll offset
    uint8 hscroll;

    //$09  vertical scroll offset
    uint8 vscroll;

    //$0a  line counter
    uint8 lineCounter;
  } io;
};

extern VDP vdp;
