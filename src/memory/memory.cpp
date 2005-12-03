#include "../base.h"

uint16 Memory::read_word(uint32 addr, uint wrap) {
uint16 r;
  switch(wrap) {
  case WRAP_NONE:
    r  = read(addr);
    r |= read(addr + 1) << 8;
    break;
  case WRAP_BANK:
    r  = read(addr);
    r |= read((addr & 0xff0000) | ((addr + 1) & 0xffff)) << 8;
    break;
  case WRAP_PAGE:
    r  = read(addr);
    r |= read((addr & 0xffff00) | ((addr + 1) & 0xff)) << 8;
    break;
  }
  return r;
}

void Memory::write_word(uint32 addr, uint16 data, uint wrap) {
  switch(wrap) {
  case WRAP_NONE:
    write(addr, data);
    write(addr + 1, data >> 8);
    return;
  case WRAP_BANK:
    write(addr, data);
    write((addr & 0xff0000) | ((addr + 1) & 0xffff), data >> 8);
    return;
  case WRAP_PAGE:
    write(addr, data);
    write((addr & 0xffff00) | ((addr + 1) & 0xff), data >> 8);
    return;
  }
}

uint32 Memory::read_long(uint32 addr, uint wrap) {
uint32 r;
  switch(wrap) {
  case WRAP_NONE:
    r  = read(addr);
    r |= read(addr + 1) <<  8;
    r |= read(addr + 2) << 16;
    break;
  case WRAP_BANK:
    r  = read(addr);
    r |= read((addr & 0xff0000) | ((addr + 1) & 0xffff)) <<  8;
    r |= read((addr & 0xff0000) | ((addr + 2) & 0xffff)) << 16;
    break;
  case WRAP_PAGE:
    r  = read(addr);
    r |= read((addr & 0xffff00) | ((addr + 1) & 0xff)) <<  8;
    r |= read((addr & 0xffff00) | ((addr + 2) & 0xff)) << 16;
    break;
  }
  return r;
}

void Memory::write_long(uint32 addr, uint32 data, uint wrap) {
  switch(wrap) {
  case WRAP_NONE:
    write(addr, data);
    write(addr + 1, data >>  8);
    write(addr + 2, data >> 16);
    return;
  case WRAP_BANK:
    write(addr, data);
    write((addr & 0xff0000) | ((addr + 1) & 0xffff), data >>  8);
    write((addr & 0xff0000) | ((addr + 2) & 0xffff), data >> 16);
    return;
  case WRAP_PAGE:
    write(addr, data);
    write((addr & 0xffff00) | ((addr + 1) & 0xff), data >>  8);
    write((addr & 0xffff00) | ((addr + 2) & 0xff), data >> 16);
    return;
  }
}

MMIO  mmio_unmapped;
uint8 MMIO::read (uint32 addr) { return r_cpu->regs.mdr; }
void  MMIO::write(uint32 addr, uint8 value) {}

uint8 MemBus::calc_speed(uint32 addr, bool fast) {
  if((addr & 0xc00000) == 0x400000)return 8;
  if((addr & 0x808000) == 0x808000)return fast?6:8;
  if((addr & 0xc00000) == 0xc00000)return fast?6:8;
  if((addr & 0xe000) == 0x2000)return 6;
  if((addr & 0xfe00) == 0x4000)return 12;
  if((addr & 0xe000) == 0x4000)return 6;
  return 8;
}

void MemBus::set_speed(bool fast) {
  fastROM = fast;

  if(fastROM) {
    speed_table = (uint8*)speed_table_fastrom;
  } else {
    speed_table = (uint8*)speed_table_slowrom;
  }
}

void MemBus::flush_mmio_mappers() {
  for(int i=0;i<0x4000;i++) {
    mmio[i] = &mmio_unmapped;
  }
}

bool MemBus::set_mmio_mapper(uint16 addr, MMIO *mapper) {
//out of range?
  if(addr < 0x2000 || addr >= 0x6000)return false;

  mmio[(addr - 0x2000) & 0x3fff] = mapper;
  return true;
}

MemBus::MemBus() {
int i;
  fastROM = false;
  flush_mmio_mappers();

  for(i=0;i<32768;i++) {
    speed_table_slowrom[i] = calc_speed(i << 9, false);
    speed_table_fastrom[i] = calc_speed(i << 9, true);
  }
  speed_table = (uint8*)speed_table_slowrom;
}
