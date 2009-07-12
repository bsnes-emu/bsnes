#ifdef SYSTEM_CPP

serializer System::serialize() {
  serializer s(1024 * 1024);

  unsigned signature = 0x31545342, version = 1, crc32 = cartridge.crc32();
  s.integer(signature);
  s.integer(version);
  s.integer(crc32);

  serialize_all(s);
  return s;
}

bool System::unserialize(serializer &s) {
  unsigned signature, version, crc32;
  s.integer(signature);
  s.integer(version);
  s.integer(crc32);

  if(signature != 0x31545342) return false;
  if(version != 1) return false;
  if(crc32 != cartridge.crc32()) return false;
  scheduler.init();

  serialize_all(s);
  return true;
}

//internal

void System::serialize(serializer &s) {
  s.integer(snes_region);
  s.integer(snes_expansion);

  s.integer(scheduler.clock.cpu_freq);
  s.integer(scheduler.clock.smp_freq);

  s.integer(scheduler.clock.cpucop);
  s.integer(scheduler.clock.cpuppu);
  s.integer(scheduler.clock.cpusmp);
  s.integer(scheduler.clock.smpdsp);
}

void System::serialize_all(serializer &s) {
  bus.serialize(s);
  cartridge.serialize(s);
  system.serialize(s);
  cpu.serialize(s);
  smp.serialize(s);
  ppu.serialize(s);
  dsp.serialize(s);

  if(cartridge.has_spc7110()) spc7110.serialize(s);
  if(cartridge.has_obc1())    obc1.serialize(s);
}

#endif
