auto PPU::portRead(uint16 addr) -> uint8 {
  //DISP_CTRL
  if(addr == 0x0000) {
    return (
      r.screenTwoWindowEnable << 5
    | r.screenTwoWindowMode   << 4
    | r.spriteWindowEnable    << 3
    | r.spriteEnable          << 2
    | r.screenTwoEnable       << 1
    | r.screenOneEnable       << 0
    );
  }

  //BACK_COLOR
  if(addr == 0x0001) return r.backColorPalette << 4 | r.backColorIndex << 0;

  //SPR_BASE
  if(addr == 0x0004) return r.spriteBase;

  //SPR_FIRST
  if(addr == 0x0005) return r.spriteFirst;

  //SPR_COUNT
  if(addr == 0x0006) return r.spriteCount;

  //MAP_BASE
  if(addr == 0x0007) return r.screenTwoMapBase << 4 | r.screenOneMapBase << 0;

  //SCR1_X
  if(addr == 0x0010) return r.scrollOneX;

  //SCR1_Y
  if(addr == 0x0011) return r.scrollOneY;

  //SCR2_X
  if(addr == 0x0012) return r.scrollTwoX;

  //SCR2_Y
  if(addr == 0x0013) return r.scrollTwoY;

  //LCD_CTRL
  if(addr == 0x0014) return r.control;

  //LCD_ICON
  if(addr == 0x0015) {
    return (
      r.iconAux3       << 5
    | r.iconAux2       << 4
    | r.iconAux1       << 3
    | r.iconHorizontal << 2
    | r.iconVertical   << 1
    | r.iconSleep      << 0
    );
  }

  //PALMONO_POOL_0
  if(addr == 0x001c) return r.monoPool[0] << 0 | r.monoPool[1] << 4;

  //PALMONO_POOL_1
  if(addr == 0x001d) return r.monoPool[2] << 0 | r.monoPool[3] << 4;

  //PALMONO_POOL_2
  if(addr == 0x001e) return r.monoPool[4] << 0 | r.monoPool[5] << 4;

  //PALMONO_POOL_3
  if(addr == 0x001f) return r.monoPool[6] << 0 | r.monoPool[7] << 4;

  //DISP_MODE
  if(addr == 0x0060) {
    return (
      r.bpp    << 7
    | r.color  << 6
    | r.format << 5
    | r.u0060  << 0
    );
  }

  return 0x00;
}

auto PPU::portWrite(uint16 addr, uint8 data) -> void {
  //DISP_CTRL
  if(addr == 0x0000) {
    r.screenTwoWindowEnable = data & 0x20;
    r.screenTwoWindowMode   = data & 0x10;
    r.spriteWindowEnable    = data & 0x08;
    r.spriteEnable          = data & 0x04;
    r.screenTwoEnable       = data & 0x02;
    r.screenOneEnable       = data & 0x01;
    return;
  }

  //BACK_COLOR
  if(addr == 0x0001) {
    if(WS()) {
      r.backColorPalette = 0;
      r.backColorIndex   = (uint3)(data >> 0);
    } else {
      r.backColorPalette = (uint4)(data >> 4);
      r.backColorIndex   = (uint4)(data >> 0);
    }
    return;
  }

  //SPR_BASE
  if(addr == 0x0004) {
    if(WS()) {
      r.spriteBase = (uint5)data;
    } else {
      r.spriteBase = (uint6)data;
    }
    return;
  }

  //SPR_FIRST
  if(addr == 0x0005) {
    r.spriteFirst = data;
    return;
  }

  //SPR_COUNT
  if(addr == 0x0006) {
    r.spriteCount = data;
    return;
  }

  //MAP_BASE
  if(addr == 0x0007) {
    if(WS()) {
      r.screenTwoMapBase = (uint3)(data >> 4);
      r.screenOneMapBase = (uint3)(data >> 0);
    } else {
      r.screenTwoMapBase = (uint4)(data >> 4);
      r.screenOneMapBase = (uint4)(data >> 0);
    }
    return;
  }

  //SCR1_X
  if(addr == 0x0010) {
    r.scrollOneX = data;
    return;
  }

  //SCR1_Y
  if(addr == 0x0011) {
    r.scrollOneY = data;
    return;
  }

  //SCR2_X
  if(addr == 0x0012) {
    r.scrollTwoX = data;
    return;
  }

  //SCR2_Y
  if(addr == 0x0013) {
    r.scrollTwoY = data;
    return;
  }

  //LCD_CTRL
  if(addr == 0x0014) {
    r.control = data;
    return;
  }

  //LCD_ICON
  if(addr == 0x0015) {
    r.iconAux3       = data & 0x20;
    r.iconAux2       = data & 0x10;
    r.iconAux1       = data & 0x08;
    r.iconHorizontal = data & 0x04;
    r.iconVertical   = data & 0x02;
    r.iconSleep      = data & 0x01;
    return;
  }

  //PALMONO_POOL_0
  if(addr == 0x001c) {
    r.monoPool[0] = data >> 0;
    r.monoPool[1] = data >> 4;
    return;
  }

  //PALMONO_POOL_1
  if(addr == 0x001d) {
    r.monoPool[2] = data >> 0;
    r.monoPool[3] = data >> 4;
    return;
  }

  //PALMONO_POOL_2
  if(addr == 0x001e) {
    r.monoPool[4] = data >> 0;
    r.monoPool[5] = data >> 4;
    return;
  }

  //PALMONO_POOL_3
  if(addr == 0x001f) {
    r.monoPool[6] = data >> 0;
    r.monoPool[7] = data >> 4;
    return;
  }

  //DISP_MODE
  if(addr == 0x0060) {
    r.bpp    = data & 0x80;
    r.color  = data & 0x40;
    r.format = data & 0x20;
    r.u0060  = data & 0b1011;
    return;
  }
}
