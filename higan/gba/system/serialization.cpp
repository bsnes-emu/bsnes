auto System::serialize() -> serializer {
  serializer s(serialize_size);

  uint signature = 0x31545342, version = Info::SerializerVersion;
  char hash[64], description[512];
  memcpy(&hash, (const char*)cartridge.sha256(), 64);
  memset(&description, 0, sizeof description);

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);

  serialize_all(s);
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
  serialize_all(s);
  return true;
}

auto System::serialize(serializer& s) -> void {
  s.integer(bios.size);
  s.integer(bios.mdr);
}

auto System::serialize_all(serializer& s) -> void {
  cartridge.serialize(s);
  system.serialize(s);
  cpu.serialize(s);
  ppu.serialize(s);
  apu.serialize(s);
  player.serialize(s);
}

auto System::serialize_init() -> void {
  serializer s;

  uint signature = 0, version = 0;
  char hash[64], description[512];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);

  serialize_all(s);
  serialize_size = s.size();
}
