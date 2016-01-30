auto CPU::ramRead(uint16 addr) -> uint8 {
  if(WS() && addr >= 0x4000) return 0x90;
  return iram[addr];
}

auto CPU::ramWrite(uint16 addr, uint8 data) -> void {
  if(WS() && addr >= 0x4000) return;
  iram[addr] = data;
}

auto CPU::portRead(uint16 addr) -> uint8 {
  //HW_FLAGS
  if(addr == 0x00a0) {
    return 1 << 7      //1 = built-in self-test passed
         | 1 << 2      //0 = 8-bit bus width; 1 = 16-bit bus width
         | !WS() << 1  //0 = WonderSwan; 1 = WonderSwan Color or SwanCrystal
         | 1 << 0;     //0 = BIOS mapped; 1 = cartridge mapped
  }

  return 0x00;  //should never occur
}

auto CPU::portWrite(uint16 addr, uint8 data) -> void {
  //HW_FLAGS
  if(addr == 0x00a0) {
    //todo: d2 (bus width) bit is writable; but ... it will do very bad things
  }
}
