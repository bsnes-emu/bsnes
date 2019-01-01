auto System::serializeInit() -> void {
  serializer s;

  uint signature = 0;
  char version[16] = {};
  char description[512] = {};

  s.integer(signature);
  s.array(version);
  s.array(description);

  serializeAll(s);
  information.serializeSize = s.size();
}

auto System::serialize() -> serializer {
  serializer s{information.serializeSize};

  uint signature = 0x31545342;
  char version[16] = {};
  char description[512] = {};
  memory::copy(&version, (const char*)Emulator::SerializerVersion, Emulator::SerializerVersion.size());

  s.integer(signature);
  s.array(version);
  s.array(description);

  serializeAll(s);
  return s;
}

auto System::unserialize(serializer& s) -> bool {
  uint signature = 0;
  char version[16] = {};
  char description[512] = {};

  s.integer(signature);
  s.array(version);
  s.array(description);

  if(signature != 0x31545342) return false;
  if(string{version} != Emulator::SerializerVersion) return false;

  power();
  serializeAll(s);
  return true;
}

auto System::serializeAll(serializer& s) -> void {
  system.serialize(s);
  cartridge.serialize(s);
  cpu.serialize(s);
  vdp.serialize(s);
  psg.serialize(s);
}

auto System::serialize(serializer& s) -> void {
}
