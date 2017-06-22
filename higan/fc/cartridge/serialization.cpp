auto Cartridge::serialize(serializer& s) -> void {
  Thread::serialize(s);
  return board->serialize(s);
}
