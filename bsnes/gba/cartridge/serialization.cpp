void Cartridge::serialize(serializer &s) {
  if(has_sram) s.array(ram.data, ram.size);
  if(has_eeprom) s.array(eeprom.data.data(), eeprom.size);
  if(has_flashrom) s.array(flashrom.data, flashrom.size);
}
