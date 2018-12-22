auto CPU::readColeco(uint16 address) -> uint8 {
  uint8 data = 0xff;
  if(address >= 0x0000 && address <= 0x1fff && coleco.replaceBIOS) return expansion.read(address);
  if(address >= 0x2000 && address <= 0x7fff && coleco.replaceRAM ) return expansion.read(address);
  if(address >= 0x0000 && address <= 0x1fff) return system.bios[address & 0x1fff];
  if(address >= 0x6000 && address <= 0x7fff) return ram.read(address - 0x6000);
  if(address >= 0x8000 && address <= 0xffff) return cartridge.rom.read(address - 0x8000);
  return data;
}

auto CPU::writeColeco(uint16 address, uint8 data) -> void {
  if(address >= 0x0000 && address <= 0x1fff && coleco.replaceBIOS) return expansion.write(address, data);
  if(address >= 0x2000 && address <= 0x7fff && coleco.replaceRAM)  return expansion.write(address, data);
  if(address >= 0x6000 && address <= 0x7fff) return ram.write(address - 0x6000, data);
  if(address >= 0x8000 && address <= 0xffff) return;
}

auto CPU::inColeco(uint8 address) -> uint8 {
  uint8 data = 0xff;
  if(address >= 0xa0 && address <= 0xbf) return !address.bit(0) ? vdp.data() : vdp.status();
  if(address >= 0xe0 && address <= 0xff && address.bit(1) == 0) return controllerPort1.device->readData();
  if(address >= 0xe0 && address <= 0xff && address.bit(1) == 1) return controllerPort2.device->readData();
  return data;
}

auto CPU::outColeco(uint8 address, uint8 data) -> void {
  if(address == 0x53) coleco.replaceRAM  = data.bit(0);
  if(address == 0x7f) coleco.replaceBIOS = data.bit(1);
  if(address >= 0x80 && address <= 0x9f) controllerPort1.device->writeData(0), controllerPort2.device->writeData(0);
  if(address >= 0xa0 && address <= 0xbf) return !address.bit(0) ? vdp.data(data) : vdp.control(data);
  if(address >= 0xc0 && address <= 0xdf) controllerPort1.device->writeData(1), controllerPort2.device->writeData(1);
  if(address >= 0xe0 && address <= 0xff) return psg.write(data);
}
