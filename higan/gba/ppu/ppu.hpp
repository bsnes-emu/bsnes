struct PPU : Thread, IO {
  PPU();
  ~PPU();

  inline auto blank() -> bool;

  static auto Enter() -> void;
  auto step(uint clocks) -> void;
  auto main() -> void;

  auto frame() -> void;
  auto refresh() -> void;
  auto power() -> void;

  auto readIO(uint32 addr) -> uint8;
  auto writeIO(uint32 addr, uint8 byte) -> void;

  auto readVRAM(uint mode, uint32 addr) -> uint32;
  auto writeVRAM(uint mode, uint32 addr, uint32 word) -> void;

  auto readPRAM(uint mode, uint32 addr) -> uint32;
  auto writePRAM(uint mode, uint32 addr, uint32 word) -> void;

  auto readOAM(uint mode, uint32 addr) -> uint32;
  auto writeOAM(uint mode, uint32 addr, uint32 word) -> void;

  auto readObjectVRAM(uint addr) const -> uint8;

  auto serialize(serializer&) -> void;

  uint8 vram[96 * 1024];
  uint16 pram[512];
  uint32* output;

private:
  //note: I/O register order is {BG0-BG3, OBJ, SFX}
  //however; layer ordering is {OBJ, BG0-BG3, SFX}
  enum : uint { OBJ = 0, BG0 = 1, BG1 = 2, BG2 = 3, BG3 = 4, SFX = 5 };
  enum : uint { IN0 = 0, IN1 = 1, IN2 = 2, OUT = 3 };

  struct IO {
    uint1 gameBoyColorMode;
    uint1 forceBlank;
    uint1 greenSwap;

    uint1 vblank;
    uint1 hblank;
    uint1 vcoincidence;
    uint1 irqvblank;
    uint1 irqhblank;
    uint1 irqvcoincidence;
    uint8 vcompare;

    uint16 vcounter;
  } io;

  struct Pixel {
    uint1  enable;
    uint2  priority;
    uint15 color;

    //OBJ only
    uint1  translucent;
    uint1  mosaic;
    uint1  window;  //IN2
  };

  struct Background {
    auto scanline(uint y) -> void;
    auto run(uint x, uint y) -> void;
    auto linear(uint x, uint y) -> void;
    auto affine(uint x, uint y) -> void;
    auto bitmap(uint x, uint y) -> void;

    auto power(uint id) -> void;
    auto serialize(serializer&) -> void;

    uint id;  //BG0, BG1, BG2, BG3

    struct IO {
      static uint3 mode;
      static uint1 frame;
      static uint5 mosaicWidth;
      static uint5 mosaicHeight;

      uint1 enable;

      uint2 priority;
      uint2 characterBase;
      uint2 unused;
      uint1 mosaic;
      uint1 colorMode;
      uint5 screenBase;
      uint1 affineWrap;  //BG2, BG3 only
      uint2 screenSize;

      uint9 hoffset;
      uint9 voffset;

      //BG2, BG3 only
      int16 pa;
      int16 pb;
      int16 pc;
      int16 pd;
      int28 x;
      int28 y;

      //internal
      int28 lx;
      int28 ly;
    } io;

    struct Latch {
      uint10 character;
      uint1  hflip;
      uint1  vflip;
      uint4  palette;
    } latch;

    Pixel output;
    Pixel mosaic;
    uint mosaicOffset;

    uint hmosaic;
    uint vmosaic;

    int28 fx;
    int28 fy;
  } bg0, bg1, bg2, bg3;

  struct Objects {
    auto scanline(uint y) -> void;
    auto run(uint x, uint y) -> void;

    auto power() -> void;
    auto serialize(serializer&) -> void;

    struct IO {
      uint1 enable;

      uint1 hblank;   //1 = allow access to OAM during Hblank
      uint1 mapping;  //0 = two-dimensional, 1 = one-dimensional
      uint5 mosaicWidth;
      uint5 mosaicHeight;
    } io;

    Pixel buffer[240];
    Pixel output;
    Pixel mosaic;
    uint mosaicOffset;
  } objects;

  struct Window {
    auto run(uint x, uint y) -> void;

    auto power(uint id) -> void;
    auto serialize(serializer&) -> void;

    uint id;  //IN0, IN1, IN2, OUT

    struct IO {
      uint1 enable;
      uint1 active[6];

      //IN0, IN1 only
      uint8 x1;
      uint8 x2;
      uint8 y1;
      uint8 y2;
    } io;

    uint1 output;  //IN0, IN1, IN2 only
  } window0, window1, window2, window3;

  struct Screen {
    auto run(uint x, uint y) -> uint15;
    auto blend(uint15 above, uint eva, uint15 below, uint evb) -> uint15;

    auto power() -> void;
    auto serialize(serializer&) -> void;

    struct IO {
      uint2 blendMode;
      uint1 blendAbove[6];
      uint1 blendBelow[6];

      uint5 blendEVA;
      uint5 blendEVB;
      uint5 blendEVY;
    } io;
  } screen;

  struct Object {
    auto serialize(serializer&) -> void;

    uint8  y;
    uint1  affine;
    uint1  affineSize;
    uint2  mode;
    uint1  mosaic;
    uint1  colors;  //0 = 16, 1 = 256
    uint2  shape;   //0 = square, 1 = horizontal, 2 = vertical

    uint9  x;
    uint5  affineParam;
    uint1  hflip;
    uint1  vflip;
    uint2  size;

    uint10 character;
    uint2  priority;
    uint4  palette;

    //ancillary data
    uint width;
    uint height;
  } object[128];

  struct ObjectParam {
    auto serialize(serializer&) -> void;

    int16 pa;
    int16 pb;
    int16 pc;
    int16 pd;
  } objectParam[32];
};

extern PPU ppu;
