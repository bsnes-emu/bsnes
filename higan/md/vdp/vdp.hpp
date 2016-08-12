//Yamaha YM7101

struct VDP : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto power() -> void;
  auto reset() -> void;

  //io.cpp
  auto readByte(uint24 addr) -> uint8;
  auto writeByte(uint24 addr, uint8 data) -> void;

  auto readWord(uint24 addr) -> uint16;
  auto writeWord(uint24 addr, uint16 data) -> void;

  auto readDataPort() -> uint16;
  auto writeDataPort(uint16 data) -> void;

  auto readControlPort() -> uint16;
  auto writeControlPort(uint16 data) -> void;

  //dma.cpp
  auto dmaRun() -> void;
  auto dmaFillVRAM() -> void;

  //render.cpp
  auto scanline() -> void;
  auto run() -> void;
  auto outputPixel(uint9 color) -> void;

  //background.cpp
  struct Background {
    auto scanline() -> void;
    auto run(uint x, uint y) -> void;

    auto power() -> void;
    auto reset() -> void;

    struct IO {
      uint15 nametableAddress;
      uint3 nametableWidth;   //1 << value
      uint3 nametableHeight;  //1 << value
    } io;

    struct Output {
      uint6 color;
      boolean priority;
    } output;
  };
  Background planeA;
  Background window;
  Background planeB;

  uint16 vram[32768];
  uint16 vramExpansion[32768];  //not present in stock Mega Drive hardware
  uint9 cram[64];
  uint10 vsram[40];

private:
  struct IO {
    //internal state
    boolean dmaActive;
    uint8 dmaFillWord;

    //command
    uint6 command;
    uint16 address;
    boolean commandPending;

    //$00  mode register 1
    uint1 displayOverlayEnable;
    uint1 counterLatch;
    uint1 horizontalInterruptEnable;
    uint1 leftColumnBlank;

    //$01  mode register 2
    uint1 videoMode;  //0 = Master System; 1 = Mega Drive
    uint1 overscan;   //0 = 224 lines; 1 = 240 lines
    uint1 dmaEnable;
    uint1 verticalBlankInterruptEnable;
    uint1 displayEnable;
    uint1 externalVRAM;

    //$05  sprite attribute table location
    uint8 attrtableSprite;

    //$06  sprite pattern base address
    uint1 nametableBaseSprite;

    //$07  background color
    uint6 backgroundColor;

    //$0a  horizontal interrupt counter
    uint8 horizontalInterruptCounter;

    //$0b  mode register 3
    uint2 horizontalScrollMode;
    uint1 verticalScrollMode;
    uint1 externalInterruptEnable;

    //$0c  mode register 4
    uint2 tileWidth;
    uint2 interlaceMode;
    uint1 shadowHighlightEnable;
    uint1 externalColorEnable;
    uint1 horizontalSync;
    uint1 verticalSync;

    //$0d  horizontal scroll data location
    uint7 horizontalScrollTable;

    //$0e  nametable pattern base address
    uint1 nametableBasePatternA;
    uint1 nametableBasePatternB;

    //$0f  data port auto-increment value
    uint8 dataIncrement;

    //$11  window plane horizontal position
    uint10 windowHorizontalLo;
    uint10 windowHorizontalHi;

    //$12  window plane vertical position
    uint10 windowVerticalLo;
    uint10 windowVerticalHi;

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
