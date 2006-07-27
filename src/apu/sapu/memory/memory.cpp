uint8 sAPU::spcram_read(uint16 addr) {
uint8 r;
  if((addr & 0xfff0) == 0x00f0) {
  //addr >= 0x00f0 && addr <= 0x00ff

  #ifdef FAVOR_SPEED
    co_return();
  #endif

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

#ifdef DEBUGGER
  snes->notify(SNES::SPCRAM_READ, addr, r);
#endif
  return r;
}

void sAPU::spcram_write(uint16 addr, uint8 data) {
  if((addr & 0xfff0) == 0x00f0) {
  //addr >= 0x00f0 && addr >= 0x00ff

  #ifdef FAVOR_SPEED
    co_return();
  #endif

    switch(addr) {
    case 0xf0: //TEST -- operation unknown
      break;
    case 0xf1: //CONTROL
      status.iplrom_enabled = !!(data & 0x80);

    //one-time clearing of APU port read registers,
    //emulated by simulating CPU writes of 0x00
      if(data & 0x20) {
        r_cpu->port_write(2, 0x00);
        r_cpu->port_write(3, 0x00);
      }
      if(data & 0x10) {
        r_cpu->port_write(0, 0x00);
        r_cpu->port_write(1, 0x00);
      }

    //0->1 transistion resets timers
      if(t2.enabled == false && (data & 0x04)) {
        t2.stage2_ticks = 0;
        t2.stage3_ticks = 0;
      }
      t2.enabled = !!(data & 0x04);

      if(t1.enabled == false && (data & 0x02)) {
        t1.stage2_ticks = 0;
        t1.stage3_ticks = 0;
      }
      t1.enabled = !!(data & 0x02);

      if(t0.enabled == false && (data & 0x01)) {
        t0.stage2_ticks = 0;
        t0.stage3_ticks = 0;
      }
      t0.enabled = !!(data & 0x01);
      break;
    case 0xf2: //DSPADDR
      status.dsp_addr = data;
      break;
    case 0xf3: //DSPDATA
    //0x80-0xff is a read-only mirror of 0x00-0x7f
      if(status.dsp_addr < 0x80) {
        r_dsp->write(status.dsp_addr & 0x7f, data);
      }
      break;
    case 0xf4: //CPUIO0
    case 0xf5: //CPUIO1
    case 0xf6: //CPUIO2
    case 0xf7: //CPUIO3
      port_write(addr & 3, data);
      break;
    case 0xf8: //???
    case 0xf9: //??? - Mapped to SPCRAM
      spcram[addr] = data;
      break;
    case 0xfa: //T0TARGET
      t0.target = data;
      break;
    case 0xfb: //T1TARGET
      t1.target = data;
      break;
    case 0xfc: //T2TARGET
      t2.target = data;
      break;
    case 0xfd: //T0OUT
    case 0xfe: //T1OUT
    case 0xff: //T2OUT -- read-only registers
      break;
    }
  } else {
  //writes to $ffc0-$ffff always go to spcram,
  //even if the iplrom is enabled.
    spcram[addr] = data;
  }

#ifdef DEBUGGER
  snes->notify(SNES::SPCRAM_WRITE, addr, data);
#endif
}

uint8 sAPU::port_read(uint8 port) {
  return spcram[0xf4 + (port & 3)];
}

void sAPU::port_write(uint8 port, uint8 data) {
  spcram[0xf4 + (port & 3)] = data;
}

//

void sAPU::op_io() {
  add_clocks(24);
  tick_timers();
//co_return();
}

uint8 sAPU::op_read(uint16 addr) {
  add_clocks(8);
#ifdef FAVOR_ACCURACY
  co_return();
#endif
uint8 r = spcram_read(addr);
  add_clocks(16);
  tick_timers();
  return r;
}

void sAPU::op_write(uint16 addr, uint8 data) {
  add_clocks(24);
  tick_timers();
#ifdef FAVOR_ACCURACY
  co_return();
#endif
  spcram_write(addr, data);
}

//

uint8 sAPU::op_readpc    ()                        { return op_read(regs.pc++); }
uint8 sAPU::op_readstack ()                        { return op_read(0x0100 | ++regs.sp); }
void  sAPU::op_writestack(uint8  data)             { op_write(0x0100 | regs.sp--, data); }
uint8 sAPU::op_readaddr  (uint16 addr)             { return op_read(addr); }

void  sAPU::op_writeaddr (uint16 addr, uint8 data) { op_write(addr, data); }
uint8 sAPU::op_readdp    (uint8  addr)             { return op_read((uint(regs.p.p) << 8) + addr); }
void  sAPU::op_writedp   (uint8  addr, uint8 data) { op_write((uint(regs.p.p) << 8) + addr, data); }
