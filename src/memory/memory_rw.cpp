#ifdef MEMORY_CPP

uint16 Memory::read_word(unsigned addr, unsigned wrap) {
  uint16 r;
  switch(wrap) {
    case WRAP_NONE: {
      r  = read(addr);
      r |= read(addr + 1) << 8;
    } break;
    case WRAP_BANK: {
      r  = read(addr);
      r |= read((addr & 0xff0000) | ((addr + 1) & 0xffff)) << 8;
    } break;
    case WRAP_PAGE: {
      r  = read(addr);
      r |= read((addr & 0xffff00) | ((addr + 1) & 0xff)) << 8;
    } break;
  }
  return r;
}

void Memory::write_word(unsigned addr, uint16 data, unsigned wrap) {
  switch(wrap) {
    case WRAP_NONE: {
      write(addr, data);
      write(addr + 1, data >> 8);
    } return;
    case WRAP_BANK: {
      write(addr, data);
      write((addr & 0xff0000) | ((addr + 1) & 0xffff), data >> 8);
    } return;
    case WRAP_PAGE: {
      write(addr, data);
      write((addr & 0xffff00) | ((addr + 1) & 0xff), data >> 8);
    } return;
  }
}

uint32 Memory::read_long(unsigned addr, unsigned wrap) {
  uint32 r;
  switch(wrap) {
    case WRAP_NONE: {
      r  = read(addr);
      r |= read(addr + 1) <<  8;
      r |= read(addr + 2) << 16;
    } break;
    case WRAP_BANK: {
      r  = read(addr);
      r |= read((addr & 0xff0000) | ((addr + 1) & 0xffff)) <<  8;
      r |= read((addr & 0xff0000) | ((addr + 2) & 0xffff)) << 16;
    } break;
    case WRAP_PAGE: {
      r  = read(addr);
      r |= read((addr & 0xffff00) | ((addr + 1) & 0xff)) <<  8;
      r |= read((addr & 0xffff00) | ((addr + 2) & 0xff)) << 16;
    } break;
  }
  return r;
}

void Memory::write_long(unsigned addr, uint32 data, unsigned wrap) {
  switch(wrap) {
    case WRAP_NONE: {
      write(addr, data);
      write(addr + 1, data >>  8);
      write(addr + 2, data >> 16);
    } return;
    case WRAP_BANK: {
      write(addr, data);
      write((addr & 0xff0000) | ((addr + 1) & 0xffff), data >>  8);
      write((addr & 0xff0000) | ((addr + 2) & 0xffff), data >> 16);
    } return;
    case WRAP_PAGE: {
      write(addr, data);
      write((addr & 0xffff00) | ((addr + 1) & 0xff), data >>  8);
      write((addr & 0xffff00) | ((addr + 2) & 0xff), data >> 16);
    } return;
  }
}

#endif //ifdef MEMORY_CPP
