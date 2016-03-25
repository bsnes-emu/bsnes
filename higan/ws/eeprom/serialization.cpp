auto EEPROM::serialize(serializer& s) -> void {
  s.array(_data);

  s.integer(r.latch);
  s.integer(r.address);
  s.integer(r.unknown);
  s.integer(r.writeRequested);
  s.integer(r.readRequested);
  s.integer(r.writeCompleted);
  s.integer(r.readCompleted);
  s.integer(r.writeProtect);
}
