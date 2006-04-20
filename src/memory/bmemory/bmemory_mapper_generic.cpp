void bMemBus::cart_map_generic(uint type) {
uint32 P0_size, P1_size, ROM_mask, ROM_size, SRAM_size;
  ROM_size  = cartridge.cart.rom_size;
  SRAM_size = cartridge.cart.sram_size;

//calculate highest power of 2, which is the size of the first ROM chip
  P0_size = 0x800000;
  while(!(ROM_size & P0_size))P0_size >>= 1;
  P1_size = ROM_size - P0_size;

  if(ROM_size == P0_size) {
  //cart only contains one ROM chip
    ROM_mask = (P0_size - 1);
  } else {
  //cart contains two ROM chips
    ROM_mask = (P0_size + P0_size - 1);
  }

  for(uint page = 0x0000; page <= 0xffff; page++) {
    if(memory_type(page << 8) != TYPE_CART)continue;

  uint addr = page << 8;
  uint bank = page >> 8;

  //SRAM mapping is incorrect in several games, this is the most compatible
  //layout I can create using only ROM header information. Additional accuracy
  //requires PCB identification.

  //HiROM SRAM region
  //$[20-3f|a0-bf]:[6000-7fff]
    if((bank & 0x7f) >= 0x20 && (bank & 0x7f) <= 0x3f && (addr & 0xe000) == 0x6000) {
      if(SRAM_size == 0)continue;

      addr  = ((bank & 0x7f) - 0x20) * 0x2000 + ((addr & 0xffff) - 0x6000);
      addr %= SRAM_size;
      page_handle[page] = cartridge.sram + addr;
      page_read  [page] = &bMemBus::read_ram;
      page_write [page] = &bMemBus::write_ram;
      continue;
    }

  //LoROM SRAM region
  //$[70-7f|f0-ff]:[0000-7fff]
  //Note: RAM is remapped over $[7e-7f]:[0000-ffff]
    if((bank & 0x7f) >= 0x70 && (bank & 0x7f) <= 0x7f && (addr & 0x8000) == 0x0000) {
      if(SRAM_size == 0)continue;

      if(type == Cartridge::LOROM || !(bank & 0x80)) {
      //HiROM maps $[f0-ff]:[0000-7fff] to ROM
        addr  = ((bank & 0x7f) - 0x70) * 0x8000 + (addr & 0x7fff);
        addr %= SRAM_size;
        page_handle[page] = cartridge.sram + addr;
        page_read  [page] = &bMemBus::read_ram;
        page_write [page] = &bMemBus::write_ram;
        continue;
      }
    }

  //ROM region
    switch(type) {

    case Cartridge::LOROM: {
      addr  = (bank & 0x7f) * 0x8000 + (addr & 0x7fff);
      addr &= ROM_mask;
      if(addr >= P0_size) {
        addr = P0_size + (addr & (P1_size - 1));
      }
    } break;

    case Cartridge::HIROM: {
      addr &= ROM_mask;
      if(addr >= P0_size) {
        addr = P0_size + (addr & (P1_size - 1));
      }
    } break;

    case Cartridge::EXLOROM: {
      if(!(bank & 0x40)) {
        bank &= 0x7f;
        addr  = (bank << 15) | (addr & 0x7fff);
      } else {
        addr &= 0x3fffff;
      }
      addr %= ROM_size;
    } break;

    case Cartridge::EXHIROM: {
      addr &= 0x3fffff;
      addr += (bank < 0x80) ? 0x400000 : 0x000000;
      addr %= ROM_size;
    } break;

    }

    page_handle[page] = cartridge.rom + addr;
    page_read  [page] = &bMemBus::read_rom;
    page_write [page] = &bMemBus::write_rom;
  }
}

void bMemBus::cart_map_sdd1() {
//$[c0-ff]:[0000-ffff]
  for(uint page = 0xc000; page <= 0xffff; page++) {
    page_read [page] = &bMemBus::read_sdd1;
    page_write[page] = &bMemBus::write_sdd1;
  }
}

void bMemBus::cart_map_c4() {
//$[00-3f|80-bf]:[6000-7fff]
  for(uint bank = 0x00; bank <= 0x3f; bank++) {
    for(uint page = 0x60; page <= 0x7f; page++) {
      page_read [0x0000 + (bank << 8) + page] = &bMemBus::read_c4;
      page_read [0x8000 + (bank << 8) + page] = &bMemBus::read_c4;
      page_write[0x0000 + (bank << 8) + page] = &bMemBus::write_c4;
      page_write[0x8000 + (bank << 8) + page] = &bMemBus::write_c4;
    }
  }
}

void bMemBus::cart_map_dsp2() {
//$[20-3f]:[6000-6fff|8000-bfff]
  for(uint bank = 0x20; bank <= 0x3f; bank++) {
    for(uint page = 0x60; page <= 0xbf; page++) {
      if(page >= 0x70 && page <= 0x7f)continue;
      page_read [(bank << 8) + page] = &bMemBus::read_dsp2;
      page_write[(bank << 8) + page] = &bMemBus::write_dsp2;
    }
  }
}

void bMemBus::cart_map_obc1() {
//$[00-3f|80-bf]:[6000-7fff]
  for(uint bank = 0x00; bank <= 0x3f; bank++) {
    for(uint page = 0x60; page <= 0x7f; page++) {
      page_read [0x0000 + (bank << 8) + page] = &bMemBus::read_obc1;
      page_read [0x8000 + (bank << 8) + page] = &bMemBus::read_obc1;
      page_write[0x0000 + (bank << 8) + page] = &bMemBus::write_obc1;
      page_write[0x8000 + (bank << 8) + page] = &bMemBus::write_obc1;
    }
  }
}
