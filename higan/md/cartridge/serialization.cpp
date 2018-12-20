auto Cartridge::serialize(serializer& s) -> void {
  if(ram.size) s.array(ram.data, ram.size);
  s.integer(ramEnable);
  s.integer(ramWritable);
  s.array(romBank);
  s.boolean(gameGenie.enable);
  for(auto& code : gameGenie.codes) {
    s.boolean(code.enable);
    s.integer(code.address);
    s.integer(code.data);
  }
  if(slot) slot->serialize(s);
}
