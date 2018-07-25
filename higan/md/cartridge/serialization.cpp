auto Cartridge::serialize(serializer& s) -> void {
  if(game.ram.size) s.array(game.ram.data, game.ram.size);
  if(lockOn.ram.size) s.array(lockOn.ram.data, lockOn.ram.size);
}
