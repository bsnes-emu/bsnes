byte mmio_rspc(byte port) {
#ifndef NO_SPC700
  return cpu_apu_bridge->cpu_read(port);
#else
static byte t = 0, counter = 0;
byte x;
  if(rand() & 1) {
    x = rand() & 7;
    if(x == 0) {
      if(!(port & 1))t = gx816->regs.a.w;
      else           t = gx816->regs.a.w >> 8;
    }
    else if(x == 1) {
      if(!(port & 1))t = gx816->regs.x;
      else           t = gx816->regs.x >> 8;
    }
    else if(x == 2) {
      if(!(port & 1))t = gx816->regs.y;
      else           t = gx816->regs.y >> 8;
    }
    else if(x == 3)t = 0xaa;
    else if(x == 4)t = 0xbb;
    else if(x == 5)t = 0xcc;
    else { t = counter++; }
  }
  return t;
#endif
}

void mmio_wspc(byte port, byte value) {
#ifndef NO_SPC700
  cpu_apu_bridge->cpu_write(port, value);
#endif
}
