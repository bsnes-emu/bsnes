//performance-focused, scanline-based, parallelized implementation of PPU

//limitations:
//* mid-scanline effects not support
//* vertical mosaic coordinates are not exact
//* (hardware-mod) 128KB VRAM mode not supported

struct PPUfast : Thread, PPUcounter {
  alwaysinline auto interlace() const -> bool;
  alwaysinline auto overscan() const -> bool;
  alwaysinline auto vdisp() const -> uint;
  alwaysinline auto hires() const -> bool;
  alwaysinline auto hd() const -> bool;
  alwaysinline auto ss() const -> bool;
  alwaysinline auto hdScale() const -> uint;
  alwaysinline auto hdPerspective() const -> bool;
  alwaysinline auto hdSupersample() const -> bool;
  alwaysinline auto hdMosaic() const -> bool;

  //ppu.cpp
  PPUfast();
  ~PPUfast();

  static auto Enter() -> void;
  alwaysinline auto step(uint clocks) -> void;
  auto main() -> void;
  auto scanline() -> void;
  auto refresh() -> void;
  auto load() -> bool;
  auto power(bool reset) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

public:
  struct Source { enum : uint { BG1, BG2, BG3, BG4, OBJ1, OBJ2, COL }; };
  struct TileMode { enum : uint { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenMode { enum : uint { Above, Below }; };

  struct Latch {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    bool interlace = 0;
    bool overscan = 0;
    bool hires = 0;
    bool hd = 0;
    bool ss = 0;

    uint16_t vram = 0;
    uint8_t oam = 0;
    uint8_t cgram = 0;

    uint10 oamAddress = 0;
    uint8_t cgramAddress = 0;

    uint8_t mode7 = 0;
    bool counters = 0;
    bool hcounter = 0;  //hdot
    bool vcounter = 0;

    struct PPU {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      uint8 mdr;
      uint8_t bgofs = 0;
    } ppu1, ppu2;
  };

  struct IO {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    bool displayDisable = 1;
    uint4 displayBrightness;
    uint10 oamBaseAddress;
    uint10 oamAddress;
    bool oamPriority = 0;
    bool bgPriority = 0;
    uint3 bgMode;
    uint4 mosaicSize;
    bool vramIncrementMode = 0;
    uint2 vramMapping;
    uint8_t vramIncrementSize = 0;
    uint16_t vramAddress = 0;
    uint8_t cgramAddress = 0;
    uint1 cgramAddressLatch;
    uint9 hcounter;  //hdot
    uint9 vcounter;
    bool interlace = 0;
    bool overscan = 0;
    bool pseudoHires = 0;
    bool extbg = 0;

    struct Mode7 {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      bool hflip = 0;
      bool vflip = 0;
      uint repeat = 0;
      uint16_t a = 0;
      uint16_t b = 0;
      uint16_t c = 0;
      uint16_t d = 0;
      uint16_t x = 0;
      uint16_t y = 0;
      uint16_t hoffset = 0;
      uint16_t voffset = 0;
    } mode7;

    struct Window {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      uint8_t oneLeft = 0;
      uint8_t oneRight = 0;
      uint8_t twoLeft = 0;
      uint8_t twoRight = 0;
    } window;

    struct WindowLayer {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      bool oneEnable = 0;
      bool oneInvert = 0;
      bool twoEnable = 0;
      bool twoInvert = 0;
      uint mask = 0;
      bool aboveEnable = 0;
      bool belowEnable = 0;
    };

    struct WindowColor {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      bool oneEnable = 0;
      bool oneInvert = 0;
      bool twoEnable = 0;
      bool twoInvert = 0;
      uint mask = 0;
      uint aboveMask = 0;
      uint belowMask = 0;
    };

    struct Background {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      WindowLayer window;
      bool aboveEnable = 0;
      bool belowEnable = 0;
      bool mosaicEnable = 0;
      uint15 tiledataAddress;
      uint15 screenAddress;
      uint2 screenSize;
      bool tileSize = 0;
      uint16 hoffset;
      uint16 voffset;
      uint3 tileMode;
      uint4 priority[2];
    } bg1, bg2, bg3, bg4;

    struct Object {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      WindowLayer window;
      bool aboveEnable = 0;
      bool belowEnable = 0;
      bool interlace = 0;
      uint3 baseSize;
      uint2 nameselect;
      uint15 tiledataAddress;
      uint7 first;
      bool rangeOver = 0;
      bool timeOver = 0;
      uint4 priority[4];
    } obj;

    struct Color {
      //serialization.cpp
      auto serialize(serializer&) -> void;

      WindowColor window;
      bool enable[7] = {};
      bool directColor = 0;
      bool blendMode = 0;  //0 = fixed; 1 = pixel
      bool halve = 0;
      bool mathMode = 0;   //0 = add; 1 = sub
      uint15 fixedColor;
    } col;
  };

  struct Object {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint9 x;
    uint8_t y;
    uint8 character;
    bool nameselect;
    bool vflip;
    bool hflip;
    uint2 priority;
    uint3 palette;
    bool size;
  };

  struct ObjectItem {
    bool valid;
    uint7 index;
    uint8_t width;
    uint8_t height;
  };

  struct ObjectTile {
    bool valid;
    uint9 x;
    uint8_t y;
    uint2 priority;
    uint8_t palette;
    bool hflip;
    uint11 number;
  };

  struct Pixel {
    uint source;
    uint priority;
    uint color;
  };

  //io.cpp
  auto latchCounters() -> void;
  alwaysinline auto vramAddress() const -> uint15;
  alwaysinline auto readVRAM() -> uint16;
  template<bool Byte> alwaysinline auto writeVRAM(uint8_t data) -> void;
  alwaysinline auto updateTiledata(uint address) -> void;
  alwaysinline auto readOAM(uint10 address) -> uint8;
  alwaysinline auto writeOAM(uint10 address, uint8 data) -> void;
  template<bool Byte> alwaysinline auto readCGRAM(uint8_t address) -> uint8;
  alwaysinline auto writeCGRAM(uint8 address, uint15 data) -> void;
  auto readIO(uint24 address, uint8 data) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;
  auto updateVideoMode() -> void;

  //object.cpp
  auto oamAddressReset() -> void;
  auto oamSetFirstObject() -> void;
  auto readObject(uint10 address) -> uint8;
  auto writeObject(uint10 address, uint8 data) -> void;

  //[serialized]
  Latch latch;
  IO io;

  uint16_t vram[32 * 1024];
  uint16_t cgram[256];
  Object   objects[128];

  //[unserialized]
  uint16_t output[2304 * 2160];
  uint16_t lightTable[16][32768];
  uint8_t* tilecache[3];  //bitplane -> bitmap tiledata
  uint ItemLimit;
  uint TileLimit;

  struct Line {
    //line.cpp
    static auto flush() -> void;
    auto render() -> void;
    auto pixel(uint x, Pixel above, Pixel below) const -> uint16_t;
    auto blend(uint x, uint y, bool halve) const -> uint16_t;
    alwaysinline auto directColor(uint paletteIndex, uint paletteColor) const -> uint16_t;
    alwaysinline auto plotAbove(uint x, uint source, uint priority, uint color) -> void;
    alwaysinline auto plotBelow(uint x, uint source, uint priority, uint color) -> void;
    alwaysinline auto plotHD(Pixel*, uint x, uint source, uint priority, uint color, bool hires, bool subpixel) -> void;

    //background.cpp
    auto renderBackground(PPUfast::IO::Background&, uint source) -> void;
    auto getTile(PPUfast::IO::Background&, uint hoffset, uint voffset) -> uint;

    //mode7.cpp
    auto renderMode7(PPUfast::IO::Background&, uint source) -> void;

    //mode7hd.cpp
    auto renderMode7HD(PPUfast::IO::Background&, uint source) -> void;
    alwaysinline auto lerp(float pa, float va, float pb, float vb, float pr) -> float;

    //object.cpp
    auto renderObject(PPUfast::IO::Object&) -> void;

    //window.cpp
    auto renderWindow(PPUfast::IO::WindowLayer&, bool, array<bool[256]>&) -> void;
    auto renderWindow(PPUfast::IO::WindowColor&, uint, array<bool[256]>&) -> void;

    //[unserialized]
    uint9 y;  //constant

    IO io;
    array<uint16_t[256]> cgram;

    array<ObjectItem[128]> items;  //32 on real hardware
    array<ObjectTile[128]> tiles;  //34 on real hardware; 1024 max (128 * 64-width tiles)

    Pixel above[256 * 9 * 9];
    Pixel below[256 * 9 * 9];

    array<bool[256]> windowAbove;
    array<bool[256]> windowBelow;

    //flush()
    static uint start;
    static uint count;
  };
  array<Line[240]> lines;

  //used to help detect when the video output size changes between frames to clear overscan area.
  struct Frame {
    uint pitch;
    uint width;
    uint height;
  } frame;
};

extern PPUfast ppufast;
