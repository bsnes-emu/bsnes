/*
  $2137 : counter latch

  reading from this register will latch the x/y
  counter positions, but only if bit 7 of $4201
  is set. the default state of this register is
  $ff on snes power on/reset.
*/
byte mmio_r2137(void) {
  if(ppu.io4201 & 0x80) {
    snes_time->set_scan_pos(snes_time->master_cycles, false);
    ppu.latch_toggle = 0;
    ppu.latch_vpos = snes_time->vscan_pos;
    ppu.latch_hpos = snes_time->hscan_pos;
    ppu.counter_latched = true;
  }
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
  trm-vvvv

  t: time over (?)
  r: range over (?)
  m: master/slave mode select (?)
  -: open bus (?)
  v: PPU1 (5c77) version number [1]
*/
byte mmio_r213e(void) {
byte r = 0x00;
  r |= 0x01; //ppu1 version #
  return r;
}

/*
  $213f : ppu2 status register
  ilcmvvvv

  i: interlace frame (0=even, 1=odd)
  l: counter latched
  c: open bus complement
  m: ntsc/pal mode (0=ntsc, 1=pal)
  v: PPU2 (5c78) version number [3]

notes:
  bit 6 (l): counter is latched by reading $2137,
  or a 1->0 transition of $4201 bit 7.

  bit 5 (c): this is not implemented correctly. basically,
  my copier (super ufo 8.3j) breaks the snes open bus.
  as a result, 0x00 is always returned instead of the
  open bus values. since this is the complement of that
  value, 1 is always returned. I am emulating what my
  copier returns until I can get a copier that supports
  open bus correctly.
*/
byte mmio_r213f(void) {
byte r = 0x00;
  r |= ppu.interlace_frame << 7;
  if(!(ppu.io4201 & 0x80)) {
    r |= 1 << 6;
  } else if(ppu.counter_latched == true) {
    r |= 1 << 6;
    ppu.counter_latched = false;
  }
  r |= 1 << 5;
  r |= 0x03; //ppu2 version #
  return r;
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
  $4201 : programmable i/o port
  l???????

  l: counter latch

  upon power on/reset, this value is set to 0xff.
  clearing bit 7 will result in the x/y dot position
  being set in $213c/$213d. the counters cannot be
  latched again by writing a 0 to bit 7 until a 1 is
  written to this bit first. reading $2137 while bit 7
  is cleared will not latch the counters.

  examples (+ = counter latched, - = counter not latched):
    $00->$4201+
    $80->$4201-
    $00->$4201+ $00->$4201- $80->$4201- $00->$4201+
    $2137->a+   $00->$4201+ $2137->a-
*/
void mmio_w4201(byte value) {
  if((ppu.io4201 & 0x80) && !(value & 0x80)) {
    snes_time->set_scan_pos(snes_time->master_cycles + 4, false);
    ppu.latch_toggle = 0;
    ppu.latch_vpos = snes_time->vscan_pos;
    ppu.latch_hpos = snes_time->hscan_pos;
    ppu.counter_latched = true;
  }
  ppu.io4201 = value;
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
  gx816->memory_speed = (value) & 0x01;
  snes_time->set_speed_map(gx816->memory_speed);
}

/*
  $4210 : nmi status
  n---vvvv

  n: nmi occurred (0=no, 1=yes)
  -: open bus (?)
  v: CPU (5a22) version number [2]

  value is set/cleared regardless of whether or not
  NMIs are enabled in $4200 bit 7.
*/
byte mmio_r4210(void) {
byte r = 0x00;
  r |= (gx816->nmi_pin ^ 1)?0x80:0x00;
  r |= 0x02; //cpu version #
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
byte r = 0x00;
  r |= 0x40;
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
  if(snes_time->hcycle_pos <= 4 || snes_time->hcycle_pos >= 1098)r |= 0x40;
  if(snes_time->vscan_pos == 0 || snes_time->vscan_pos >= (ppu.visible_scanlines + 1)) {
    if(snes_time->vscan_pos == 0) {
      if(snes_time->hcycle_pos <  2)r |= 0x80;
    } else if(snes_time->vscan_pos == (ppu.visible_scanlines + 1)) {
      if(snes_time->hcycle_pos >= 2)r |= 0x80;
    } else {
      r |= 0x80;
    }
  }

  return r;
}
