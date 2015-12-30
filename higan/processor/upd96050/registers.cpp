uPD96050::Flag::operator uint() const {
  return (s1 << 5) + (s0 << 4) + (c << 3) + (z << 2) + (ov1 << 1) + (ov0 << 0);
}

auto uPD96050::Flag::operator=(uint d) -> uint {
  s1 = d & 0x20; s0 = d & 0x10; c = d & 0x08; z = d & 0x04; ov1 = d & 0x02; ov0 = d & 0x01;
  return d;
}

uPD96050::Status::operator uint() const {
  return (rqm << 15) + (usf1 << 14) + (usf0 << 13) + (drs << 12)
       + (dma << 11) + (drc  << 10) + (soc  <<  9) + (sic <<  8)
       + (ei  <<  7) + (p1   <<  1) + (p0   <<  0);
}

auto uPD96050::Status::operator=(uint d) -> uint {
  rqm = d & 0x8000; usf1 = d & 0x4000; usf0 = d & 0x2000; drs = d & 0x1000;
  dma = d & 0x0800; drc  = d & 0x0400; soc  = d & 0x0200; sic = d & 0x0100;
  ei  = d & 0x0080; p1   = d & 0x0002; p0   = d & 0x0001;
  return d;
}
