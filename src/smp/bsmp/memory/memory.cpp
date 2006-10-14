uint8 bSMP::ram_read(uint16 addr) {
uint8 r;
  if(addr >= 0x00f0 && addr <= 0x00ff) {
    switch(addr) {
    case 0xf0: //TEST -- operation unknown, supposedly returns 0x00
      r = 0x00;
      break;
    case 0xf1: //CONTROL -- write-only register, always returns 0x00
      r = 0x00;
      break;
    case 0xf2: //DSPADDR
      r = status.dsp_addr;
      break;
    case 0xf3: //DSPDATA
    //0x80-0xff is a read-only mirror of 0x00-0x7f
      r = r_dsp->read(status.dsp_addr & 0x7f);
      break;
    case 0xf4: //CPUIO0
    case 0xf5: //CPUIO1
    case 0xf6: //CPUIO2
    case 0xf7: //CPUIO3
      r = r_cpu->port_read(addr & 3);
      break;
    case 0xf8: //???
    case 0xf9: //??? -- Mapped to SPCRAM
      r = spcram[addr];
      break;
    case 0xfa: //T0TARGET
    case 0xfb: //T1TARGET
    case 0xfc: //T2TARGET -- write-only registers, always return 0x00
      r = 0x00;
      break;
    case 0xfd: //T0OUT -- 4-bit counter value
      r = t0.stage3_ticks & 15;
      t0.stage3_ticks = 0;
      break;
    case 0xfe: //T1OUT -- 4-bit counter value
      r = t1.stage3_ticks & 15;
      t1.stage3_ticks = 0;
      break;
    case 0xff: //T2OUT -- 4-bit counter value
      r = t2.stage3_ticks & 15;
      t2.stage3_ticks = 0;
      break;
    }
  } else if(addr < 0xffc0) {
    r = spcram[addr];
  } else {
    if(status.iplrom_enabled == true) {
      r = iplrom[addr & 0x3f];
    } else {
      r = spcram[addr];
    }
  }

  return r;
}

void bSMP::ram_write(uint16 addr, uint8 value) {
  if(addr >= 0x00f0 && addr <= 0x00ff) {
    switch(addr) {
    case 0xf0: //TEST -- operation unknown
      break;
    case 0xf1: //CONTROL
      status.iplrom_enabled = !!(value & 0x80);

    //one-time clearing of SMP port read registers,
    //emulated by simulating CPU writes of 0x00
      if(value & 0x20) {
        r_cpu->port_write(2, 0x00);
        r_cpu->port_write(3, 0x00);
      }
      if(value & 0x10) {
        r_cpu->port_write(0, 0x00);
        r_cpu->port_write(1, 0x00);
      }

    //0->1 transistion resets timers
      if(t2.enabled == false && (value & 0x04)) {
        t2.stage2_ticks = 0;
        t2.stage3_ticks = 0;
      }
      t2.enabled = !!(value & 0x04);

      if(t1.enabled == false && (value & 0x02)) {
        t1.stage2_ticks = 0;
        t1.stage3_ticks = 0;
      }
      t1.enabled = !!(value & 0x02);

      if(t0.enabled == false && (value & 0x01)) {
        t0.stage2_ticks = 0;
        t0.stage3_ticks = 0;
      }
      t0.enabled = !!(value & 0x01);
      break;
    case 0xf2: //DSPADDR
      status.dsp_addr = value;
      break;
    case 0xf3: //DSPDATA
    //0x80-0xff is a read-only mirror of 0x00-0x7f
      if(status.dsp_addr < 0x80) {
        r_dsp->write(status.dsp_addr & 0x7f, value);
      }
      break;
    case 0xf4: //CPUIO0
    case 0xf5: //CPUIO1
    case 0xf6: //CPUIO2
    case 0xf7: //CPUIO3
      port_write(addr & 3, value);
      break;
    case 0xf8: //???
    case 0xf9: //??? - Mapped to SPCRAM
      spcram[addr] = value;
      break;
    case 0xfa: //T0TARGET
      t0.target = value;
      break;
    case 0xfb: //T1TARGET
      t1.target = value;
      break;
    case 0xfc: //T2TARGET
      t2.target = value;
      break;
    case 0xfd: //T0OUT
    case 0xfe: //T1OUT
    case 0xff: //T2OUT -- read-only registers
      break;
    }
  } else {
  //writes to $ffc0-$ffff always go to spcram,
  //even if the iplrom is enabled.
    spcram[addr] = value;
  }
}

uint8 bSMP::port_read(uint8 port) {
  return spcram[0xf4 + (port & 3)];
}

void bSMP::port_write(uint8 port, uint8 value) {
  spcram[0xf4 + (port & 0x03)] = value;
}

uint8 bSMP::op_read() {
uint8 r;
  r = ram_read(regs.pc);
  regs.pc++;
  return r;
}

uint8 bSMP::op_read(uint8 mode, uint16 addr) {
uint8 r;
  switch(mode) {
  case OPMODE_ADDR:
    r = ram_read(addr);
    break;
  case OPMODE_DP:
    r = ram_read(((regs.p.p)?0x100:0x000) + (addr & 0xff));
    break;
  }
  return r;
}

void bSMP::op_write(uint8 mode, uint16 addr, uint8 value) {
  switch(mode) {
  case OPMODE_ADDR:
    ram_write(addr, value);
    break;
  case OPMODE_DP:
    ram_write(((regs.p.p)?0x100:0x000) + (addr & 0xff), value);
    break;
  }
}

uint8 bSMP::stack_read() {
  regs.sp++;
  return ram_read(0x0100 | regs.sp);
}

void bSMP::stack_write(uint8 value) {
  ram_write(0x0100 | regs.sp, value);
  regs.sp--;
}
