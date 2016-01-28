auto CPU::ramRead(uint16 addr) -> uint8 {
  if(WS() && addr >= 0x4000) return 0x90;
  return iram[addr];
}

auto CPU::ramWrite(uint16 addr, uint8 data) -> void {
  if(WS() && addr >= 0x4000) return;
  iram[addr] = data;
}

auto CPU::portRead(uint16 addr) -> uint16 {
  return 0x0000;
}

auto CPU::portWrite(uint16 addr, uint16 data) -> void {
}
