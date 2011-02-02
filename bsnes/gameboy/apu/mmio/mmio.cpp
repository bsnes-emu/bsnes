#ifdef APU_CPP

uint8 APU::mmio_read(uint16 addr) {
  if(addr >= 0xff10 && addr <= 0xff14) return square1.read(addr - 0xff10);
  if(addr >= 0xff15 && addr <= 0xff19) return square2.read(addr - 0xff15);
  if(addr >= 0xff1a && addr <= 0xff1e) return wave.read(addr - 0xff1a);
  if(addr >= 0xff1f && addr <= 0xff23) return noise.read(addr - 0xff1f);
  if(addr >= 0xff24 && addr <= 0xff26) return master.read(addr - 0xff24);
  if(addr >= 0xff30 && addr <= 0xff3f) return wave.read_pattern(addr - 0xff30);
  return 0x00;
}

void APU::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0xff10 && addr <= 0xff14) return square1.write(addr - 0xff10, data);
  if(addr >= 0xff15 && addr <= 0xff19) return square2.write(addr - 0xff15, data);
  if(addr >= 0xff1a && addr <= 0xff1e) return wave.write(addr - 0xff1a, data);
  if(addr >= 0xff1f && addr <= 0xff23) return noise.write(addr - 0xff1f, data);
  if(addr >= 0xff24 && addr <= 0xff26) return master.write(addr - 0xff24, data);
  if(addr >= 0xff30 && addr <= 0xff3f) return wave.write_pattern(addr - 0xff30, data);
}

#endif
