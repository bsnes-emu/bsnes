auto CPU::readByte(uint24 addr) -> uint16 {
  if(addr >= 0x000000 && addr <= 0x3fffff) {
    if(!io.romEnable) return tmss[addr & 0x7ff];
    return cartridge.read(addr & ~1).byte(!addr.bit(0));
  }
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return apu.granted() ? apu.read(addr) : (uint8)0x00;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return readIO(addr & ~0xff00);
  if(addr >= 0xa11000 && addr <= 0xa11fff) return readIO(addr & ~0x00ff);
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.readIO(addr);
  if(addr >= 0xc00000 && addr <= 0xdfffff) return vdp.read(addr & ~1).byte(!addr.bit(0));
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    return ram[addr & 0xffff];
  }
  return 0x0000;
}

auto CPU::readWord(uint24 addr) -> uint16 {
  if(addr >= 0x000000 && addr <= 0x3fffff) {
    if(!io.romEnable) return tmss[addr & 0x7fe | 0] << 8 | tmss[addr & 0x7fe | 1] << 0;
    return cartridge.read(addr);
  }
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return apu.granted() ? apu.read(addr) : (uint8)0x00;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return readIO(addr & ~0xff00) << 0;
  if(addr >= 0xa11000 && addr <= 0xa11fff) return readIO(addr & ~0x00ff) << 8;
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.readIO(addr);
  if(addr >= 0xc00000 && addr <= 0xdfffff) return vdp.read(addr);
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    uint16 data = ram[addr + 0 & 0xffff] << 8;
    return data | ram[addr + 1 & 0xffff] << 0;
  }
  return 0x0000;
}

auto CPU::writeByte(uint24 addr, uint16 data) -> void {
  if(addr >= 0x000000 && addr <= 0x3fffff) return cartridge.write(addr & ~1, data << 8 | data << 0);
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return apu.granted() ? apu.write(addr, data) : (void)0;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return writeIO(addr & ~0xff00, data);
  if(addr >= 0xa11000 && addr <= 0xa11fff) return writeIO(addr & ~0x00ff, data);
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.writeIO(addr, data);
  if(addr >= 0xa14000 && addr <= 0xa140ff) return writeIO(addr & ~0xff00, data);
  if(addr >= 0xa14100 && addr <= 0xa141ff) return writeIO(addr & ~0x00ff, data);
  if(addr >= 0xc00000 && addr <= 0xc0000f) return vdp.write(addr & ~1, data << 8 | data << 0);
  if(addr >= 0xc00010 && addr <= 0xc00017) return psg.write(data);
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    ram[addr & 0xffff] = data;
    return;
  }
}

auto CPU::writeWord(uint24 addr, uint16 data) -> void {
  if(addr >= 0x000000 && addr <= 0x3fffff) return cartridge.write(addr, data);
  if(addr >= 0xa00000 && addr <= 0xa0ffff) return apu.granted() ? apu.write(addr, data) : (void)0;
  if(addr >= 0xa10000 && addr <= 0xa10fff) return writeIO(addr & ~0xff00, data >> 0);
  if(addr >= 0xa11000 && addr <= 0xa11fff) return writeIO(addr & ~0x00ff, data >> 8);
  if(addr >= 0xa13000 && addr <= 0xa130ff) return cartridge.writeIO(addr, data);
  if(addr >= 0xa14000 && addr <= 0xa140ff) return writeIO(addr & ~0xff00, data >> 0);
  if(addr >= 0xa14100 && addr <= 0xa141ff) return writeIO(addr & ~0x00ff, data >> 8);
  if(addr >= 0xc00000 && addr <= 0xc0000f) return vdp.write(addr, data);
  if(addr >= 0xc00010 && addr <= 0xc00017) return psg.write(data);
  if(addr >= 0xe00000 && addr <= 0xffffff) {
    ram[addr + 0 & 0xffff] = data >> 8;
    ram[addr + 1 & 0xffff] = data >> 0;
    return;
  }
}

auto CPU::readIO(uint24 addr) -> uint16 {
  switch(addr & ~1) {
  case 0xa10000: return (
    !Region::NTSCJ() << 7  //0 = domestic (Japan); 1 = export
  | Region::PAL() << 6     //0 = NTSC; 1 = PAL
  | 1 << 5                 //0 = Mega CD connected; 1 = no expansion connected
  | io.version << 0        //0 = Model 1; 1 = Model 2+
  );

  case 0xa10002: return controllerPort1.device->readData();
  case 0xa10004: return controllerPort2.device->readData();
  case 0xa10006: return extensionPort.device->readData();

  case 0xa10008: return controllerPort1.readControl();
  case 0xa1000a: return controllerPort2.readControl();
  case 0xa1000c: return extensionPort.readControl();

  case 0xa11100: return !apu.granted();
  }

  return 0x0000;
}

auto CPU::writeIO(uint24 addr, uint16 data) -> void {
  switch(addr & ~1) {
  case 0xa10002: return controllerPort1.device->writeData(data);
  case 0xa10004: return controllerPort2.device->writeData(data);
  case 0xa10006: return extensionPort.device->writeData(data);

  case 0xa10008: return controllerPort1.writeControl(data);
  case 0xa1000a: return controllerPort2.writeControl(data);
  case 0xa1000c: return extensionPort.writeControl(data);

  case 0xa11100: return apu.request(data.bit(0));
  case 0xa11200: return apu.enable(data.bit(0));

  case 0xa14000:
    io.vdpEnable[0] = data == 0x5345;
    return;

  case 0xa14002:
    io.vdpEnable[1] = data == 0x4741;
    return;

  case 0xa14100:
    io.romEnable = data.bit(0);
    return;
  }
}
