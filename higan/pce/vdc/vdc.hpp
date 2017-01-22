//Hudson Soft HuC6260 -- Video Color Encoder
//Hudson Soft HuC6270 -- Video Display Controller

struct VDC : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  auto power() -> void;

  //io.cpp
  auto read(uint11 addr) -> uint8;
  auto write(uint11 addr, uint8 data) -> void;

private:
  uint32 buffer[1140 * 512];

  struct VRAM {
    //memory.cpp
    auto read(uint16 addr) -> uint16;
    auto write(uint16 addr, uint16 data) -> void;

    uint16 addressRead;
    uint16 addressWrite;
    uint16 addressIncrement;

    uint16 dataRead;
    uint16 dataWrite;

  private:
    uint16 data[0x8000];
  } vram;

  struct SATB {
    //memory.cpp
    auto read(uint8 addr) -> uint16;
    auto write(uint8 addr, uint16 data) -> void;

  private:
    uint16 data[0x100];
  } satb;

  struct CRAM {
    //memory.cpp
    auto read(uint9 addr) -> uint9;
    auto write(uint9 addr, bool a0, uint8 data) -> void;

    uint9 address;

  private:
    uint9 data[0x200];
  } cram;

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
  } irq;

  struct DMA {
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

  struct VCE {
    uint5 horizontalSyncWidth;
    uint7 horizontalDisplayStart;
    uint7 horizontalDisplayLength;
    uint7 horizontalDisplayEnd;

    uint5 verticalSyncWidth;
    uint8 verticalDisplayStart;
    uint9 verticalDisplayLength;
    uint8 verticalDisplayEnd;

    uint  clock;

    uint  hclock;
    uint  vclock;

    uint  hoffset;
    uint  voffset;

    uint  hstart;
    uint  vstart;

    uint  hlength;
    uint  vlength;
  } vce;

  struct Background {
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

    maybe<uint9> color;
  } background;

  struct Sprite {
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

    maybe<uint9> color;
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

    //$0400  CR
    bool   colorBlur;
    bool   grayscale;
  } io;
};

extern VDC vdc;
