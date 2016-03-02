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

  //render.cpp
  auto renderBack() -> void;
  auto renderScreenOne() -> void;
  auto renderScreenTwo() -> void;
  auto renderSprite() -> void;

  //state
  uint12 output[224 * 144];

  struct Status {
    uint vclk;
    uint hclk;
  } status;

  struct Pixel {
    enum class Source : uint { Back, ScreenOne, ScreenTwo, Sprite };
    Source source;
    uint12 color;
  } pixel;

  struct Registers {
    //$0000  DISP_CTRL
    bool screenTwoWindowEnable;
    bool screenTwoWindowInvert;
    bool spriteWindowEnable;
    bool spriteEnable;
    bool screenTwoEnable;
    bool screenOneEnable;

    //$0001  BACK_COLOR
    uint4 backColorIndex;
    uint4 backColorPalette;

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
    bool iconAux3;
    bool iconAux2;
    bool iconAux1;
    bool iconHorizontal;
    bool iconVertical;
    bool iconSleep;

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

    //$0060  DISP_MODE
    bool bpp;
    bool color;
    bool format;
    uint5 u0060;  //unknown purpose
  } r;
};

extern PPU ppu;
