/*
  $2100 : screen brightness / enable
  d---bbbb

  d: display (0=on, 1=off)
  b: brightness (0-15)
*/
extern emustate emu_state;

void mmio_w2100(byte value) {
  ppu.display_disable    = (value & 0x80)?true:false;
  ppu.display_brightness = (value & 0x0f);
}

/*
  $2105 : screen mode register
  dcbapmmm

  d: bg4 tile size (0=8x8, 1=16x16)
  c: bg3 tile size (0=8x8, 1=16x16)
  b: bg2 tile size (0=8x8, 1=16x16)
  a: bg1 tile size (0=8x8, 1=16x16)
  p: bg priority mode
  m: screen mode
*/
void mmio_w2105(byte value) {
  ppu.bg_tile_size[BG4] = (value & 0x80)?1:0;
  ppu.bg_tile_size[BG3] = (value & 0x40)?1:0;
  ppu.bg_tile_size[BG2] = (value & 0x20)?1:0;
  ppu.bg_tile_size[BG1] = (value & 0x10)?1:0;
  ppu.bg_priority_mode  = (value & 0x08)?1:0;
  ppu.bg_mode           = (value & 0x07);
  video_setsnesmode();
}

/*
  $2106 : mosaic
  ssssdcba

  s: size (0=smallest, 15=largest)
  d: affect bg4
  c: affect bg3
  b: affect bg2
  a: affect bg1
*/
void mmio_w2106(byte value) {
  ppu.mosaic_size         = (value >> 4) & 15;
  ppu.mosaic_enabled[BG4] = (value & 0x08)?true:false;
  ppu.mosaic_enabled[BG3] = (value & 0x04)?true:false;
  ppu.mosaic_enabled[BG2] = (value & 0x02)?true:false;
  ppu.mosaic_enabled[BG1] = (value & 0x01)?true:false;
}

/*
  $212c : main screen desgination
  $212d : sub screen designation
  ---sdcba

  s: oam enable
  d: bg4 enable
  c: bg3 enable
  b: bg2 enable
  a: bg1 enable
*/
void mmio_w212c(byte value) {
  ppu.bg_enabled[OAM] = (value & 0x10)?true:false;
  ppu.bg_enabled[BG4] = (value & 0x08)?true:false;
  ppu.bg_enabled[BG3] = (value & 0x04)?true:false;
  ppu.bg_enabled[BG2] = (value & 0x02)?true:false;
  ppu.bg_enabled[BG1] = (value & 0x01)?true:false;
}

void mmio_w212d(byte value) {
  ppu.ss_bg_enabled[OAM] = (value & 0x10)?true:false;
  ppu.ss_bg_enabled[BG4] = (value & 0x08)?true:false;
  ppu.ss_bg_enabled[BG3] = (value & 0x04)?true:false;
  ppu.ss_bg_enabled[BG2] = (value & 0x02)?true:false;
  ppu.ss_bg_enabled[BG1] = (value & 0x01)?true:false;
}

/*
  $2133 : screen mode settings
  ?????h?i

  h: snes height (0 = 224, 1 = 240)
  i: interlace   (0 = off, 1 = on)
*/
void mmio_w2133(byte value) {
  ppu.toggle_visible_scanlines = (value & 0x04)?240:224;
  ppu.interlace                = (value & 0x01)?true:false;
  video_setsnesmode();
}
