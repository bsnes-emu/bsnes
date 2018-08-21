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
  command.serialize(s);
  address.serialize(s);
  input.serialize(s);
  output.serialize(s);
}

auto Cartridge::MBC7::EEPROM::ShiftRegister::serialize(serializer& s) -> void {
  s.integer(value);
  s.integer(count);
  s.integer(length);
}
