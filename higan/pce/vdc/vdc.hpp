//Hudson Soft HuC6270 -- Video Display Controller

struct VDC : Thread {
  inline auto bus() const -> uint9 { return data; }
  inline auto irqLine() const -> bool { return irq.line; }

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto scanline() -> void;
  auto frame() -> void;

  auto power() -> void;

  //io.cpp
  auto read(uint2 addr) -> uint8;
  auto write(uint2 addr, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  uint9 data;

  struct VRAM {
    //memory.cpp
    auto read(uint16 addr) -> uint16;
    auto write(uint16 addr, uint16 data) -> void;

    uint16 data[0x8000];

    uint16 addressRead;
    uint16 addressWrite;
    uint16 addressIncrement;

    uint16 dataRead;
    uint16 dataWrite;
  } vram;

  struct SATB {
    //memory.cpp
    auto read(uint8 addr) -> uint16;
    auto write(uint8 addr, uint16 data) -> void;

    uint16 data[0x100];
  } satb;

  struct Timing {
    uint5 horizontalSyncWidth;
    uint7 horizontalDisplayStart;
    uint7 horizontalDisplayLength;
    uint7 horizontalDisplayEnd;

    uint5 verticalSyncWidth;
    uint8 verticalDisplayStart;
    uint9 verticalDisplayLength;
    uint8 verticalDisplayEnd;

    bool  vpulse = 0;
    bool  hpulse = 0;

    uint  hclock = 0;
    uint  vclock = 0;

    uint  hoffset = 0;
    uint  voffset = 0;

    uint  hstart = 0;
    uint  vstart = 0;

    uint  hlength = 0;
    uint  vlength = 0;
  } timing;

  struct IRQ {
    enum class Line : uint {
      Collision,
      Overflow,
      LineCoincidence,
      Vblank,
      TransferVRAM,
      TransferSATB,
    };

    //irq.cpp
    auto poll() -> void;
    auto raise(Line) -> void;
    auto lower() -> void;

    bool enableCollision = 0;
    bool enableOverflow = 0;
    bool enableLineCoincidence = 0;
    bool enableVblank = 0;
    bool enableTransferVRAM = 0;
    bool enableTransferSATB = 0;

    bool pendingCollision = 0;
    bool pendingOverflow = 0;
    bool pendingLineCoincidence = 0;
    bool pendingVblank = 0;
    bool pendingTransferVRAM = 0;
    bool pendingTransferSATB = 0;

    bool line = 0;
  } irq;

  struct DMA {
    VDC* vdc = nullptr;

    //dma.cpp
    auto step(uint clocks) -> void;
    auto vramStart() -> void;
    auto satbStart() -> void;
    auto satbQueue() -> void;

    bool   sourceIncrementMode = 0;
    bool   targetIncrementMode = 0;
    bool   satbRepeat = 0;
    uint16 source;
    uint16 target;
    uint16 length;
    uint16 satbSource;

    bool   vramActive = 0;
    bool   satbActive = 0;
    bool   satbPending = 0;
    uint16 satbOffset;
  } dma;

  struct Background {
    VDC* vdc = nullptr;

    //background.cpp
    auto scanline(uint y) -> void;
    auto run(uint x, uint y) -> void;

    bool   enable = 0;
    uint10 hscroll;
    uint9  vscroll;
    uint9  vcounter;
    uint8  width;
    uint8  height;

    uint10 hoffset;
    uint9  voffset;

    uint4 color;
    uint4 palette;
  } background;

  struct Sprite {
    VDC* vdc = nullptr;

    //sprite.cpp
    auto scanline(uint y) -> void;
    auto run(uint x, uint y) -> void;

    bool enable = 0;

    struct Object {
      uint10 y;
      uint10 x;
      bool   mode = 0;
      uint10 pattern;
      uint4  palette;
      bool   priority = 0;
      uint   width = 0;
      bool   hflip = 0;
      uint   height = 0;
      bool   vflip = 0;
      bool   first = 0;
    };
    array<Object, 64> objects;

    uint4 color;
    uint4 palette;
    bool priority = 0;
  } sprite;

  struct IO {
    uint5  address;

    //$0005  CR (W)
    uint2  externalSync;
    uint2  displayOutput;
    bool   dramRefresh = 0;

    //$0006  RCR
    uint10 lineCoincidence;

    //$0009  MWR
    uint2  vramAccess;
    uint2  spriteAccess;
    bool   cgMode = 0;
  } io;
};

extern VDC vdc0;
extern VDC vdc1;
