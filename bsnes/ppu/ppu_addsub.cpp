/*
  $2130 : fixed color / screen add sub
  abcd--ef

  ab: main
  cd: subscreen
    00: all the time
    01: inside window only
    10: outside window only
    11: all the time
  e: 0 = enable addsub for fixed color
     1 = enable addsub for sub screen
  f: color / chardata = direct color
     (mode3, 4, 7 only)
*/
void mmio_w2130(byte value) {
  ppu.color_mask    = (value >> 6) & 3;
  ppu.ss_color_mask = (value >> 4) & 3;
  ppu.addsub_mode   = (value & 0x02)?1:0;
}

/*
  $2131 : addsub designation
  mrgsdcba

  m: 0 = enable add color data mode
     1 = enable sub color data mode
  r: 1/2 color mode
  g: Backarea enable
  s: OAM enable
  d: BG4 enable
  c: BG3 enable
  b: BG2 enable
  a: BG1 enable
*/
void mmio_w2131(byte value) {
  ppu.color_mode             = (value & 0x80)?COLORMODE_SUB:COLORMODE_ADD;
  ppu.color_halve            = (value & 0x40)?1:0;
  ppu.bg_color_enabled[BACK] = (value & 0x20)?true:false;
  ppu.bg_color_enabled[OAM]  = (value & 0x10)?true:false;
  ppu.bg_color_enabled[BG4]  = (value & 0x08)?true:false;
  ppu.bg_color_enabled[BG3]  = (value & 0x04)?true:false;
  ppu.bg_color_enabled[BG2]  = (value & 0x02)?true:false;
  ppu.bg_color_enabled[BG1]  = (value & 0x01)?true:false;
}

/*
  $2132 : addsub settings
  bgrddddd

  b: affect blue
  g: affect green
  r: affect red
  d: color constant
*/
void mmio_w2132(byte value) {
  if(value & 0x80)ppu.color_b = (value & 0x1f);
  if(value & 0x40)ppu.color_g = (value & 0x1f);
  if(value & 0x20)ppu.color_r = (value & 0x1f);
}
