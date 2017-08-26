auto PPU::portRead(uint16 addr) -> uint8 {
  //DISP_CTRL
  if(addr == 0x0000) return (
    r.screenOneEnable       << 0
  | r.screenTwoEnable       << 1
  | r.spriteEnable          << 2
  | r.spriteWindowEnable    << 3
  | r.screenTwoWindowInvert << 4
  | r.screenTwoWindowEnable << 5
  );

  //BACK_COLOR
  if(addr == 0x0001) return (
    r.backColor.bits(0, !system.color() ? 2 : 7)
  );

  //LINE_CUR
  if(addr == 0x0002) return s.vclk;

  //LINE_CMP
  if(addr == 0x0003) return r.lineCompare;

  //SPR_BASE
  if(addr == 0x0004) return (
    r.spriteBase.bits(0, 4 + system.depth())
  );

  //SPR_FIRST
  if(addr == 0x0005) return r.spriteFirst;

  //SPR_COUNT
  if(addr == 0x0006) return r.spriteCount;

  //MAP_BASE
  if(addr == 0x0007) return (
    r.screenOneMapBase.bits(0, 2 + system.depth()) << 0
  | r.screenTwoMapBase.bits(0, 2 + system.depth()) << 4
  );

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
  if(addr == 0x0014) return (
    r.lcdEnable   << 0
  | r.lcdContrast << 1
  | r.lcdUnknown  << 2
  );

  //LCD_ICON
  if(addr == 0x0015) return (
    r.iconSleep      << 0
  | r.iconVertical   << 1
  | r.iconHorizontal << 2
  | r.iconAux1       << 3
  | r.iconAux2       << 4
  | r.iconAux3       << 5
  );

  //LCD_VTOTAL
  if(addr == 0x0016) return r.vtotal;

  //LCD_VBLANK
  if(addr == 0x0017) return r.vblank;

  //PALMONO_POOL
  if(addr >= 0x001c && addr <= 0x001f) return (
    r.pool[addr.bits(0,1) * 2 + 0] << 0
  | r.pool[addr.bits(0,1) * 2 + 1] << 4
  );

  //PALMONO
  if(addr >= 0x0020 && addr <= 0x003f) return (
    r.palette[addr.bits(1,4)].color[addr.bit(0) * 2 + 0] << 0
  | r.palette[addr.bits(1,4)].color[addr.bit(0) * 2 + 1] << 4
  );

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
    r.screenOneEnable       = data.bit(0);
    r.screenTwoEnable       = data.bit(1);
    r.spriteEnable          = data.bit(2);
    r.spriteWindowEnable    = data.bit(3);
    r.screenTwoWindowInvert = data.bit(4);
    r.screenTwoWindowEnable = data.bit(5);
  }

  //BACK_COLOR
  if(addr == 0x0001) r.backColor = data;

  //LINE_CMP
  if(addr == 0x0003) r.lineCompare = data;

  //SPR_BASE
  if(addr == 0x0004) r.spriteBase = data.bits(0,5);

  //SPR_FIRST
  if(addr == 0x0005) r.spriteFirst = data.bits(6,0);

  //SPR_COUNT
  if(addr == 0x0006) r.spriteCount = data;

  //MAP_BASE
  if(addr == 0x0007) {
    r.screenOneMapBase = data.bits(0,3);
    r.screenTwoMapBase = data.bits(4,7);
  }

  //SCR2_WIN_X0
  if(addr == 0x0008) r.screenTwoWindowX0 = data;

  //SCR2_WIN_Y0
  if(addr == 0x0009) r.screenTwoWindowY0 = data;

  //SCR2_WIN_X1
  if(addr == 0x000a) r.screenTwoWindowX1 = data;

  //SCR2_WIN_Y1
  if(addr == 0x000b) r.screenTwoWindowY1 = data;

  //SPR_WIN_X0
  if(addr == 0x000c) r.spriteWindowX0 = data;

  //SPR_WIN_Y0
  if(addr == 0x000d) r.spriteWindowY0 = data;

  //SPR_WIN_X1
  if(addr == 0x000e) r.spriteWindowX1 = data;

  //SPR_WIN_Y1
  if(addr == 0x000f) r.spriteWindowY1 = data;

  //SCR1_X
  if(addr == 0x0010) r.scrollOneX = data;

  //SCR1_Y
  if(addr == 0x0011) r.scrollOneY = data;

  //SCR2_X
  if(addr == 0x0012) r.scrollTwoX = data;

  //SCR2_Y
  if(addr == 0x0013) r.scrollTwoY = data;

  //LCD_CTRL
  if(addr == 0x0014) {
    r.lcdEnable   = data.bit (0);
    r.lcdContrast = data.bit (1);
    r.lcdUnknown  = data.bits(2,7);

    if(Model::WonderSwanColor()) {
      r.lcdUnknown &= 0b111100;
    }

    if(Model::SwanCrystal()) {
      r.lcdContrast = 0;
      r.lcdUnknown  = 0;
    }
  }

  //LCD_ICON
  if(addr == 0x0015) {
    r.iconSleep      = data.bit(0);
    r.iconVertical   = data.bit(1);
    r.iconHorizontal = data.bit(2);
    r.iconAux1       = data.bit(3);
    r.iconAux2       = data.bit(4);
    r.iconAux3       = data.bit(5);
  }

  //LCD_VTOTAL
  if(addr == 0x0016) r.vtotal = data;

  //LCD_VBLANK
  if(addr == 0x0017) r.vblank = data;

  //PALMONO_POOL
  if(addr >= 0x001c && addr <= 0x001f) {
    r.pool[addr.bits(0,1) * 2 + 0] = data.bits(0,3);
    r.pool[addr.bits(0,1) * 2 + 1] = data.bits(4,7);
  }

  //PALMONO
  if(addr >= 0x0020 && addr <= 0x003f) {
    r.palette[addr.bits(1,4)].color[addr.bit(0) * 2 + 0] = data.bits(0,2);
    r.palette[addr.bits(1,4)].color[addr.bit(0) * 2 + 1] = data.bits(4,6);
  }

  //TMR_CTRL
  if(addr == 0x00a2) {
    r.htimerEnable = data.bit(0);
    r.htimerRepeat = data.bit(1);
    r.vtimerEnable = data.bit(2);
    r.vtimerRepeat = data.bit(3);

    if(r.htimerEnable) r.htimerCounter = 0;
    if(r.vtimerEnable) r.vtimerCounter = 0;
  }

  //HTMR_FREQ
  if(addr == 0x00a4) r.htimerFrequency.byte(0) = data;
  if(addr == 0x00a5) r.htimerFrequency.byte(1) = data;

  if(addr == 0x00a4 || addr == 0x00a5) {
    r.htimerEnable = true;
    r.htimerRepeat = true;
    r.htimerCounter = 0;
  }

  //VTMR_FREQ
  if(addr == 0x00a6) r.vtimerFrequency.byte(0) = data;
  if(addr == 0x00a7) r.vtimerFrequency.byte(1) = data;

  if(addr == 0x00a6 || addr == 0x00a7) {
    r.vtimerEnable = true;
    r.vtimerRepeat = true;
    r.vtimerCounter = 0;
  }
}
