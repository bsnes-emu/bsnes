#ifdef PPU_CPP

void PPUCounter::serialize(serializer &s) {
  s.integer(status.interlace);
  s.integer(status.field);
  s.integer(status.vcounter);
  s.integer(status.hcounter);

  s.array(history.field);
  s.array(history.vcounter);
  s.array(history.hcounter);
  s.integer(history.index);
}

void PPU::serialize(serializer &s) {
  PPUCounter::serialize(s);

  s.integer(status.frames_updated);
  s.integer(status.frames_executed);

  s.integer(ppu1_version);
  s.integer(ppu2_version);
}

#endif
