auto Cartridge::serialize(serializer& s) -> void {
  if(ram.size) s.array(ram.data, ram.size);
  s.integer(ramEnable);
  s.integer(ramWritable);
  s.array(romBank);
  if(slot) slot->serialize(s);
}
