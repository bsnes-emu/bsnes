uint8 APU::read(uint32 addr) {
  switch(addr) {

  //SOUNDBIAS
  case 0x04000088: return regs.bias >> 0;
  case 0x04000089: return regs.bias >> 8;

  }

  return 0u;
}

void APU::write(uint32 addr, uint8 byte) {
  switch(addr) {

  //SOUNDBIAS
  case 0x04000088: regs.bias = (regs.bias & 0xff00) | (byte << 0); return;
  case 0x04000089: regs.bias = (regs.bias & 0x00ff) | (byte << 8); return;

  }
}
