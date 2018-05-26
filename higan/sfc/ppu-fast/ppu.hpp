struct PPU : Thread, PPUcounter {
  alwaysinline auto interlace() const -> bool { return false; }
  alwaysinline auto overscan() const -> bool { return false; }
  alwaysinline auto vdisp() const -> uint { return 225; }

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

  auto latchCounters() -> void {}

  //serialization.cpp
  auto serialize(serializer&) -> void;

public:
  uint32* output = nullptr;
  uint16 vram[32 * 1024];
  uint16 cgram[256];

  struct {
    uint4 version;
    uint8 mdr;
  } ppu1, ppu2;

  struct Latch {
    uint8 mode7;
  } latch;

  enum : uint {
    INIDISP = 0x00, OBSEL = 0x01, OAMADDL = 0x02, OAMADDH = 0x03,
    OAMDATA = 0x04, BGMODE = 0x05, MOSAIC = 0x06, BG1SC = 0x07,
    BG2SC = 0x08, BG3SC = 0x09, BG4SC = 0x0a, BG12NBA = 0x0b,
    BG34NBA = 0x0c, BG1HOFS = 0x0d, BG1VOFS = 0x0e, BG2HOFS = 0x0f,
    BG2VOFS = 0x10, BG3HOFS = 0x11, BG3VOFS = 0x12, BG4HOFS = 0x13,
    BG4VOFS = 0x14, VMAIN = 0x15, VMADDL = 0x16, VMADDH = 0x17,
    VMDATAL = 0x18, VMDATAH = 0x19, M7SEL = 0x1a, M7A = 0x1b,
    M7B = 0x1c, M7C = 0x1d, M7D = 0x1e, M7X = 0x1f,
    M7Y = 0x20, CGADD = 0x21, CGDATA = 0x22, W12SEL = 0x23,
    W34SEL = 0x24, WOBJSEL = 0x25, WH0 = 0x26, WH1 = 0x27,
    WH2 = 0x28, WH3 = 0x29, WBGLOG = 0x2a, WOBJLOG = 0x2b,
    TM = 0x2c, TS = 0x2d, TMW = 0x2e, TSW = 0x2f,
    CGWSEL = 0x30, CGADDSUB = 0x31, COLDATA = 0x32, SETINI = 0x33,
    MPYL = 0x34, MPYM = 0x35, MPYH = 0x36, SLHV = 0x37,
    OAMDATAREAD = 0x38, VMDATALREAD = 0x39, VMDATAHREAD = 0x3a, CGDATAREAD = 0x3b,
    OPHCT = 0x3c, OPVCT = 0x3d, STAT77 = 0x3e, STAT78 = 0x3f,
  };

  //io.cpp
  auto readIO(uint24 address, uint8 data) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;

  struct IO {
    uint16 m7a;
    uint16 m7b;
    uint16 m7c;
    uint16 m7d;
    uint16 m7x;
    uint16 m7y;
  } io;

  //object.cpp
  auto readOAM(uint10 address) -> uint8;
  auto writeOAM(uint10 address, uint8 data) -> void;

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

  //bitplane -> bitmap tile caches
  uint8 vram2bpp[4096 * 64];
  uint8 vram4bpp[2048 * 64];
  uint8 vram8bpp[1024 * 64];

  struct Line {
    //line.cpp
    auto render() -> void;

    uint y = 0;
    uint32* outputLo = nullptr;
    uint32* outputHi = nullptr;

    uint15 cgram[256];
    IO io;
  } lines[240];
};

extern PPU ppu;
