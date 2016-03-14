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

  //render-sprite.cpp
  auto renderSpriteDecode() -> void;

  //render-mono.cpp
  auto renderMonoFetch(uint14 offset, uint3 y, uint3 x) -> uint2;
  auto renderMonoBack() -> void;
  auto renderMonoScreenOne() -> void;
  auto renderMonoScreenTwo() -> void;
  auto renderMonoSprite() -> void;

  //render-color.cpp
  auto renderColorFetch(uint16 offset, uint3 y, uint3 x) -> uint4;
  auto renderColorBack() -> void;
  auto renderColorScreenOne() -> void;
  auto renderColorScreenTwo() -> void;
  auto renderColorSprite() -> void;

  //state
  uint12 output[224 * 144];

  struct Status {
    uint vclk;
    uint hclk;
  } status;

  struct Sprite {
    uint8 x;
    uint8 y;
    uint1 vflip;
    uint1 hflip;
    uint1 priority;
    uint1 window;
    uint4 palette;  //renderSpriteDecode() always sets bit3
    uint9 tile;
  };
  vector<Sprite> sprites;

  struct Pixel {
    enum class Source : uint { Back, ScreenOne, ScreenTwo, Sprite };
    Source source;
    uint12 color;
  } pixel;

  struct Registers {
    //$0000  DISP_CTRL
    uint1 screenTwoWindowEnable;
    uint1 screenTwoWindowInvert;
    uint1 spriteWindowEnable;
    uint1 spriteEnable;
    uint1 screenTwoEnable;
    uint1 screenOneEnable;

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
    uint4 screenTwoMapBase;
    uint4 screenOneMapBase;

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
    uint8 control;

    //$0015  LCD_ICON
    uint1 iconAux3;
    uint1 iconAux2;
    uint1 iconAux1;
    uint1 iconHorizontal;
    uint1 iconVertical;
    uint1 iconSleep;

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
