void bMemBus::cart_map_range(
  uint8 bank_lo, uint8 bank_hi,
  uint8 page_lo, uint8 page_hi,
  uint type, uint offset
) {
uint8 *data  = 0;
uint   size  = 0;
uint   index = 0;

  switch(type) {

  case MAP_ROM: {
    data = cartridge.rom;
    size = cartridge.info.rom_size;
  } break;

  case MAP_RAM: {
    data = cartridge.sram;
    size = cartridge.info.ram_size;
  } break;

  }

  if(size) { index = (index + offset) % size; }

  for(uint bank = bank_lo; bank <= bank_hi; bank++) {
    for(uint page = page_lo; page <= page_hi; page++) {
    uint16 n = (bank << 8) + page;

      page_handle[n] = data + index;
      if(size) { index = (index + 256) % size; }

      switch(type) {

      case MAP_ROM: {
        page_read [n] = &bMemBus::read_rom;
        page_write[n] = &bMemBus::write_rom;
      } break;

      case MAP_RAM: {
        page_read [n] = &bMemBus::read_ram;
        page_write[n] = &bMemBus::write_ram;
      } break;

      }
    }
  }
}

#define mapper(name) void bMemBus::cart_map_##name()
#define map cart_map_range

#include "mapper_pcb.cpp"

#undef cart_map_range
#undef mapper
