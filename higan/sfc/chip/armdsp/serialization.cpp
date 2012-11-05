#ifdef ARMDSP_CPP

nall::vector<uint8> ArmDSP::firmware() {
  nall::vector<uint8> buffer;
  if(cartridge.has_armdsp() == false) return buffer;
  buffer.reserve(128 * 1024 + 32 * 1024);
  for(unsigned n = 0; n < 128 * 1024; n++) buffer.append(programROM[n]);
  for(unsigned n = 0; n <  32 * 1024; n++) buffer.append(dataROM[n]);
  return buffer;
}

void ArmDSP::serialize(serializer &s) {
  ARM::serialize(s);
  Thread::serialize(s);

  s.array(programRAM, 16 * 1024);

  s.integer(bridge.cputoarm.ready);
  s.integer(bridge.cputoarm.data);
  s.integer(bridge.armtocpu.ready);
  s.integer(bridge.armtocpu.data);
  s.integer(bridge.timer);
  s.integer(bridge.timerlatch);
  s.integer(bridge.reset);
  s.integer(bridge.ready);
  s.integer(bridge.signal);
}

#endif
