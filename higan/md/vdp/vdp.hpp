//Yamaha YM7101

struct VDP : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto power() -> void;
  auto reset() -> void;

  //io.cpp
  auto read(uint24 addr) -> uint16;
  auto write(uint24 addr, uint16 data) -> void;

  auto readDataPort() -> uint16;
  auto writeDataPort(uint16 data) -> void;

  auto readControlPort() -> uint16;
  auto writeControlPort(uint16 data) -> void;

  //dma.cpp
  auto dmaRun() -> void;
  auto dmaLoad() -> void;
  auto dmaFill() -> void;
  auto dmaCopy() -> void;

  //render.cpp
  auto scanline() -> void;
  auto run() -> void;
  auto outputPixel(uint9 color) -> void;

  //background.cpp
  struct Background {
    enum class ID : uint { PlaneA, Window, PlaneB } id;

    auto isWindowed(uint x, uint y) -> bool;

    auto updateHorizontalScroll(uint y) -> void;
    auto updateVerticalScroll(uint x, uint y) -> void;

    auto nametableAddress() -> uint15;
    auto nametableWidth() -> uint;
    auto nametableHeight() -> uint;

    auto scanline(uint y) -> void;
    auto run(uint x, uint y) -> void;

    auto power() -> void;
    auto reset() -> void;

    struct IO {
      uint15 nametableAddress;

      //PlaneA, PlaneB
      uint2  nametableWidth;
      uint2  nametableHeight;
      uint15 horizontalScrollAddress;
      uint2  horizontalScrollMode;
      uint1  verticalScrollMode;

      //Window
      uint1  horizontalDirection;
      uint10 horizontalOffset;
      uint1  verticalDirection;
      uint10 verticalOffset;
    } io;

    struct State {
      uint10 horizontalScroll;
      uint10 verticalScroll;
    } state;

    struct Output {
      uint6 color;
      boolean priority;
    } output;
  };
  Background planeA{Background::ID::PlaneA};
  Background window{Background::ID::Window};
  Background planeB{Background::ID::PlaneB};

  //sprite.cpp
  struct Sprite {
    auto write(uint9 addr, uint16 data) -> void;
    auto scanline(uint y) -> void;
    auto run(uint x, uint y) -> void;

    auto power() -> void;
    auto reset() -> void;

    struct IO {
      uint15 attributeAddress;
      uint1  nametableAddressBase;
    } io;

    struct Object {
      uint9  x;
      uint9  y;
      uint   width;
      uint   height;
      bool   horizontalFlip;
      bool   verticalFlip;
      uint2  palette;
      uint1  priority;
      uint15 address;
      uint7  link;
    };

    struct Output {
      uint6   color;
      boolean priority;
    } output;

    array<Object, 80> oam;
    array<Object, 20> objects;
  };
  Sprite sprite;

private:
  auto screenWidth() const -> uint { return io.tileWidth ? 320 : 256; }
  auto screenHeight() const -> uint { return io.overscan ? 240 : 224; }

  uint16 vram[32768];
  uint16 vramExpansion[32768];  //not present in stock Mega Drive hardware
  uint9 cram[64];
  uint10 vsram[40];

  struct IO {
    //internal state
    boolean dmaFillWait;
    uint8 dmaFillByte;

    //command
    uint6 command;
    uint16 address;
    boolean commandPending;

    //$00  mode register 1
    uint1 displayOverlayEnable;
    uint1 counterLatch;
    uint1 horizontalBlankInterruptEnable;
    uint1 leftColumnBlank;

    //$01  mode register 2
    uint1 videoMode;  //0 = Master System; 1 = Mega Drive
    uint1 overscan;   //0 = 224 lines; 1 = 240 lines
    uint1 dmaEnable;
    uint1 verticalBlankInterruptEnable;
    uint1 displayEnable;
    uint1 externalVRAM;

    //$07  background color
    uint6 backgroundColor;

    //$0a  horizontal interrupt counter
    uint8 horizontalInterruptCounter;

    //$0b  mode register 3
    uint1 externalInterruptEnable;

    //$0c  mode register 4
    uint2 tileWidth;
    uint2 interlaceMode;
    uint1 shadowHighlightEnable;
    uint1 externalColorEnable;
    uint1 horizontalSync;
    uint1 verticalSync;

    //$0e  nametable pattern base address
    uint1 nametableBasePatternA;
    uint1 nametableBasePatternB;

    //$0f  data port auto-increment value
    uint8 dataIncrement;

    //$13-$14  DMA length
    uint16 dmaLength;

    //$15-$17  DMA source
    uint22 dmaSource;
    uint2 dmaMode;
  } io;

  struct State {
    uint32* output = nullptr;
    uint x;
    uint y;
  } state;

  uint32 buffer[1280 * 480];
};

extern VDP vdp;
