/*
  $210d-$2114 : Scroll registers

  210d/210e: bg1 hscroll/bg1 vscroll
  210f/2110: bg2 hscroll/bg2 vscroll
  2111/2112: bg3 hscroll/bg3 vscroll
  2113/2114: bg4 hscroll/bg4 vscroll

  you must write to this register twice to write the full address.
  starting positions are 0, 0. only 11 bits of the address are used.
*/
void mmio_w210d(byte value) {
  ppu.bg_hscroll_pos[BG1] = (value << 8) | (ppu.bg_hscroll_pos[BG1] >> 8);
  ppu.m7hofs = (value << 8) | m7_latch;
  m7_latch   = value;
}

void mmio_w210e(byte value) {
  ppu.bg_vscroll_pos[BG1] = (value << 8) | (ppu.bg_vscroll_pos[BG1] >> 8);
  ppu.m7vofs = (value << 8) | m7_latch;
  m7_latch   = value;
}

void mmio_w210f(byte value) {
  ppu.bg_hscroll_pos[BG2] = (value << 8) | (ppu.bg_hscroll_pos[BG2] >> 8);
}

void mmio_w2110(byte value) {
  ppu.bg_vscroll_pos[BG2] = (value << 8) | (ppu.bg_vscroll_pos[BG2] >> 8);
}

void mmio_w2111(byte value) {
  ppu.bg_hscroll_pos[BG3] = (value << 8) | (ppu.bg_hscroll_pos[BG3] >> 8);
}

void mmio_w2112(byte value) {
  ppu.bg_vscroll_pos[BG3] = (value << 8) | (ppu.bg_vscroll_pos[BG3] >> 8);
}

void mmio_w2113(byte value) {
  ppu.bg_hscroll_pos[BG4] = (value << 8) | (ppu.bg_hscroll_pos[BG4] >> 8);
}

void mmio_w2114(byte value) {
  ppu.bg_vscroll_pos[BG4] = (value << 8) | (ppu.bg_vscroll_pos[BG4] >> 8);
}
