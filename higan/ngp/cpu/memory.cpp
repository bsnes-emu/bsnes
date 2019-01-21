auto CPU::read(uint24 address) -> uint8 {
  if(address < 0x000100) return 0xff;
  if(address < 0x004000) return 0xff;
  if(address < 0x007000) return cpu.ram.read((uint14)address);
  if(address < 0x008000) return apu.ram.read((uint12)address);
  if(address < 0x00c000) return vpu.ram.read((uint14)address);
  if(address < 0x200000) return 0xff;
  if(address < 0x400000) return cartridge.read(0, (uint21)address);
  if(address < 0x800000) return 0xff;
  if(address < 0xa00000) return cartridge.read(1, (uint21)address);
  if(address < 0xff0000) return 0xff;
                         return system.bios.read((uint16)address);
}

auto CPU::write(uint24 address, uint8 data) -> void {
  if(address < 0x000100) return;
  if(address < 0x004000) return;
  if(address < 0x007000) return cpu.ram.write((uint14)address, data);
  if(address < 0x008000) return apu.ram.write((uint12)address, data);
  if(address < 0x00c000) return vpu.ram.write((uint14)address, data);
  if(address < 0x200000) return;
  if(address < 0x400000) return cartridge.write(0, (uint21)address, data);
  if(address < 0x800000) return;
  if(address < 0xa00000) return cartridge.write(1, (uint21)address, data);
  if(address < 0xff0000) return;
                         return system.bios.write((uint16)address, data);
}
