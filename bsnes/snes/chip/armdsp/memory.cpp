#ifdef ARMDSP_CPP

uint32 ArmDSP::bus_read(uint32 addr, uint32 size) {
  static auto adjust = [&](uint32 word, uint32 addr, uint32 size) {
    unsigned rotate = (addr & 3) << 3;
    word = (word >> rotate) | (word << (32 - rotate));
    return word & (~0u >> (32 - size));
  };

  static auto memory = [&](const uint8 *memory, uint32 addr, uint32 size) {
    memory += addr & ~3;
    uint32 word = 0;
    word |= *memory++ <<  0;
    word |= *memory++ <<  8;
    word |= *memory++ << 16;
    word |= *memory++ << 24;
    return adjust(word, addr, size);
  };

  switch(addr & 0xe0000000) {
  case 0x00000000: return memory(programROM, addr & 0x1ffff, size);
  case 0x20000000: return adjust(pipeline.fetch.instruction, addr, size);
  case 0x40000000: break;
  case 0x60000000: return adjust(0x40404001, addr, size);
  case 0x80000000: return adjust(pipeline.fetch.instruction, addr, size);
  case 0xa0000000: return memory(dataROM, addr & 0x7fff, size);
  case 0xc0000000: return adjust(pipeline.fetch.instruction, addr, size);
  case 0xe0000000: return memory(programRAM, addr & 0x3fff, size);
  }

  addr &= 0xe000003f;

  if(addr == 0x40000010) {
    if(bridge.cputoarm.ready) {
      bridge.cputoarm.ready = false;
      return adjust(bridge.cputoarm.data, addr, size);
    }
  }

  if(addr == 0x40000020) {
    return adjust(bridge.status(), addr, size);
  }

  return 0u;
}

void ArmDSP::bus_write(uint32 addr, uint32 size, uint32 word) {
  static auto memory = [](uint8 *memory, uint32 addr, uint32 size, uint32 word) {
    switch(size) {
    case Word:
      memory += addr & ~3;
      *memory++ = word >>  0;
      *memory++ = word >>  8;
      *memory++ = word >> 16;
      *memory++ = word >> 24;
      break;
    case Byte:
      memory += addr;
      *memory++ = word >>  0;
      break;
    }
  };

  switch(addr & 0xe0000000) {
  case 0x00000000: return;
  case 0x20000000: return;
  case 0x40000000: break;
  case 0x60000000: return;
  case 0x80000000: return;
  case 0xa0000000: return;
  case 0xc0000000: return;
  case 0xe0000000: return memory(programRAM, addr & 0x3fff, size, word);
  }

  addr &= 0xe000003f;
  word &= 0x000000ff;

  if(addr == 0x40000000) {
    bridge.armtocpu.ready = true;
    bridge.armtocpu.data = word;
    return;
  }

  if(addr == 0x40000010) {
    bridge.signal = true;
    return;
  }

  if(addr == 0x40000020) { bridge.timerlatch = (bridge.timerlatch & 0xffff00) | (word <<  0); return; }
  if(addr == 0x40000024) { bridge.timerlatch = (bridge.timerlatch & 0xff00ff) | (word <<  8); return; }
  if(addr == 0x40000028) { bridge.timerlatch = (bridge.timerlatch & 0x00ffff) | (word << 16); return; }

  if(addr == 0x4000002c) {
    bridge.timer = bridge.timerlatch;
    return;
  }
}

#endif
