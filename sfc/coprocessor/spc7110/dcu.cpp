#include "decompressor.cpp"

auto SPC7110::dcu_load_address() -> void {
  uint table = r4801 | r4802 << 8 | r4803 << 16;
  uint index = r4804 << 2;

  uint addr = table + index;
  dcu_mode  = datarom_read(addr + 0);
  dcu_addr  = datarom_read(addr + 1) << 16;
  dcu_addr |= datarom_read(addr + 2) <<  8;
  dcu_addr |= datarom_read(addr + 3) <<  0;
}

auto SPC7110::dcu_begin_transfer() -> void {
  if(dcu_mode == 3) return;  //invalid mode

  add_clocks(20);
  decompressor->initialize(dcu_mode, dcu_addr);
  decompressor->decode();

  uint seek = r480b & 2 ? r4805 | r4806 << 8 : 0;
  while(seek--) decompressor->decode();

  r480c |= 0x80;
  dcu_offset = 0;
}

auto SPC7110::dcu_read() -> uint8 {
  if((r480c & 0x80) == 0) return 0x00;

  if(dcu_offset == 0) {
    for(auto row : range(8)) {
      switch(decompressor->bpp) {
      case 1:
        dcu_tile[row] = decompressor->result;
        break;
      case 2:
        dcu_tile[row * 2 + 0] = decompressor->result >> 0;
        dcu_tile[row * 2 + 1] = decompressor->result >> 8;
        break;
      case 4:
        dcu_tile[row * 2 +  0] = decompressor->result >>  0;
        dcu_tile[row * 2 +  1] = decompressor->result >>  8;
        dcu_tile[row * 2 + 16] = decompressor->result >> 16;
        dcu_tile[row * 2 + 17] = decompressor->result >> 24;
        break;
      }

      uint seek = r480b & 1 ? r4807 : 1;
      while(seek--) decompressor->decode();
    }
  }

  uint8 data = dcu_tile[dcu_offset++];
  dcu_offset &= 8 * decompressor->bpp - 1;
  return data;
}
