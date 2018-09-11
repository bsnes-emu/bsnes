auto Cartridge::MBC7::serialize(serializer& s) -> void {
  eeprom.serialize(s);
  s.integer(io.rom.bank);
  s.integer(io.ram.enable[0]);
  s.integer(io.ram.enable[1]);
  s.integer(io.accelerometer.x);
  s.integer(io.accelerometer.y);
}

auto Cartridge::MBC7::EEPROM::serialize(serializer& s) -> void {
  s.array(data);
  s.integer(size);
  s.integer(width);
  s.boolean(select);
  s.boolean(clock);
  s.boolean(writable);
  s.integer(busy);
  input.serialize(s);
  output.serialize(s);
}

auto Cartridge::MBC7::EEPROM::InputShiftRegister::serialize(serializer& s) -> void {
  s.integer(value);
  s.integer(count);
  s.integer(addressLength);
  s.integer(dataLength);
}

auto Cartridge::MBC7::EEPROM::OutputShiftRegister::serialize(serializer& s) -> void {
  s.integer(value);
  s.integer(count);
}
