CPU::Registers::DMAControl::operator uint16() const {
  return (
    (targetmode <<  5)
  | (sourcemode <<  7)
  | (repeat     <<  9)
  | (size       << 10)
  | (drq        << 11)
  | (timingmode << 12)
  | (irq        << 14)
  | (enable     << 15)
  );
}

uint16 CPU::Registers::DMAControl::operator=(uint16 source) {
  targetmode = source >>  5;
  sourcemode = source >>  7;
  repeat     = source >>  9;
  size       = source >> 10;
  drq        = source >> 11;
  timingmode = source >> 12;
  irq        = source >> 14;
  enable     = source >> 15;
  return operator uint16();
}

unsigned CPU::Registers::TimerControl::multiplier() const {
  static unsigned multiplier[] = { 1, 64, 256, 1024 };
  return multiplier[frequency];
}

CPU::Registers::TimerControl::operator uint8() const {
  return (
    (frequency << 0)
  | (cascade   << 2)
  | (irq       << 6)
  | (enable    << 7)
  );
}

uint8 CPU::Registers::TimerControl::operator=(uint8 source) {
  frequency = source >> 0;
  cascade   = source >> 2;
  irq       = source >> 6;
  enable    = source >> 7;
  return operator uint8();
}

//return number of clocks before counter overflow
signed CPU::Registers::Timer::period() const {
  return (65536 - reload) * control.multiplier() + counter;
}

CPU::Registers::KeypadControl::operator uint16() const {
  return (
    (a         <<  0)
  | (b         <<  1)
  | (select    <<  2)
  | (start     <<  3)
  | (right     <<  4)
  | (left      <<  5)
  | (up        <<  6)
  | (down      <<  7)
  | (r         <<  8)
  | (l         <<  9)
  | (enable    << 14)
  | (condition << 15)
  );
}

uint16 CPU::Registers::KeypadControl::operator=(uint16 source) {
  a         = (source >>  0) & 1;
  b         = (source >>  1) & 1;
  select    = (source >>  2) & 1;
  start     = (source >>  3) & 1;
  right     = (source >>  4) & 1;
  left      = (source >>  5) & 1;
  up        = (source >>  6) & 1;
  down      = (source >>  7) & 1;
  r         = (source >>  8) & 1;
  l         = (source >>  9) & 1;
  enable    = (source >> 14) & 1;
  condition = (source >> 15) & 1;
  return operator uint16();
}

CPU::Registers::Interrupt::operator uint16() const {
  return (
    (vblank       <<  0)
  | (hblank       <<  1)
  | (vcoincidence <<  2)
  | (timer[0]     <<  3)
  | (timer[1]     <<  4)
  | (timer[2]     <<  5)
  | (timer[3]     <<  6)
  | (serial       <<  7)
  | (dma[0]       <<  8)
  | (dma[1]       <<  9)
  | (dma[2]       << 10)
  | (dma[3]       << 11)
  | (keypad       << 12)
  | (cartridge    << 13)
  );
}

uint16 CPU::Registers::Interrupt::operator=(uint16 source) {
  vblank       = source & (1 <<  0);
  hblank       = source & (1 <<  1);
  vcoincidence = source & (1 <<  2);
  timer[0]     = source & (1 <<  3);
  timer[1]     = source & (1 <<  4);
  timer[2]     = source & (1 <<  5);
  timer[3]     = source & (1 <<  6);
  serial       = source & (1 <<  7);
  dma[0]       = source & (1 <<  8);
  dma[1]       = source & (1 <<  9);
  dma[2]       = source & (1 << 10);
  dma[3]       = source & (1 << 11);
  keypad       = source & (1 << 12);
  cartridge    = source & (1 << 13);
  return operator uint16();
}

CPU::Registers::WaitControl::operator uint16() const {
  return (
    (sram     <<  0)
  | (wait0n   <<  2)
  | (wait0s   <<  4)
  | (wait1n   <<  5)
  | (wait1s   <<  7)
  | (wait2n   <<  8)
  | (wait2s   << 10)
  | (phi      << 11)
  | (prefetch << 14)
  | (gametype << 15)
  );
}

uint16 CPU::Registers::WaitControl::operator=(uint16 source) {
  sram     = (source >>  0) & 3;
  wait0n   = (source >>  2) & 3;
  wait0s   = (source >>  4) & 1;
  wait1n   = (source >>  5) & 3;
  wait1s   = (source >>  7) & 1;
  wait2n   = (source >>  8) & 3;
  wait2s   = (source >> 10) & 1;
  phi      = (source >> 11) & 3;
  prefetch = (source >> 14) & 1;
  gametype = (source >> 15) & 1;
  return operator uint16();
}

CPU::Registers::MemoryControl::operator uint32() const {
  return (
    (disable   <<  0)
  | (unknown1  <<  1)
  | (ewram     <<  5)
  | (ewramwait << 24)
  | (unknown2  << 28)
  );
}

uint32 CPU::Registers::MemoryControl::operator=(uint32 source) {
  disable   = (source >>  0) &  1;
  unknown1  = (source >>  1) &  7;
  ewram     = (source >>  5) &  1;
  ewramwait = (source >> 24) & 15;
  unknown2  = (source >> 28) & 15;
  return operator uint32();
}
