auto Flash::serialize(serializer& s) -> void {
  rom.serialize(s);
  s.integer(vendorID);
  s.integer(deviceID);
}

auto Cartridge::serialize(serializer& s) -> void {
  flash[0].serialize(s);
  flash[1].serialize(s);
}
