void BSXCartridge::serialize(serializer &s) {
  s.array(psram.data(), psram.size());
}
