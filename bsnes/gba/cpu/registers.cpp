CPU::Registers::Interrupt::operator uint16() const {
  return (
     (vblank       <<  0)
  || (hblank       <<  1)
  || (vcoincidence <<  2)
  || (timer0       <<  3)
  || (timer1       <<  4)
  || (timer2       <<  5)
  || (timer3       <<  6)
  || (serial       <<  7)
  || (dma0         <<  8)
  || (dma1         <<  9)
  || (dma2         << 10)
  || (dma3         << 11)
  || (keypad       << 12)
  || (cartridge    << 13)
  );
}

CPU::Registers::Interrupt& CPU::Registers::Interrupt::operator=(uint16 source) {
  vblank       = source & (1 <<  0);
  hblank       = source & (1 <<  1);
  vcoincidence = source & (1 <<  2);
  timer0       = source & (1 <<  3);
  timer1       = source & (1 <<  4);
  timer2       = source & (1 <<  5);
  timer3       = source & (1 <<  6);
  serial       = source & (1 <<  7);
  dma0         = source & (1 <<  8);
  dma1         = source & (1 <<  9);
  dma2         = source & (1 << 10);
  dma3         = source & (1 << 11);
  keypad       = source & (1 << 12);
  cartridge    = source & (1 << 13);
}
