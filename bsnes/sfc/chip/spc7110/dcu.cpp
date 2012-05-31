#include "decompressor.cpp"

void SPC7110::dcu_load_address() {
  unsigned table = r4801 | r4802 << 8 | r4803 << 16;
  unsigned index = r4804 << 2;

  unsigned addr = table + index;
  dcu_mode  = datarom_read(addr + 0);
  dcu_addr  = datarom_read(addr + 1) << 16;
  dcu_addr |= datarom_read(addr + 2) <<  8;
  dcu_addr |= datarom_read(addr + 3) <<  0;
}

void SPC7110::dcu_begin_transfer() {
  if(dcu_mode == 3) return;  //invalid mode

  add_clocks(20);
  decompressor->initialize(dcu_mode, dcu_addr);

  if(r480b & 2) {
    unsigned seek = (r4805 | r4806 << 8) << dcu_mode;
    add_clocks(seek);
    while(seek--) decompressor->output();
  }

  r480c |= 0x80;
}

uint8 SPC7110::dcu_read() {
  if((r480c & 0x80) == 0) return 0x00;
  return decompressor->output();
}

/*
  unsigned tilesize = 8 << dcu_mode;

  if(dcu_sp == 0) {
    unsigned tiles = r480b & 1 ? r4807 : 1;
    for(unsigned tile = 0; tile < tiles; tile++) {
      dcu_dp = tile * tilesize;
      switch(dcu_mode) {
      case 0: decompress_1bpp(); deinterleave_1bpp(tiles); break;
      case 1: decompress_2bpp(); deinterleave_2bpp(tiles); break;
      case 2: decompress_4bpp(); deinterleave_4bpp(tiles); break;
      case 3: return 0x00;
      }
    }
  }

  uint8 data = dcu_output[dcu_sp++];
  dcu_sp &= tilesize - 1;
  return data;
*/

/*
void SPC7110::deinterleave_1bpp(unsigned length) {
  uint8 *target = dcu_output, *source = dcu_tiledata;
  for(unsigned row = 0, sp = 0; row < 8; row++) {
    target[row] = source[sp];
    sp += length;
  }
}

void SPC7110::deinterleave_2bpp(unsigned length) {
  uint8 *target = dcu_output, *source = dcu_tiledata;
  for(unsigned row = 0, sp = 0; row < 8; row++) {
    target[row * 2 + 0] = source[sp + 0];
    target[row * 2 + 1] = source[sp + 1];
    sp += 2 * length;
  }
}

void SPC7110::deinterleave_4bpp(unsigned length) {
  uint8 *target = dcu_output, *source = dcu_tiledata;
  for(unsigned row = 0, sp = 0; row < 8; row++) {
    target[row * 2 +  0] = source[sp +  0];
    target[row * 2 +  1] = source[sp +  1];
    target[row * 2 + 16] = source[sp + 16];
    target[row * 2 + 17] = source[sp + 17];
    sp = sp + 2 * length + 16 * ((sp + 2 * length) / 16 - sp / 16);
  }
}
*/
