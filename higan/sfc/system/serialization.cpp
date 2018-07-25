auto System::serialize() -> serializer {
  serializer s(serializeSize);

  uint signature = 0x31545342;
  char version[16] = {};
  char description[512] = {};
  memory::copy(&version, (const char*)Emulator::SerializerVersion, Emulator::SerializerVersion.size());

  s.integer(signature);
  s.array(version);
  s.array(description);

  s.boolean(hacks.fastPPU);
  s.boolean(hacks.fastDSP);

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

  s.boolean(hacks.fastPPU);
  s.boolean(hacks.fastDSP);

  settings.fastPPU = hacks.fastPPU;
  settings.fastDSP = hacks.fastDSP;

  power(/* reset = */ false);
  serializeAll(s);
  return true;
}

//internal

auto System::serialize(serializer& s) -> void {
}

auto System::serializeAll(serializer& s) -> void {
  system.serialize(s);
  random.serialize(s);
  cartridge.serialize(s);
  cpu.serialize(s);
  smp.serialize(s);
  ppu.serialize(s);
  dsp.serialize(s);

  if(cartridge.has.ICD) icd.serialize(s);
  if(cartridge.has.MCC) mcc.serialize(s);
  if(cartridge.has.DIP) dip.serialize(s);
  if(cartridge.has.Event) event.serialize(s);
  if(cartridge.has.SA1) sa1.serialize(s);
  if(cartridge.has.SuperFX) superfx.serialize(s);
  if(cartridge.has.ARMDSP) armdsp.serialize(s);
  if(cartridge.has.HitachiDSP) hitachidsp.serialize(s);
  if(cartridge.has.NECDSP) necdsp.serialize(s);
  if(cartridge.has.EpsonRTC) epsonrtc.serialize(s);
  if(cartridge.has.SharpRTC) sharprtc.serialize(s);
  if(cartridge.has.SPC7110) spc7110.serialize(s);
  if(cartridge.has.SDD1) sdd1.serialize(s);
  if(cartridge.has.OBC1) obc1.serialize(s);
  if(cartridge.has.MSU1) msu1.serialize(s);

  if(cartridge.has.BSMemorySlot) bsmemory.serialize(s);
  if(cartridge.has.SufamiTurboSlotA) sufamiturboA.serialize(s);
  if(cartridge.has.SufamiTurboSlotB) sufamiturboB.serialize(s);

  controllerPort1.serialize(s);
  controllerPort2.serialize(s);
  expansionPort.serialize(s);
}

//perform dry-run state save:
//determines exactly how many bytes are needed to save state for this cartridge,
//as amount varies per game (eg different RAM sizes, special chips, etc.)
auto System::serializeInit() -> void {
  serializer s;

  uint signature = 0;
  char version[16] = {};
  char description[512] = {};

  s.integer(signature);
  s.array(version);
  s.array(description);

  s.boolean(hacks.fastPPU);
  s.boolean(hacks.fastDSP);

  serializeAll(s);
  serializeSize = s.size();
}
