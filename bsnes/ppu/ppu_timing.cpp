/*
  $213d : vertical latch position

  returns the current scanline. must read from
  $2137 before reading from this register.
*/
byte mmio_r2137(void) {
  ppu.latch_toggle = 0;
  ppu.latch_vpos = snes_time->vscan_pos;
  ppu.latch_hpos = snes_time->hscan_pos;
  return 0x00;
}

byte mmio_r213c(void) {
word r;
  r = ppu.latch_hpos;
  if(ppu.latch_toggle)r >>= 8;
  ppu.latch_toggle ^= 1;
  return r;
}

byte mmio_r213d(void) {
word r;
  r = ppu.latch_vpos;
  if(ppu.latch_toggle)r >>= 8;
  ppu.latch_toggle ^= 1;
  return r;
}

/*
  $213e : ppu1 status register

  trm0vvvv
  t: time over (?)
  r: range over (?)
  m: master/slave mode select (?)
  v: version # (returns 1)
*/
byte mmio_r213e(void) {
  return 0x01;
}

/*
  $213f : ppu2 status register
  fl0mvvvv
  f: field # (?)
  l: external signal applied (should be 0)
  m: ntsc/pal mode (0=ntsc, 1=pal)
  v: version # (returns 0)
*/
byte mmio_r213f(void) {
  return 0x00;
}

/*
  $4200 : counter enable
  n-vh---j

  n: nmi enable
  v: vertical counter enable
  h: horizontal counter enable
  j: automatic joypad enable

  the v/h counters must be enabled to invoke IRQs. the vertical
  counter will override the horizontal counter. in other words,
  if both v+h are set, only vertical IRQs will be performed.
*/
void mmio_w4200(byte value) {
  gx816->nmi_enabled   = (value & 0x80)?true:false;
  ppu.vcounter_enabled = (value & 0x20)?true:false;
  ppu.hcounter_enabled = (value & 0x10)?true:false;
  ppu.auto_joypad_read = (value & 0x01)?true:false;
}

/*
  $4207/$4208 : horizontal counter position

  9-bit value, used to invoke horizontal IRQs
  horizontal range: 0-339
*/
void mmio_w4207(byte value) {
  ppu.hirq_pos = (ppu.hirq_pos & 0x0100) | value;
}
void mmio_w4208(byte value) {
  ppu.hirq_pos = (ppu.hirq_pos & 0x00ff) | (value & 1) << 8;
}

/*
  $4209/$420a : vertical counter position

  9-bit value, used to invoke vertical IRQs
  vertical range: 0-261
*/
void mmio_w4209(byte value) {
  ppu.virq_pos = (ppu.virq_pos & 0x0100) | value;
}
void mmio_w420a(byte value) {
  ppu.virq_pos = (ppu.virq_pos & 0x00ff) | (value & 1) << 8;
}

/*
  $420d : memory speed
  0000000x

  x: 0 = SlowROM
     1 = FastROM
*/
void mmio_w420d(byte value) {
  gx816->toggle_memory_speed = (value) & 0x01;
}

/*
  $4210 : nmi status
  n-------

  n: outside nmi (0=no, 1=yes)
*/
byte mmio_r4210(void) {
byte r;
  r = (gx816->nmi_pin ^ 1)?0x80:0x00;
  gx816->nmi_pin = 1;
  return r;
}

/*
  $4211 : irq toggle
  i?------

  i: irq state (1=in irq, 0=not in irq)?
  ?: unknown, always return 1?
*/
byte mmio_r4211(void) {
byte r;
  r = 0x40;
  if(ppu.irq_triggered == true)r |= 0x80;
  ppu.irq_triggered = false;
  return r;
}

/*
  $4212 : video status
  vh-----j

  v: vblank (0=no, 1=yes)
  h: hblank (0=no, 1=yes)
  j: joypad ready (for auto joypad mode)
*/
byte mmio_r4212(void) {
byte r;
  r = 0x00;

//set when the SNES is updating the joypad data automatically
  if(snes_time->vscan_pos >= (ppu.visible_scanlines + 1) && snes_time->vscan_pos <= (ppu.visible_scanlines + 3))r |= 0x01;

//set when the SNES is in hblank/vblank
  if(snes_time->hscan_pos >= 256)r |= 0x40;
  if(snes_time->vscan_pos >= ppu.visible_scanlines)r |= 0x80;

  return r;
}
