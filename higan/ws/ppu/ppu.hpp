#include "video.hpp"

struct PPU : Thread, IO {
  static auto Enter() -> void;
  auto main() -> void;
  auto scanline() -> void;
  auto frame() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  //latch.cpp
  auto latchRegisters() -> void;
  auto latchSprites() -> void;
  auto latchOAM() -> void;

  //render-mono.cpp
  auto renderMonoFetch(uint14 offset, uint3 y, uint3 x) -> uint2;
  auto renderMonoPalette(uint4 palette, uint2 index) -> uint12;
  auto renderMonoBack() -> void;
  auto renderMonoScreenOne() -> void;
  auto renderMonoScreenTwo() -> void;
  auto renderMonoSprite() -> void;

  //render-color.cpp
  auto renderColorFetch(uint16 offset, uint3 y, uint3 x) -> uint4;
  auto renderColorPalette(uint4 palette, uint4 index) -> uint12;
  auto renderColorBack() -> void;
  auto renderColorScreenOne() -> void;
  auto renderColorScreenTwo() -> void;
  auto renderColorSprite() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //state
  struct Pixel {
    enum class Source : uint { Back, ScreenOne, ScreenTwo, Sprite };
    Source source;
    uint12 color;
  };

  struct Sprite {
    uint8 x;
    uint8 y;
    uint1 vflip;
    uint1 hflip;
    uint1 priority;
    uint1 window;
    uint4 palette;  //latchSprites() always sets bit3
    uint9 tile;
  };

  uint12 output[224 * 144];

  struct State {
    bool field;
    uint vclk;
    uint hclk;
    Pixel pixel;
  } s;

  struct Latches {
    //latchRegisters()
    uint8 backColor;

    uint1 screenOneEnable;
    uint4 screenOneMapBase;
    uint8 scrollOneX;
    uint8 scrollOneY;

    uint1 screenTwoEnable;
    uint4 screenTwoMapBase;
    uint8 scrollTwoX;
    uint8 scrollTwoY;
    uint1 screenTwoWindowEnable;
    uint1 screenTwoWindowInvert;
    uint8 screenTwoWindowX0;
    uint8 screenTwoWindowY0;
    uint8 screenTwoWindowX1;
    uint8 screenTwoWindowY1;

    uint1 spriteEnable;
    uint1 spriteWindowEnable;
    uint8 spriteWindowX0;
    uint8 spriteWindowY0;
    uint8 spriteWindowX1;
    uint8 spriteWindowY1;

    //latchSprites()
    Sprite sprite[32];
    uint spriteCount;

    //latchOAM()
    uint32 oam[2][128];
    uint oamCount;
  } l;

  struct Registers {
    //$0000  DISP_CTRL
    uint1 screenOneEnable;
    uint1 screenTwoEnable;
    uint1 spriteEnable;
    uint1 spriteWindowEnable;
    uint1 screenTwoWindowInvert;
    uint1 screenTwoWindowEnable;

    //$0001  BACK_COLOR
    uint8 backColor;

    //$0003  LINE_CMP
    uint8 lineCompare;

    //$0004  SPR_BASE
    uint6 spriteBase;

    //$0005  SPR_FIRST
    uint7 spriteFirst;

    //$0006  SPR_COUNT
    uint8 spriteCount;  //0 - 128

    //$0007  MAP_BASE
    uint4 screenOneMapBase;
    uint4 screenTwoMapBase;

    //$0008  SCR2_WIN_X0
    uint8 screenTwoWindowX0;

    //$0009  SCR2_WIN_Y0
    uint8 screenTwoWindowY0;

    //$000a  SCR2_WIN_X1
    uint8 screenTwoWindowX1;

    //$000b  SCR2_WIN_Y1
    uint8 screenTwoWindowY1;

    //$000c  SPR_WIN_X0
    uint8 spriteWindowX0;

    //$000d  SPR_WIN_Y0
    uint8 spriteWindowY0;

    //$000e  SPR_WIN_X1
    uint8 spriteWindowX1;

    //$000f  SPR_WIN_Y1
    uint8 spriteWindowY1;

    //$0010  SCR1_X
    uint8 scrollOneX;

    //$0011  SCR1_Y
    uint8 scrollOneY;

    //$0012  SCR2_X
    uint8 scrollTwoX;

    //$0013  SCR2_Y
    uint8 scrollTwoY;

    //$0014  LCD_CTRL
    uint1 lcdEnable;
    uint1 lcdContrast;  //WSC only
    uint6 lcdUnknown;

    //$0015  LCD_ICON
    uint1 iconSleep;
    uint1 iconVertical;
    uint1 iconHorizontal;
    uint1 iconAux1;
    uint1 iconAux2;
    uint1 iconAux3;

    //$0016  LCD_VTOTAL
    uint8 vtotal;

    //$0017  LCD_VBLANK
    uint8 vblank;

    //$001c-001f  PALMONO_POOL
    uint4 pool[8];

    //$0020-003f  PALMONO
    struct Palette {
      uint3 color[4];
    } palette[16];

    //$00a2  TMR_CTRL
    uint1 htimerEnable;
    uint1 htimerRepeat;
    uint1 vtimerEnable;
    uint1 vtimerRepeat;

    //$00a4,$00a5  HTMR_FREQ
    uint16 htimerFrequency;

    //$00a6,$00a7  VTMR_FREQ
    uint16 vtimerFrequency;

    //$00a8,$00a9  HTMR_CTR
    uint16 htimerCounter;

    //$00aa,$00ab  VTMR_CTR
    uint16 vtimerCounter;
  } r;
};

extern PPU ppu;
