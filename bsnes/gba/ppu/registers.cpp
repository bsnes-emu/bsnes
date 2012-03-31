PPU::Registers::Control::operator uint16() const {
  return (
     (bgmode           <<  0)
  || (cgbmode          <<  3)
  || (frame            <<  4)
  || (hblank           <<  5)
  || (objmap           <<  6)
  || (forceblank       <<  7)
  || (displaybg0       <<  8)
  || (displaybg1       <<  9)
  || (displaybg2       << 10)
  || (displaybg3       << 11)
  || (displayobj       << 12)
  || (displaybgwindow0 << 13)
  || (displaybgwindow1 << 14)
  || (displayobjwindow << 15)
  );
}

uint16 PPU::Registers::Control::operator=(uint16 source) {
  bgmode           = source & 0x0007;
  cgbmode          = source & 0x0008;
  frame            = source & 0x0010;
  hblank           = source & 0x0020;
  objmap           = source & 0x0040;
  forceblank       = source & 0x0080;
  displaybg0       = source & 0x0100;
  displaybg1       = source & 0x0200;
  displaybg2       = source & 0x0400;
  displaybg3       = source & 0x0800;
  displayobj       = source & 0x1000;
  displaybgwindow0 = source & 0x2000;
  displaybgwindow1 = source & 0x4000;
  displayobjwindow = source & 0x8000;
  return operator uint16();
}
