auto Cartridge::read(uint16 addr) -> maybe<uint8> {
  uint2 page = addr >> 14;
  addr &= 0x3fff;

  switch(page) {

  case 0: {
    if(addr <= 0x03ff) return rom.read(addr);
    return rom.read(mapper.romPage0 << 14 | addr);
  }

  case 1: {
    return rom.read(mapper.romPage1 << 14 | addr);
  }

  case 2: {
    if(mapper.ramEnablePage2) {
      return ram.read(mapper.ramPage2 << 14 | addr);
    }

    return rom.read(mapper.romPage2 << 14 | addr);
  }

  case 3: {
    if(mapper.ramEnablePage3) {
      return ram.read(addr);
    }

    return nothing;
  }

  }

  unreachable;
}

auto Cartridge::write(uint16 addr, uint8 data) -> bool {
  if(addr == 0xfffc) {
    mapper.shift = data.bits(0,1);
    mapper.ramPage2 = data.bit(2);
    mapper.ramEnablePage2 = data.bit(3);
    mapper.ramEnablePage3 = data.bit(4);
    mapper.romWriteEnable = data.bit(7);
  }

  if(addr == 0xfffd) {
    mapper.romPage0 = data;
  }

  if(addr == 0xfffe) {
    mapper.romPage1 = data;
  }

  if(addr == 0xffff) {
    mapper.romPage2 = data;
  }

  uint2 page = addr >> 14;
  addr &= 0x3fff;

  switch(page) {

  case 0: {
    return false;
  }

  case 1: {
    return false;
  }

  case 2: {
    if(mapper.ramEnablePage2) {
      ram.write(mapper.ramPage2 << 14 | addr, data);
      return true;
    }

    return false;
  }

  case 3: {
    if(mapper.ramEnablePage3) {
      ram.write(addr, data);
      return true;
    }

    return false;
  }

  }

  unreachable;
}
