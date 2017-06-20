auto VDP::VRAM::read(uint15 address) const -> uint16 {
  return memory[address];
}

auto VDP::VRAM::write(uint15 address, uint16 data) -> void {
  memory[address] = data;
  if(address < vdp.sprite.io.attributeAddress) return;
  if(address > vdp.sprite.io.attributeAddress + 319) return;
  vdp.sprite.write(address - vdp.sprite.io.attributeAddress, data);
}

auto VDP::VRAM::readByte(uint16 address) const -> uint8 {
  return read(address >> 1).byte(!address.bit(0));
}

auto VDP::VRAM::writeByte(uint16 address, uint8 data) -> void {
  auto word = read(address >> 1);
  word.byte(!address.bit(0)) = data;
  write(address >> 1, word);
}

auto VDP::VSRAM::read(uint6 address) const -> uint10 {
  if(address >= 40) return 0x0000;
  return memory[address];
}

auto VDP::VSRAM::write(uint6 address, uint10 data) -> void {
  if(address >= 40) return;
  memory[address] = data;
}

auto VDP::CRAM::read(uint6 address) const -> uint9 {
  return memory[address];
}

auto VDP::CRAM::write(uint6 address, uint9 data) -> void {
  memory[address] = data;
}
