auto System::serializeInit() -> void {
  serializer s;

  uint signature = 0, version = 0;
  char hash[64], description[512];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);

  serializeAll(s);
  _serializeSize = s.size();
}

auto System::serialize() -> serializer {
  serializer s(_serializeSize);

  uint signature = 0x31545342, version = Info::SerializerVersion;
  char hash[64], description[512];
  memory::copy(&hash, (const char*)cartridge.information.sha256, 64);
  memory::fill(&description, 512);

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);

  serializeAll(s);
  return s;
}

auto System::unserialize(serializer& s) -> bool {
  uint signature, version;
  char hash[64], description[512];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);

  if(signature != 0x31545342) return false;
  if(version != Info::SerializerVersion) return false;

  power();
  serializeAll(s);
  return true;
}

auto System::serializeAll(serializer& s) -> void {
  system.serialize(s);
  cpu.serialize(s);
  ppu.serialize(s);
  apu.serialize(s);
  cartridge.serialize(s);
  iram.serialize(s);
}

auto System::serialize(serializer& s) -> void {
  eeprom.serialize(s);

  s.integer(r.unknown);
  s.integer(r.format);
  s.integer(r.depth);
  s.integer(r.color);
}
