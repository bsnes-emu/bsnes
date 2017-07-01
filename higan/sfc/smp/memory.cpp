alwaysinline auto SMP::ramRead(uint16 addr) -> uint8 {
  if(addr >= 0xffc0 && io.iplromEnable) return iplrom[addr & 0x3f];
  if(io.ramDisable) return 0x5a;  //0xff on mini-SNES
  return dsp.apuram[addr];
}

alwaysinline auto SMP::ramWrite(uint16 addr, uint8 data) -> void {
  //writes to $ffc0-$ffff always go to apuram, even if the iplrom is enabled
  if(io.ramWritable && !io.ramDisable) dsp.apuram[addr] = data;
}

auto SMP::portRead(uint2 port) const -> uint8 {
  if(port == 0) return io.cpu0;
  if(port == 1) return io.cpu1;
  if(port == 2) return io.cpu2;
  if(port == 3) return io.cpu3;
  unreachable;
}

auto SMP::portWrite(uint2 port, uint8 data) -> void {
  if(port == 0) io.apu0 = data;
  if(port == 1) io.apu1 = data;
  if(port == 2) io.apu2 = data;
  if(port == 3) io.apu3 = data;
}

auto SMP::busRead(uint16 addr) -> uint8 {
  uint result;

  switch(addr) {
  case 0xf0:  //TEST (write-only register)
    return 0x00;

  case 0xf1:  //CONTROL (write-only register)
    return 0x00;

  case 0xf2:  //DSPADDR
    return io.dspAddr;

  case 0xf3:  //DSPDATA
    //0x80-0xff are read-only mirrors of 0x00-0x7f
    return dsp.read(io.dspAddr & 0x7f);

  case 0xf4:  //CPUIO0
    synchronize(cpu);
    return io.apu0;

  case 0xf5:  //CPUIO1
    synchronize(cpu);
    return io.apu1;

  case 0xf6:  //CPUIO2
    synchronize(cpu);
    return io.apu2;

  case 0xf7:  //CPUIO3
    synchronize(cpu);
    return io.apu3;

  case 0xf8:  //AUXIO4
    return io.aux4;

  case 0xf9:  //AUXIO5
    return io.aux5;

  case 0xfa:  //T0TARGET
  case 0xfb:  //T1TARGET
  case 0xfc:  //T2TARGET (write-only registers)
    return 0x00;

  case 0xfd:  //T0OUT (4-bit counter value)
    result = timer0.stage3;
    timer0.stage3 = 0;
    return result;

  case 0xfe:  //T1OUT (4-bit counter value)
    result = timer1.stage3;
    timer1.stage3 = 0;
    return result;

  case 0xff:  //T2OUT (4-bit counter value)
    result = timer2.stage3;
    timer2.stage3 = 0;
    return result;
  }

  return ramRead(addr);
}

auto SMP::busWrite(uint16 addr, uint8 data) -> void {
  switch(addr) {
  case 0xf0:  //TEST
    if(r.p.p) break;  //writes only valid when P flag is clear

    io.timersDisable = data.bit (0);
    io.ramWritable   = data.bit (1);
    io.ramDisable    = data.bit (2);
    io.timersEnable  = data.bit (3);
    io.ramSpeed      = data.bits(4,5);
    io.romIOSpeed    = data.bits(6,7);

    io.timerStep = (1 << io.romIOSpeed) + (2 << io.ramSpeed);

    timer0.synchronizeStage1();
    timer1.synchronizeStage1();
    timer2.synchronizeStage1();
    break;

  case 0xf1:  //CONTROL
    //0->1 transistion resets timers
    if(!timer0.enable && data.bit(0)) {
      timer0.stage2 = 0;
      timer0.stage3 = 0;
    }
    timer0.enable = data.bit(0);

    if(!timer1.enable && data.bit(1)) {
      timer1.stage2 = 0;
      timer1.stage3 = 0;
    }
    timer1.enable = data.bit(1);

    if(!timer2.enable && data.bit(2)) {
      timer2.stage2 = 0;
      timer2.stage3 = 0;
    }
    timer2.enable = data.bit(2);

    if(data.bit(4)) {
      synchronize(cpu);
      io.apu0 = 0x00;
      io.apu1 = 0x00;
    }

    if(data.bit(5)) {
      synchronize(cpu);
      io.apu2 = 0x00;
      io.apu3 = 0x00;
    }

    io.iplromEnable = data.bit(7);
    break;

  case 0xf2:  //DSPADDR
    io.dspAddr = data;
    break;

  case 0xf3:  //DSPDATA
    if(io.dspAddr & 0x80) break;  //0x80-0xff are read-only mirrors of 0x00-0x7f
    dsp.write(io.dspAddr & 0x7f, data);
    break;

  case 0xf4:  //CPUIO0
    synchronize(cpu);
    io.cpu0 = data;
    break;

  case 0xf5:  //CPUIO1
    synchronize(cpu);
    io.cpu1 = data;
    break;

  case 0xf6:  //CPUIO2
    synchronize(cpu);
    io.cpu2 = data;
    break;

  case 0xf7:  //CPUIO3
    synchronize(cpu);
    io.cpu3 = data;
    break;

  case 0xf8:  //AUXIO4
    io.aux4 = data;
    break;

  case 0xf9:  //AUXIO5
    io.aux5 = data;
    break;

  case 0xfa:  //T0TARGET
    timer0.target = data;
    break;

  case 0xfb:  //T1TARGET
    timer1.target = data;
    break;

  case 0xfc:  //T2TARGET
    timer2.target = data;
    break;

  case 0xfd:  //T0OUT
  case 0xfe:  //T1OUT
  case 0xff:  //T2OUT (read-only registers)
    break;
  }

  ramWrite(addr, data);  //all writes, even to I/O registers, appear on bus
}

auto SMP::speed(uint16 addr) const -> uint {
  static const uint waitStates[4] = {1, 2, 5, 10};
  if((addr & 0xfff0) == 0x00f0) return waitStates[io.romIOSpeed];
  if(addr >= 0xffc0 && io.iplromEnable) return waitStates[io.romIOSpeed];
  return waitStates[io.ramSpeed];
}

auto SMP::idle(uint16 addr, bool read) -> void {
  step(24 * speed(addr));
  if(read) busRead(addr);
  cycleEdge();
}

auto SMP::read(uint16 addr) -> uint8 {
  step(24 * speed(addr));
  uint8 data = busRead(addr);
  cycleEdge();
  return data;
}

auto SMP::write(uint16 addr, uint8 data) -> void {
  step(24 * speed(addr));
  busWrite(addr, data);
  cycleEdge();
}

auto SMP::readDisassembler(uint16 addr) -> uint8 {
  if((addr & 0xfff0) == 0x00f0) return 0x00;
  if(addr >= 0xffc0 && io.iplromEnable) return iplrom[addr & 0x3f];
  return dsp.apuram[addr];
}
