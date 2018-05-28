//performance-focused, scanline-based, parallelized implementation of PPU

//limitations:
//* mid-scanline effects not support
//* mid-frame OAM changes not supported

struct PPU : Thread, PPUcounter {
  //as a scanline-based renderer, PPU::PPUcounter values are not cycle-accurate
  alwaysinline auto field() const -> bool { return cpu.field(); }
  alwaysinline auto vcounter() const -> uint16 { return cpu.vcounter(); }
  alwaysinline auto hcounter() const -> uint16 { return cpu.hcounter(); }
  alwaysinline auto hdot() const -> uint16 { return cpu.hdot(); }
  alwaysinline auto lineclocks() const -> uint16 { return cpu.lineclocks(); }

  alwaysinline auto interlace() const -> bool { return false; }
  alwaysinline auto overscan() const -> bool { return false; }
  alwaysinline auto vdisp() const -> uint { return !io.overscan ? 225 : 240; }

  //ppu.cpp
  PPU();
  ~PPU();

  static auto Enter() -> void;
  alwaysinline auto step(uint clocks) -> void;
  auto main() -> void;
  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;
  auto load(Markup::Node) -> bool;
  auto power(bool reset) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

public:
  uint32* output = nullptr;
  uint8* tilecache[3] = {};  //bitplane -> bitmap tiledata
  uint16 vram[32 * 1024];
  uint16 cgram[256];

  struct {
    uint4 version;
    uint8 mdr;
  } ppu1, ppu2;

  struct Latch {
    uint16 vram;
    uint8  oam;
    uint8  cgram;
    uint8  bgofsPPU1;
    uint8  bgofsPPU2;
    uint8  mode7;
    uint1  counters;
    uint1  hcounter;  //hdot
    uint1  vcounter;

    uint10 oamAddress;
    uint8  cgramAddress;
  } latch;

  //io.cpp
  auto latchCounters() -> void;
  alwaysinline auto vramAddress() const -> uint15;
  alwaysinline auto readVRAM() -> uint16;
  alwaysinline auto writeVRAM(uint1 byte, uint8 data) -> void;
  alwaysinline auto readOAM(uint10 address) -> uint8;
  alwaysinline auto writeOAM(uint10 address, uint8 data) -> void;
  alwaysinline auto readCGRAM(uint1 byte, uint8 address) -> uint8;
  alwaysinline auto writeCGRAM(uint8 address, uint15 data) -> void;
  auto readIO(uint24 address, uint8 data) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;
  auto updateVideoMode() -> void;

  struct Source { enum : uint { BG1, BG2, BG3, BG4, OBJ1, OBJ2, COL }; };
  struct TileMode { enum : uint { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenMode { enum : uint { Above, Below }; };

  struct IO {
    uint1  displayDisable;
    uint4  displayBrightness;
    uint10 oamBaseAddress;
    uint10 oamAddress;
    uint1  oamPriority;
    uint1  bgPriority;
    uint3  bgMode;
    uint4  mosaicSize;
    uint1  vramIncrementMode;
    uint2  vramMapping;
    uint8  vramIncrementSize;
    uint16 vramAddress;
    uint8  cgramAddress;
    uint1  cgramAddressLatch;
    uint9  hcounter;  //hdot
    uint9  vcounter;
    uint1  interlace;
    uint1  overscan;
    uint1  pseudoHires;
    uint1  extbg;

    struct WindowLayer {
      uint1 oneEnable;
      uint1 oneInvert;
      uint1 twoEnable;
      uint1 twoInvert;
      uint2 mask;
      uint1 aboveEnable;
      uint1 belowEnable;
    };

    struct WindowColor {
      uint1 oneEnable;
      uint1 oneInvert;
      uint1 twoEnable;
      uint1 twoInvert;
      uint2 mask;
      uint2 aboveMask;
      uint2 belowMask;
    };

    struct Window {
      uint8 oneLeft;
      uint8 oneRight;
      uint8 twoLeft;
      uint8 twoRight;
    } window;

    struct Mode7 {
      uint16 a;
      uint16 b;
      uint16 c;
      uint16 d;
      uint16 x;
      uint16 y;
      uint16 hoffset;
      uint16 voffset;
    } mode7;

    struct Background {
      WindowLayer window;
      uint1  aboveEnable;
      uint1  belowEnable;
      uint1  colorEnable;
      uint1  mosaicEnable;
      uint15 tiledataAddress;
      uint15 screenAddress;
      uint2  screenSize;
      uint1  tileSize;
      uint16 hoffset;
      uint16 voffset;
      uint3  tileMode;
      uint4  priority[2];
    } bg1, bg2, bg3, bg4;

    struct Object {
      WindowLayer window;
      uint1  aboveEnable;
      uint1  belowEnable;
      uint1  colorEnable;
      uint1  interlace;
      uint3  baseSize;
      uint2  nameselect;
      uint15 tiledataAddress;
      uint7  firstObject;
      uint1  rangeOver;
      uint1  timeOver;
      uint4  priority[4];
    } obj;

    struct Color {
      WindowColor window;
      uint1 colorEnable;
      uint1 directColor;
      uint1 blendMode;
      uint1 colorHalve;
      uint1 colorMode;
      uint5 colorRed;
      uint5 colorGreen;
      uint5 colorBlue;
    } col;
  } io;

  //object.cpp
  auto oamAddressReset() -> void;
  auto oamSetFirstObject() -> void;
  auto readObject(uint10 address) -> uint8;
  auto writeObject(uint10 address, uint8 data) -> void;

  struct Object {
    uint9 x;
    uint8 y;
    uint8 character;
    uint1 nameselect;
    uint1 vflip;
    uint1 hflip;
    uint2 priority;
    uint3 palette;
    uint1 size;
  } object[128];

  struct Line {
    //line.cpp
    auto render() -> void;
    alwaysinline auto plotAbove(uint x, uint source, uint priority, uint color) -> void;
    alwaysinline auto plotBelow(uint x, uint source, uint priority, uint color) -> void;

    //background.cpp
    auto renderBackground(PPU::IO::Background&, uint source) -> void;

    //object.cpp
    auto renderObject(PPU::IO::Object&) -> void;

    //window.cpp
    auto renderWindow(PPU::IO::WindowLayer&) -> void;
    auto renderWindow(PPU::IO::WindowColor&) -> void;

    uint9 y;
    uint32* outputLo = nullptr;
    uint32* outputHi = nullptr;

    uint15 cgram[256];
    IO io;

    struct Screen {
      uint source;
      uint priority;
      uint color;
    } above[256], below[256];
  } lines[240];
};

extern PPU ppu;
