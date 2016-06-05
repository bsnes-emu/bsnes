alwaysinline auto SMP::ramRead(uint16 addr) -> uint8 {
  if(addr >= 0xffc0 && status.iplromEnable) return iplrom[addr & 0x3f];
  if(status.ramDisable) return 0x5a;  //0xff on mini-SNES
  return apuram[addr];
}

alwaysinline auto SMP::ramWrite(uint16 addr, uint8 data) -> void {
  //writes to $ffc0-$ffff always go to apuram, even if the iplrom is enabled
  if(status.ramWritable && !status.ramDisable) apuram[addr] = data;
}

auto SMP::portRead(uint2 port) const -> uint8 {
  return apuram[0xf4 + port];
}

auto SMP::portWrite(uint2 port, uint8 data) -> void {
  apuram[0xf4 + port] = data;
}

auto SMP::busRead(uint16 addr) -> uint8 {
  uint result;

  switch(addr) {
  case 0xf0:  //TEST -- write-only register
    return 0x00;

  case 0xf1:  //CONTROL -- write-only register
    return 0x00;

  case 0xf2:  //DSPADDR
    return status.dspAddr;

  case 0xf3:  //DSPDATA
    //0x80-0xff are read-only mirrors of 0x00-0x7f
    return dsp.read(status.dspAddr & 0x7f);

  case 0xf4:  //CPUIO0
  case 0xf5:  //CPUIO1
  case 0xf6:  //CPUIO2
  case 0xf7:  //CPUIO3
    synchronizeCPU();
    return cpu.portRead(addr);

  case 0xf8:  //RAM0
    return status.ram00f8;

  case 0xf9:  //RAM1
    return status.ram00f9;

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

  return ramRead(addr);
}

auto SMP::busWrite(uint16 addr, uint8 data) -> void {
  switch(addr) {
  case 0xf0:  //TEST
    if(regs.p.p) break;  //writes only valid when P flag is clear

    status.clockSpeed    = (data >> 6) & 3;
    status.timerSpeed    = (data >> 4) & 3;
    status.timersEnable  = data & 0x08;
    status.ramDisable    = data & 0x04;
    status.ramWritable   = data & 0x02;
    status.timersDisable = data & 0x01;

    status.timerStep = (1 << status.clockSpeed) + (2 << status.timerSpeed);

    timer0.synchronizeStage1();
    timer1.synchronizeStage1();
    timer2.synchronizeStage1();
    break;

  case 0xf1:  //CONTROL
    status.iplromEnable = data & 0x80;

    if(data & 0x30) {
      //one-time clearing of APU port read registers,
      //emulated by simulating CPU writes of 0x00
      synchronizeCPU();
      if(data & 0x20) {
        cpu.portWrite(2, 0x00);
        cpu.portWrite(3, 0x00);
      }
      if(data & 0x10) {
        cpu.portWrite(0, 0x00);
        cpu.portWrite(1, 0x00);
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
    status.dspAddr = data;
    break;

  case 0xf3:  //DSPDATA
    if(status.dspAddr & 0x80) break;  //0x80-0xff are read-only mirrors of 0x00-0x7f
    dsp.write(status.dspAddr & 0x7f, data);
    break;

  case 0xf4:  //CPUIO0
  case 0xf5:  //CPUIO1
  case 0xf6:  //CPUIO2
  case 0xf7:  //CPUIO3
    synchronizeCPU();
    portWrite(addr, data);
    break;

  case 0xf8:  //RAM0
    status.ram00f8 = data;
    break;

  case 0xf9:  //RAM1
    status.ram00f9 = data;
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

  ramWrite(addr, data);  //all writes, even to MMIO registers, appear on bus
}

auto SMP::io() -> void {
  addClocks(24);
  cycleEdge();
}

auto SMP::read(uint16 addr) -> uint8 {
  addClocks(12);
  uint8 data = busRead(addr);
  addClocks(12);
  cycleEdge();
  debugger.op_read(addr, data);
  return data;
}

auto SMP::write(uint16 addr, uint8 data) -> void {
  addClocks(24);
  busWrite(addr, data);
  cycleEdge();
  debugger.op_write(addr, data);
}

auto SMP::disassemblerRead(uint16 addr) -> uint8 {
  if((addr & 0xfff0) == 0x00f0) return 0x00;
  if((addr & 0xffc0) == 0xffc0 && status.iplromEnable) return iplrom[addr & 0x3f];
  return apuram[addr];
}
