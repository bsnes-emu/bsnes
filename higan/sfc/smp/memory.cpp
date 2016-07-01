alwaysinline auto SMP::readRAM(uint16 addr) -> uint8 {
  if(addr >= 0xffc0 && io.iplromEnable) return iplrom[addr & 0x3f];
  if(io.ramDisable) return 0x5a;  //0xff on mini-SNES
  return apuram[addr];
}

alwaysinline auto SMP::writeRAM(uint16 addr, uint8 data) -> void {
  //writes to $ffc0-$ffff always go to apuram, even if the iplrom is enabled
  if(io.ramWritable && !io.ramDisable) apuram[addr] = data;
}

auto SMP::readPort(uint2 port) const -> uint8 {
  return apuram[0xf4 + port];
}

auto SMP::writePort(uint2 port, uint8 data) -> void {
  apuram[0xf4 + port] = data;
}

auto SMP::readBus(uint16 addr) -> uint8 {
  uint result;

  switch(addr) {
  case 0xf0:  //TEST -- write-only register
    return 0x00;

  case 0xf1:  //CONTROL -- write-only register
    return 0x00;

  case 0xf2:  //DSPADDR
    return io.dspAddr;

  case 0xf3:  //DSPDATA
    //0x80-0xff are read-only mirrors of 0x00-0x7f
    return dsp.read(io.dspAddr & 0x7f);

  case 0xf4:  //CPUIO0
  case 0xf5:  //CPUIO1
  case 0xf6:  //CPUIO2
  case 0xf7:  //CPUIO3
    synchronizeCPU();
    return cpu.readPort(addr);

  case 0xf8:  //RAM0
    return io.ram00f8;

  case 0xf9:  //RAM1
    return io.ram00f9;

  case 0xfa:  //T0TARGET
  case 0xfb:  //T1TARGET
  case 0xfc:  //T2TARGET -- write-only registers
    return 0x00;

  case 0xfd:  //T0OUT -- 4-bit counter value
    result = timer0.stage3;
    timer0.stage3 = 0;
    return result;

  case 0xfe:  //T1OUT -- 4-bit counter value
    result = timer1.stage3;
    timer1.stage3 = 0;
    return result;

  case 0xff:  //T2OUT -- 4-bit counter value
    result = timer2.stage3;
    timer2.stage3 = 0;
    return result;
  }

  return readRAM(addr);
}

auto SMP::writeBus(uint16 addr, uint8 data) -> void {
  switch(addr) {
  case 0xf0:  //TEST
    if(regs.p.p) break;  //writes only valid when P flag is clear

    io.clockSpeed    = (data >> 6) & 3;
    io.timerSpeed    = (data >> 4) & 3;
    io.timersEnable  = data & 0x08;
    io.ramDisable    = data & 0x04;
    io.ramWritable   = data & 0x02;
    io.timersDisable = data & 0x01;

    io.timerStep = (1 << io.clockSpeed) + (2 << io.timerSpeed);

    timer0.synchronizeStage1();
    timer1.synchronizeStage1();
    timer2.synchronizeStage1();
    break;

  case 0xf1:  //CONTROL
    io.iplromEnable = data & 0x80;

    if(data & 0x30) {
      //one-time clearing of APU port read registers,
      //emulated by simulating CPU writes of 0x00
      synchronizeCPU();
      if(data & 0x20) {
        cpu.writePort(2, 0x00);
        cpu.writePort(3, 0x00);
      }
      if(data & 0x10) {
        cpu.writePort(0, 0x00);
        cpu.writePort(1, 0x00);
      }
    }

    //0->1 transistion resets timers
    if(!timer2.enable && (data & 0x04)) {
      timer2.stage2 = 0;
      timer2.stage3 = 0;
    }
    timer2.enable = data & 0x04;

    if(!timer1.enable && (data & 0x02)) {
      timer1.stage2 = 0;
      timer1.stage3 = 0;
    }
    timer1.enable = data & 0x02;

    if(!timer0.enable && (data & 0x01)) {
      timer0.stage2 = 0;
      timer0.stage3 = 0;
    }
    timer0.enable = data & 0x01;
    break;

  case 0xf2:  //DSPADDR
    io.dspAddr = data;
    break;

  case 0xf3:  //DSPDATA
    if(io.dspAddr & 0x80) break;  //0x80-0xff are read-only mirrors of 0x00-0x7f
    dsp.write(io.dspAddr & 0x7f, data);
    break;

  case 0xf4:  //CPUIO0
  case 0xf5:  //CPUIO1
  case 0xf6:  //CPUIO2
  case 0xf7:  //CPUIO3
    synchronizeCPU();
    writePort(addr, data);
    break;

  case 0xf8:  //RAM0
    io.ram00f8 = data;
    break;

  case 0xf9:  //RAM1
    io.ram00f9 = data;
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
  case 0xff:  //T2OUT -- read-only registers
    break;
  }

  writeRAM(addr, data);  //all writes, even to MMIO registers, appear on bus
}

auto SMP::idle() -> void {
  step(24);
  cycleEdge();
}

auto SMP::read(uint16 addr) -> uint8 {
  step(12);
  uint8 data = readBus(addr);
  step(12);
  cycleEdge();
  return data;
}

auto SMP::write(uint16 addr, uint8 data) -> void {
  step(24);
  writeBus(addr, data);
  cycleEdge();
}

auto SMP::readDisassembler(uint16 addr) -> uint8 {
  if((addr & 0xfff0) == 0x00f0) return 0x00;
  if((addr & 0xffc0) == 0xffc0 && io.iplromEnable) return iplrom[addr & 0x3f];
  return apuram[addr];
}
