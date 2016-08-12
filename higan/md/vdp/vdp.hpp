//Yamaha YM7101

struct VDP : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;

  auto power() -> void;
  auto reset() -> void;

  auto readByte(uint24 addr) -> uint8;
  auto readWord(uint24 addr) -> uint16;
  auto writeByte(uint24 addr, uint8 data) -> void;
  auto writeWord(uint24 addr, uint16 data) -> void;

  auto readControlPort() -> uint16;
  auto writeControlPort(uint7 addr, uint8 data) -> void;

private:
  struct IO {
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

    //$02  plane A name table location
    uint4 nametablePlaneA;

    //$03  window name table location
    uint6 nametableWindow;

    //$04  plane B name table location
    uint4 nametablePlaneB;

    //$05  sprite attribute table location
    uint8 attrtableSprite;

    //$06  sprite pattern base address
    uint1 nametableBaseSprite;

    //$07  background color
    uint4 backgroundIndex;
    uint2 backgroundPalette;

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

    //$0f  VRAM auto-increment value
    uint8 vramAutoIncrement;

    //$10  plane size
    uint2 horizontalPlaneSize;
    uint2 verticalPlaneSize;

    //$11  window plane horizontal position
    uint5 horizontalWindowPlanePosition;
    uint1 horizontalWindowPlaneRight;

    //$12  window plane vertical position
    uint5 verticalWindowPlanePosition;
    uint1 verticalWindowPlaneDown;

    //$13-$14  DMA length
    uint16 dmaLength;

    //$15-$17  DMA source
    uint22 dmaSource;
    uint2 dmaMode;
  } io;

  uint32 buffer[1280 * 480];
};

extern VDP vdp;
