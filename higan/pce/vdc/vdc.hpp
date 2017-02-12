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

    bool  vpulse;
    bool  hpulse;

    uint  hclock;
    uint  vclock;

    uint  hoffset;
    uint  voffset;

    uint  hstart;
    uint  vstart;

    uint  hlength;
    uint  vlength;
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

    bool enableCollision;
    bool enableOverflow;
    bool enableLineCoincidence;
    bool enableVblank;
    bool enableTransferVRAM;
    bool enableTransferSATB;

    bool pendingCollision;
    bool pendingOverflow;
    bool pendingLineCoincidence;
    bool pendingVblank;
    bool pendingTransferVRAM;
    bool pendingTransferSATB;

    bool line;
  } irq;

  struct DMA {
    VDC* vdc = nullptr;

    //dma.cpp
    auto step(uint clocks) -> void;
    auto vramStart() -> void;
    auto satbStart() -> void;
    auto satbQueue() -> void;

    bool   sourceIncrementMode;
    bool   targetIncrementMode;
    bool   satbRepeat;
    uint16 source;
    uint16 target;
    uint16 length;
    uint16 satbSource;

    bool   vramActive;
    bool   satbActive;
    bool   satbPending;
    uint16 satbOffset;
  } dma;

  struct Background {
    VDC* vdc = nullptr;

    //background.cpp
    auto scanline(uint y) -> void;
    auto run(uint x, uint y) -> void;

    bool   enable;
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

    bool enable;

    struct Object {
      uint10 y;
      uint10 x;
      bool   mode;
      uint10 pattern;
      uint4  palette;
      bool   priority;
      uint   width;
      bool   hflip;
      uint   height;
      bool   vflip;
      bool   first;
    };
    array<Object, 64> objects;

    uint4 color;
    uint4 palette;
    bool priority;
  } sprite;

  struct IO {
    uint5  address;

    //$0005  CR (W)
    uint2  externalSync;
    uint2  displayOutput;
    bool   dramRefresh;

    //$0006  RCR
    uint10 lineCoincidence;

    //$0009  MWR
    uint2  vramAccess;
    uint2  spriteAccess;
    bool   cgMode;
  } io;
};

extern VDC vdc0;
extern VDC vdc1;
