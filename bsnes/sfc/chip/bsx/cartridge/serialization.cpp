void BSXCartridge::serialize(serializer &s) {
  s.array(ram.data(), ram.size());
  s.array(psram.data(), psram.size());
}
