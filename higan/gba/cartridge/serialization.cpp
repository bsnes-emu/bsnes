auto Cartridge::serialize(serializer& s) -> void {
  mrom.serialize(s);
  if(hasSRAM) sram.serialize(s);
  if(hasEEPROM) eeprom.serialize(s);
  if(hasFLASH) flash.serialize(s);
}

auto Cartridge::MROM::serialize(serializer& s) -> void {
  s.integer(size);
  s.integer(mask);
}

auto Cartridge::SRAM::serialize(serializer& s) -> void {
  s.array(data, size);
  s.integer(size);
  s.integer(mask);
}

auto Cartridge::EEPROM::serialize(serializer& s) -> void {
  s.array(data, size);
  s.integer(size);
  s.integer(mask);
  s.integer(test);
  s.integer(bits);
  s.integer((uint&)mode);
  s.integer(offset);
  s.integer(address);
  s.integer(addressbits);
}

auto Cartridge::FLASH::serialize(serializer& s) -> void {
  s.array(data, size);
  s.integer(size);
  s.integer(id);
  s.integer(unlockhi);
  s.integer(unlocklo);
  s.integer(idmode);
  s.integer(erasemode);
  s.integer(bankselect);
  s.integer(writeselect);
  s.integer(bank);
}
