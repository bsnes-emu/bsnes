auto HitachiDSP::isROM(uint24 address) -> bool {
  return (bool)addressROM(address);
}

auto HitachiDSP::isRAM(uint24 address) -> bool {
  return (bool)addressRAM(address);
}

auto HitachiDSP::read(uint24 address) -> uint8 {
  if(auto linear = addressROM (address)) return readROM (*linear);
  if(auto linear = addressRAM (address)) return readRAM (*linear);
  if(auto linear = addressDRAM(address)) return readDRAM(*linear);
  if(auto linear = addressIO  (address)) return readIO  (*linear);
  return 0x00;
}

auto HitachiDSP::write(uint24 address, uint8 data) -> void {
  if(auto linear = addressROM (address)) return writeROM (*linear, data);
  if(auto linear = addressRAM (address)) return writeRAM (*linear, data);
  if(auto linear = addressDRAM(address)) return writeDRAM(*linear, data);
  if(auto linear = addressIO  (address)) return writeIO  (*linear, data);
}

//

auto HitachiDSP::addressROM(uint24 address) const -> maybe<uint24> {
  if(Mapping == 0) {
    //00-3f,80-bf:8000-ffff; c0-ff:0000-ffff
    if((address & 0x408000) == 0x008000 || (address & 0xc00000) == 0xc00000) {
      address = (address & 0x3f0000) >> 1 | (address & 0x7fff);
      return {address & 0x1fffff};
    }
  } else {
    //00-3f,80-bf:8000-ffff; c0-ff:0000-ffff
    if((address & 0x408000) == 0x008000 || (address & 0xc00000) == 0xc00000) {
      return {address & 0x3fffff};
    }
  }
  return {};
}

auto HitachiDSP::readROM(uint24 address, uint8 data) -> uint8 {
  if(hitachidsp.active() || !busy()) {
    address = bus.mirror(address, rom.size());
  //if(Roms == 2 && mmio.r1f52 == 1 && address >= (bit::round(rom.size()) >> 1)) return 0x00;
    return rom.read(address, data);
  }
  //DSP has the bus acquired: CPU reads from 00:ffc0-ffff return IO registers (including reset vector overrides)
  if(Mapping == 0 && (address & 0xbfffc0) == 0x007fc0) return readIO(0x7f40 | address & 0x3f);
  if(Mapping == 1 && (address & 0xbfffc0) == 0x00ffc0) return readIO(0x7f40 | address & 0x3f);
  return data;
}

auto HitachiDSP::writeROM(uint24 address, uint8 data) -> void {
}

//

auto HitachiDSP::addressRAM(uint24 address) const -> maybe<uint24> {
  if(Mapping == 0) {
    //70-77:0000-7fff
    if((address & 0xf88000) == 0x700000) {
      address = (address & 0x070000) >> 1 | (address & 0x7fff);
      return {address & 0x03ffff};
    }
  } else {
    //30-3f,b0-bf:6000-7fff
    if((address & 0x70e000) == 0x306000) {
      address = (address & 0x0f0000) >> 3 | (address & 0x1fff);
      return {address & 0x01ffff};
    }
  }
  return {};
}

auto HitachiDSP::readRAM(uint24 address, uint8 data) -> uint8 {
  if(ram.size() == 0) return 0x00;  //not open bus
  return ram.read(bus.mirror(address, ram.size()), data);
}

auto HitachiDSP::writeRAM(uint24 address, uint8 data) -> void {
  if(ram.size() == 0) return;
  return ram.write(bus.mirror(address, ram.size()), data);
}

//

auto HitachiDSP::addressDRAM(uint24 address) const -> maybe<uint24> {
  if(Mapping == 0) {
    //00-3f,80-bf:6000-6bff,7000-7bff
    if((address & 0x40e000) == 0x006000 && (address & 0x0c00) != 0x0c00) {
      return {address & 0x0fff};
    }
  } else {
    //00-2f,80-af:6000-6bff,7000-7bff
    if((address & 0x40e000) == 0x006000 && (address & 0x0c00) != 0x0c00 && (address & 0x300000) != 0x300000) {
      return {address & 0x0fff};
    }
  }
  return {};
}

auto HitachiDSP::readDRAM(uint24 address, uint8 data) -> uint8 {
  address &= 0xfff;
  if(address >= 0xc00) return data;
  return dataRAM[address];
}

auto HitachiDSP::writeDRAM(uint24 address, uint8 data) -> void {
  address &= 0xfff;
  if(address >= 0xc00) return;
  dataRAM[address] = data;
}

//

auto HitachiDSP::addressIO(uint24 address) const -> maybe<uint24> {
  if(Mapping == 0) {
    //00-3f,80-bf:6c00-6fff,7c00-7fff
    if((address & 0x40ec00) == 0x006c00) {
      return {address & 0x03ff};
    }
  } else {
    //00-2f,80-af:6c00-6fff,7c00-7fff
    if((address & 0x40ec00) == 0x006c00 && (address & 0x300000) != 0x300000) {
      return {address & 0x03ff};
    }
  }
  return {};
}

auto HitachiDSP::readIO(uint24 address, uint8 data) -> uint8 {
  address = 0x7c00 | (address & 0x03ff);

  //IO
  switch(address) {
  case 0x7f40: return io.dma.source.byte(0);
  case 0x7f41: return io.dma.source.byte(1);
  case 0x7f42: return io.dma.source.byte(2);
  case 0x7f43: return io.dma.length.byte(0);
  case 0x7f44: return io.dma.length.byte(1);
  case 0x7f45: return io.dma.target.byte(0);
  case 0x7f46: return io.dma.target.byte(1);
  case 0x7f47: return io.dma.target.byte(2);
  case 0x7f48: return io.cache.page;
  case 0x7f49: return io.cache.base.byte(0);
  case 0x7f4a: return io.cache.base.byte(1);
  case 0x7f4b: return io.cache.base.byte(2);
  case 0x7f4c: return io.cache.lock[0] << 0 | io.cache.lock[1] << 1;
  case 0x7f4d: return io.cache.pb.byte(0);
  case 0x7f4e: return io.cache.pb.byte(1);
  case 0x7f4f: return io.cache.pc;
  case 0x7f50: return io.wait.ram << 0 | io.wait.rom << 4;
  case 0x7f51: return io.irq;
  case 0x7f52: return io.rom;
  case 0x7f53: case 0x7f54: case 0x7f55: case 0x7f56: case 0x7f57:
  case 0x7f59: case 0x7f5b: case 0x7f5c: case 0x7f5d: case 0x7f5e:
  case 0x7f5f: return io.suspend.enable << 0 | r.i << 1 | running() << 6 | busy() << 7;
  }

  //vectors
  if(address >= 0x7f60 && address <= 0x7f7f) {
    return io.vector[address & 0x1f];
  }

  //registers
  if((address >= 0x7f80 && address <= 0x7faf) || (address >= 0x7fc0 && address <= 0x7fef)) {
    address &= 0x3f;
    return r.gpr[address / 3].byte(address % 3);
  }

  return 0x00;
}

auto HitachiDSP::writeIO(uint24 address, uint8 data) -> void {
  address = 0x7c00 | (address & 0x03ff);

  //IO
  switch(address) {
  case 0x7f40: io.dma.source.byte(0) = data; return;
  case 0x7f41: io.dma.source.byte(1) = data; return;
  case 0x7f42: io.dma.source.byte(2) = data; return;

  case 0x7f43: io.dma.length.byte(0) = data; return;
  case 0x7f44: io.dma.length.byte(1) = data; return;

  case 0x7f45: io.dma.target.byte(0) = data; return;
  case 0x7f46: io.dma.target.byte(1) = data; return;
  case 0x7f47: io.dma.target.byte(2) = data;
    if(io.halt) io.dma.enable = 1;
    return;

  case 0x7f48:
    io.cache.page = data.bit(0);
    if(io.halt) io.cache.enable = 1;
    return;

  case 0x7f49: io.cache.base.byte(0) = data; return;
  case 0x7f4a: io.cache.base.byte(1) = data; return;
  case 0x7f4b: io.cache.base.byte(2) = data; return;

  case 0x7f4c:
    io.cache.lock[0] = data.bit(0);
    io.cache.lock[1] = data.bit(1);
    return;

  case 0x7f4d: io.cache.pb.byte(0) = data; return;
  case 0x7f4e: io.cache.pb.byte(1) = data; return;

  case 0x7f4f:
    io.cache.pc = data;
    if(io.halt) {
      io.halt = 0;
      r.pb = io.cache.pb;
      r.pc = io.cache.pc;
    }
    return;

  case 0x7f50:
    io.wait.ram = data.bits(0,2);
    io.wait.rom = data.bits(4,6);
    return;

  case 0x7f51:
    io.irq = data.bit(0);
    if(io.irq == 1) r.i = 0, cpu.r.irq = 0;
    return;

  case 0x7f52:
    io.rom = data.bit(0);
    return;

  case 0x7f53:
    io.lock = 0;
    io.halt = 1;
    return;

  case 0x7f55: io.suspend.enable = 1; io.suspend.duration =   0; return;  //indefinite
  case 0x7f56: io.suspend.enable = 1; io.suspend.duration =  32; return;
  case 0x7f57: io.suspend.enable = 1; io.suspend.duration =  64; return;
  case 0x7f58: io.suspend.enable = 1; io.suspend.duration =  96; return;
  case 0x7f59: io.suspend.enable = 1; io.suspend.duration = 128; return;
  case 0x7f5a: io.suspend.enable = 1; io.suspend.duration = 160; return;
  case 0x7f5b: io.suspend.enable = 1; io.suspend.duration = 192; return;
  case 0x7f5c: io.suspend.enable = 1; io.suspend.duration = 224; return;
  case 0x7f5d: io.suspend.enable = 0; return;  //resume

  case 0x7f5e:
    r.i = 0;  //does not deassert CPU IRQ line
    return;
  }

  //vectors
  if(address >= 0x7f60 && address <= 0x7f7f) {
    io.vector[address & 0x1f] = data;
    return;
  }

  //registers
  if((address >= 0x7f80 && address <= 0x7faf) || (address >= 0x7fc0 && address <= 0x7fef)) {
    address &= 0x3f;
    r.gpr[address / 3].byte(address % 3) = data;
  }
}
