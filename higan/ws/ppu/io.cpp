auto PPU::portRead(uint16 addr) -> uint8 {
  //DISP_CTRL
  if(addr == 0x0000) {
    return (
      r.screenTwoWindowEnable << 5
    | r.screenTwoWindowInvert << 4
    | r.spriteWindowEnable    << 3
    | r.spriteEnable          << 2
    | r.screenTwoEnable       << 1
    | r.screenOneEnable       << 0
    );
  }

  //BACK_COLOR
  if(addr == 0x0001) {
    if(!system.depth()) {
      return r.backColor.bits(0,2);
    } else {
      return r.backColor.bits(0,7);
    }
  }

  //LINE_CUR
  if(addr == 0x0002) return status.vclk;

  //LINE_CMP
  if(addr == 0x0003) return r.lineCompare;

  //SPR_BASE
  if(addr == 0x0004) {
    if(!system.depth()) {
      return r.spriteBase.bits(0,4);
    } else {
      return r.spriteBase.bits(0,5);
    }
  }

  //SPR_FIRST
  if(addr == 0x0005) return r.spriteFirst;

  //SPR_COUNT
  if(addr == 0x0006) return r.spriteCount;

  //MAP_BASE
  if(addr == 0x0007) {
    if(!system.depth()) {
      return r.screenTwoMapBase.bits(0,2) << 4 | r.screenOneMapBase.bits(0,2) << 0;
    } else {
      return r.screenTwoMapBase.bits(0,3) << 4 | r.screenOneMapBase.bits(0,3) << 0;
    }
  }

  //SCR2_WIN_X0
  if(addr == 0x0008) return r.screenTwoWindowX0;

  //SCR2_WIN_Y0
  if(addr == 0x0009) return r.screenTwoWindowY0;

  //SCR2_WIN_X1
  if(addr == 0x000a) return r.screenTwoWindowX1;

  //SCR2_WIN_Y1
  if(addr == 0x000b) return r.screenTwoWindowY1;

  //SPR_WIN_X0
  if(addr == 0x000c) return r.spriteWindowX0;

  //SPR_WIN_Y0
  if(addr == 0x000d) return r.spriteWindowY0;

  //SPR_WIN_X1
  if(addr == 0x000e) return r.spriteWindowX1;

  //SPR_WIN_Y1
  if(addr == 0x000f) return r.spriteWindowY1;

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

  //LCD_VTOTAL
  if(addr == 0x0016) return r.vtotal;

  //LCD_VBLANK
  if(addr == 0x0017) return r.vblank;

  //PALMONO_POOL
  if(addr >= 0x001c && addr <= 0x001f) {
    return (
      r.pool[addr.bits(1,0) * 2 + 1] << 4
    | r.pool[addr.bits(1,0) * 2 + 0] << 0
    );
  }

  //PALMONO
  if(addr >= 0x0020 && addr <= 0x003f) {
    return (
      r.palette[addr.bits(3,1)].color[addr.bit(0) * 2 + 1] << 4
    | r.palette[addr.bits(3,1)].color[addr.bit(0) * 2 + 0] << 0
    );
  }

  //TMR_CTRL
  if(addr == 0x00a2) return (
    r.htimerEnable << 0
  | r.htimerRepeat << 1
  | r.vtimerEnable << 2
  | r.vtimerRepeat << 3
  );

  //HTMR_FREQ
  if(addr == 0x00a4) return r.htimerFrequency.byte(0);
  if(addr == 0x00a5) return r.htimerFrequency.byte(1);

  //VTMR_FREQ
  if(addr == 0x00a6) return r.vtimerFrequency.byte(0);
  if(addr == 0x00a7) return r.vtimerFrequency.byte(1);

  //HTMR_CTR
  if(addr == 0x00a8) return r.htimerCounter.byte(0);
  if(addr == 0x00a9) return r.htimerCounter.byte(1);

  //VTMR_CTR
  if(addr == 0x00aa) return r.vtimerCounter.byte(0);
  if(addr == 0x00ab) return r.vtimerCounter.byte(1);

  return 0x00;
}

auto PPU::portWrite(uint16 addr, uint8 data) -> void {
  //DISP_CTRL
  if(addr == 0x0000) {
    r.screenTwoWindowEnable = data.bit(5);
    r.screenTwoWindowInvert = data.bit(4);
    r.spriteWindowEnable    = data.bit(3);
    r.spriteEnable          = data.bit(2);
    r.screenTwoEnable       = data.bit(1);
    r.screenOneEnable       = data.bit(0);
    return;
  }

  //BACK_COLOR
  if(addr == 0x0001) {
    r.backColor = data;
    return;
  }

  //LINE_CMP
  if(addr == 0x0003) {
    r.lineCompare = data;
    return;
  }

  //SPR_BASE
  if(addr == 0x0004) {
    r.spriteBase = data.bits(0,5);
    return;
  }

  //SPR_FIRST
  if(addr == 0x0005) {
    r.spriteFirst = data.bits(6,0);
    return;
  }

  //SPR_COUNT
  if(addr == 0x0006) {
    r.spriteCount = data;  //0 - 128
    return;
  }

  //MAP_BASE
  if(addr == 0x0007) {
    r.screenOneMapBase = data.bits(0,3);
    r.screenTwoMapBase = data.bits(4,7);
    return;
  }

  //SCR2_WIN_X0
  if(addr == 0x0008) {
    r.screenTwoWindowX0 = data;
    return;
  }

  //SCR2_WIN_Y0
  if(addr == 0x0009) {
    r.screenTwoWindowY0 = data;
    return;
  }

  //SCR2_WIN_X1
  if(addr == 0x000a) {
    r.screenTwoWindowX1 = data;
    return;
  }

  //SCR2_WIN_Y1
  if(addr == 0x000b) {
    r.screenTwoWindowY1 = data;
    return;
  }

  //SPR_WIN_X0
  if(addr == 0x000c) {
    r.spriteWindowX0 = data;
    return;
  }

  //SPR_WIN_Y0
  if(addr == 0x000d) {
    r.spriteWindowY0 = data;
    return;
  }

  //SPR_WIN_X1
  if(addr == 0x000e) {
    r.spriteWindowX1 = data;
    return;
  }

  //SPR_WIN_Y1
  if(addr == 0x000f) {
    r.spriteWindowY1 = data;
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
    r.iconAux3       = data.bit(5);
    r.iconAux2       = data.bit(4);
    r.iconAux1       = data.bit(3);
    r.iconHorizontal = data.bit(2);
    r.iconVertical   = data.bit(1);
    r.iconSleep      = data.bit(0);
    return;
  }

  //LCD_VTOTAL
  if(addr == 0x0016) {
    r.vtotal = data;
    return;
  }

  //LCD_VBLANK
  if(addr == 0x0017) {
    r.vblank = data;
    return;
  }

  //PALMONO_POOL
  if(addr >= 0x001c && addr <= 0x001f) {
    r.pool[addr.bits(1,0) * 2 + 1] = data.bits(7,4);
    r.pool[addr.bits(1,0) * 2 + 0] = data.bits(3,0);
    return;
  }

  //PALMONO
  if(addr >= 0x0020 && addr <= 0x003f) {
    r.palette[addr.bits(4,1)].color[addr.bit(0) * 2 + 1] = data.bits(6,4);
    r.palette[addr.bits(4,1)].color[addr.bit(0) * 2 + 0] = data.bits(2,0);
    return;
  }

  //TMR_CTRL
  if(addr == 0x00a2) {
    r.htimerEnable = data.bit(0);
    r.htimerRepeat = data.bit(1);
    r.vtimerEnable = data.bit(2);
    r.vtimerRepeat = data.bit(3);
  }

  //HTMR_FREQ
  if(addr == 0x00a4) r.htimerFrequency.byte(0) = data;
  if(addr == 0x00a5) r.htimerFrequency.byte(1) = data;

  //VTMR_FREQ
  if(addr == 0x00a6) r.vtimerFrequency.byte(0) = data;
  if(addr == 0x00a7) r.vtimerFrequency.byte(1) = data;
}
