#ifdef SYSTEM_CPP

serializer System::serialize() {
  serializer s(serialize_size);

  unsigned signature = 0x31545342, version = Info::SerializerVersion;
  char hash[64], description[512], profile[16];
  memcpy(&hash, (const char*)cartridge.sha256(), 64);
  memset(&description, 0, sizeof description);
  memset(&profile, 0, sizeof profile);
  strcpy(profile, Emulator::Profile);

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);
  s.array(profile);

  serialize_all(s);
  return s;
}

bool System::unserialize(serializer& s) {
  unsigned signature, version;
  char hash[64], description[512], profile[16];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(description);
  s.array(profile);

  if(signature != 0x31545342) return false;
  if(version != Info::SerializerVersion) return false;
  if(strcmp(profile, Emulator::Profile)) return false;

  power();
  serialize_all(s);
  return true;
}

//========
//internal
//========

void System::serialize(serializer& s) {
  s.integer((unsigned&)region);
  s.integer((unsigned&)expansion);
}

void System::serialize_all(serializer& s) {
  cartridge.serialize(s);
  system.serialize(s);
  random.serialize(s);
  cpu.serialize(s);
  smp.serialize(s);
  ppu.serialize(s);
  dsp.serialize(s);

  if(cartridge.hasICD2()) icd2.serialize(s);
  if(cartridge.hasMCC()) mcc.serialize(s);
  if(cartridge.hasEvent()) event.serialize(s);
  if(cartridge.hasSA1()) sa1.serialize(s);
  if(cartridge.hasSuperFX()) superfx.serialize(s);
  if(cartridge.hasARMDSP()) armdsp.serialize(s);
  if(cartridge.hasHitachiDSP()) hitachidsp.serialize(s);
  if(cartridge.hasNECDSP()) necdsp.serialize(s);
  if(cartridge.hasEpsonRTC()) epsonrtc.serialize(s);
  if(cartridge.hasSharpRTC()) sharprtc.serialize(s);
  if(cartridge.hasSPC7110()) spc7110.serialize(s);
  if(cartridge.hasSDD1()) sdd1.serialize(s);
  if(cartridge.hasOBC1()) obc1.serialize(s);
  if(cartridge.hasMSU1()) msu1.serialize(s);

  if(cartridge.hasSufamiTurboSlots()) sufamiturboA.serialize(s), sufamiturboB.serialize(s);
}

//perform dry-run state save:
//determines exactly how many bytes are needed to save state for this cartridge,
//as amount varies per game (eg different RAM sizes, special chips, etc.)
void System::serialize_init() {
  serializer s;

  unsigned signature = 0, version = 0;
  char hash[64], profile[16], description[512];

  s.integer(signature);
  s.integer(version);
  s.array(hash);
  s.array(profile);
  s.array(description);

  serialize_all(s);
  serialize_size = s.size();
}

#endif
