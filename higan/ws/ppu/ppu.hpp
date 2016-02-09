#include "video.hpp"

struct PPU : Thread, IO {
  static auto Enter() -> void;
  auto main() -> void;
  auto scanline() -> void;
  auto frame() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  uint16 output[224 * 144] = {0};

  struct Status {
    uint vclk;
    uint hclk;
  } status;

  struct Registers {
    //$0000  DISP_CTRL
    bool screenTwoWindowEnable;
    bool screenTwoWindowMode;
    bool spriteWindowEnable;
    bool spriteEnable;
    bool screenTwoEnable;
    bool screenOneEnable;

    //$0001  BACK_COLOR
    uint4 backColorIndex;
    uint4 backColorPalette;

    //$0004  SPR_BASE
    uint6 spriteBase;

    //$0005  SPR_FIRST
    uint7 spriteFirst;

    //$0006  SPR_COUNT
    uint8 spriteCount;  //0 - 128

    //$0007  MAP_BASE
    uint4 screenTwoMapBase;
    uint4 screenOneMapBase;

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

    //$001c-001f  PALMONO_POOL
    uint4 monoPool[8];

    //$0060  DISP_MODE
    bool bpp;
    bool color;
    bool format;
    uint5 u0060;  //unknown purpose
  } r;
};

extern PPU ppu;
