/*
  $2123/$2124/$2125 : window mask settings

  $2123:
    hgfedcba
    (bg2)
    h: enable window 2
    g: clip window 2 (0=in, 1=out)
    f: enable window 1
    e: clip window 1 (0=in, 1=out)
    (bg1)
    h: enable window 2
    g: clip window 2 (0=in, 1=out)
    f: enable window 1
    e: clip window 1 (0=in, 1=out)
  $2124: same as $2123, but with bg4/3
  $2125:
    hgfedcba
    h: enable color window 2
    g: clip window 2 (0=in, 1=out)
    f: enable color window 1
    e: clip window 1 (0=in, 1=out)
    d: enable OAM window 2
    c: clip window 2 (0=in, 1=out)
    b: enable OAM window 1
    a: clip window 1 (0=in, 1=out)
*/
void mmio_w2123(byte value) {
  ppu.bg_window2_enabled [BG2] = (value & 0x80)?true:false;
  ppu.bg_window2_clipmode[BG2] = (value & 0x40)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window1_enabled [BG2] = (value & 0x20)?true:false;
  ppu.bg_window1_clipmode[BG2] = (value & 0x10)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window2_enabled [BG1] = (value & 0x08)?true:false;
  ppu.bg_window2_clipmode[BG1] = (value & 0x04)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window1_enabled [BG1] = (value & 0x02)?true:false;
  ppu.bg_window1_clipmode[BG1] = (value & 0x01)?CLIPMODE_OUT:CLIPMODE_IN;
}

void mmio_w2124(byte value) {
  ppu.bg_window2_enabled [BG4] = (value & 0x80)?true:false;
  ppu.bg_window2_clipmode[BG4] = (value & 0x40)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window1_enabled [BG4] = (value & 0x20)?true:false;
  ppu.bg_window1_clipmode[BG4] = (value & 0x10)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window2_enabled [BG3] = (value & 0x08)?true:false;
  ppu.bg_window2_clipmode[BG3] = (value & 0x04)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window1_enabled [BG3] = (value & 0x02)?true:false;
  ppu.bg_window1_clipmode[BG3] = (value & 0x01)?CLIPMODE_OUT:CLIPMODE_IN;
}

void mmio_w2125(byte value) {
  ppu.color_window2_enabled  = (value & 0x80)?true:false;
  ppu.color_window2_clipmode = (value & 0x40)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.color_window1_enabled  = (value & 0x20)?true:false;
  ppu.color_window1_clipmode = (value & 0x10)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window2_enabled [OAM] = (value & 0x08)?true:false;
  ppu.bg_window2_clipmode[OAM] = (value & 0x04)?CLIPMODE_OUT:CLIPMODE_IN;
  ppu.bg_window1_enabled [OAM] = (value & 0x02)?true:false;
  ppu.bg_window1_clipmode[OAM] = (value & 0x01)?CLIPMODE_OUT:CLIPMODE_IN;
}

/*
 $2126-$2129 : window position settings

  $2126: window 1 left
  $2127: window 1 right
  $2128: window 2 left
  $2129: window 2 right
*/
void mmio_w2126(byte value) { ppu.window1_left  = value; }
void mmio_w2127(byte value) { ppu.window1_right = value; }
void mmio_w2128(byte value) { ppu.window2_left  = value; }
void mmio_w2129(byte value) { ppu.window2_right = value; }

/*
  $212a/$212b : window mask settings
  $212a: ddccbbaa (d=bg4, c=bg3, b=bg2, a=bg1)
  $212b: ----ccss (c=color add/sub, s=oam)

  00=or
  01=and
  10=xor
  11=xnor
*/
void mmio_w212a(byte value) {
  ppu.bg_window_mask[BG4] = (value >> 6) & 3;
  ppu.bg_window_mask[BG3] = (value >> 4) & 3;
  ppu.bg_window_mask[BG2] = (value >> 2) & 3;
  ppu.bg_window_mask[BG1] = (value     ) & 3;
}

void mmio_w212b(byte value) {
  ppu.color_window_mask   = (value >> 2) & 3;
  ppu.bg_window_mask[OAM] = (value     ) & 3;
}

/*
  $212e/$212f : main window designation

  ---odcba

  o: OAM enable
  d: BG4 enable
  c: BG3 enable
  b: BG2 enable
  a: BG1 enable
*/
void mmio_w212e(byte value) {
  ppu.bg_windowing_enabled[OAM] = (value & 0x10)?true:false;
  ppu.bg_windowing_enabled[BG4] = (value & 0x08)?true:false;
  ppu.bg_windowing_enabled[BG3] = (value & 0x04)?true:false;
  ppu.bg_windowing_enabled[BG2] = (value & 0x02)?true:false;
  ppu.bg_windowing_enabled[BG1] = (value & 0x01)?true:false;
}

void mmio_w212f(byte value) {
  ppu.ss_bg_windowing_enabled[OAM] = (value & 0x10)?true:false;
  ppu.ss_bg_windowing_enabled[BG4] = (value & 0x08)?true:false;
  ppu.ss_bg_windowing_enabled[BG3] = (value & 0x04)?true:false;
  ppu.ss_bg_windowing_enabled[BG2] = (value & 0x02)?true:false;
  ppu.ss_bg_windowing_enabled[BG1] = (value & 0x01)?true:false;
}
