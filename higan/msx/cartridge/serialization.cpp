auto Cartridge::serialize(serializer& s) -> void {
  if(ram) s.array(ram.data(), ram.size());
}
