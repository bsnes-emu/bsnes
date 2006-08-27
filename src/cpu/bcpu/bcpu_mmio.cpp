void bCPU::mmio_reset() {
//$2181-$2183
  status.wram_addr = 0x000000;

//$4016-$4017
  status.joypad_strobe_latch = 0;
  status.joypad1_read_pos    = 0;
  status.joypad2_read_pos    = 0;

//$4200
  status.nmi_enabled      = false;
  status.hirq_enabled     = false;
  status.virq_enabled     = false;
  status.auto_joypad_poll = false;

//$4201
  status.pio = 0xff;

//$4202-$4203
  status.mul_a = 0xff;
  status.mul_b = 0xff;

//$4204-$4206
  status.div_a = 0xffff;
  status.div_b = 0xff;

//$4207-$420a
  status.hirq_pos = 0x01ff;
  status.virq_pos = 0x01ff;

//$4214-$4217
  status.r4214 = 0x0000;
  status.r4216 = 0x0000;
}

//WMDATA
uint8 bCPU::mmio_r2180() {
uint8 r;
  r = r_mem->read(0x7e0000 | status.wram_addr);
  status.wram_addr++;
  status.wram_addr &= 0x01ffff;
  return r;
}

//JOYSER0
//7-2 = MDR
//1-0 = Joypad serial data
/* The joypad contains a small bit shifter that has 16 bits.
 * Reading from 4016 reads one bit from this buffer, then moves
 * the buffer left one, and adds a '1' to the rightmost bit.
 * Writing a one to $4016 will fill the buffer with the current
 * joypad button states, and lock the bit shifter at position
 * zero. All reads will be the first buffer state, or 'B'.
 * A zero must be written back to $4016 to unlock the buffer,
 * so that reads will increment the bit shifting position.
 */
uint8 bCPU::mmio_r4016() {
uint8 r;
  r = regs.mdr & 0xfc;

  if(status.joypad_strobe_latch == 1) {
    r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_B);
  } else {
    switch(status.joypad1_read_pos) {
    case  0: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_B);      break;
    case  1: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_Y);      break;
    case  2: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_SELECT); break;
    case  3: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_START);  break;
    case  4: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_UP);     break;
    case  5: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_DOWN);   break;
    case  6: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_LEFT);   break;
    case  7: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_RIGHT);  break;
    case  8: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_A);      break;
    case  9: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_X);      break;
    case 10: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_L);      break;
    case 11: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_R);      break;
    case 12: break;
    case 13: break;
    case 14: break;
    case 15: break; //bits 12-15 always return 0
  //all subsequent reads return joypad connection status
    case 16: r |= 1; break; //joypad connected bit
    }
    if(++status.joypad1_read_pos > 16)status.joypad1_read_pos = 16;
  }

  return r;
}

//JOYSER1
//7-5 = MDR
//4-2 = Always 1 (pins are connected to GND)
//1-0 = Joypad serial data
uint8 bCPU::mmio_r4017() {
uint8 r;
  r  = regs.mdr & 0xe0;
  r |= 0x1c;

  if(status.joypad_strobe_latch == 1) {
    r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_B);
  } else {
    switch(status.joypad2_read_pos) {
    case  0: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_B);      break;
    case  1: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_Y);      break;
    case  2: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_SELECT); break;
    case  3: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_START);  break;
    case  4: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_UP);     break;
    case  5: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_DOWN);   break;
    case  6: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_LEFT);   break;
    case  7: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_RIGHT);  break;
    case  8: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_A);      break;
    case  9: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_X);      break;
    case 10: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_L);      break;
    case 11: r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_R);      break;
    case 12: break;
    case 13: break;
    case 14: break;
    case 15: break; //bits 12-15 always return 0
  //all subsequent reads return joypad connection status
    case 16: r |= 1; break; //joypad connected bit
    }
    if(++status.joypad2_read_pos > 16)status.joypad2_read_pos = 16;
  }

  return r;
}

//RDNMI
//7   = NMI acknowledge
//6-4 = MDR
//3-0 = CPU (5a22) version
uint8 bCPU::mmio_r4210() {
uint8 r;
  r  = regs.mdr & 0x70;
  r |= uint8(!time.nmi_read) << 7;

  if(!nmi_trigger_pos_match(0) && !nmi_trigger_pos_match(2)) {
    time.nmi_read = 1;
  }

  r |= (cpu_version & 0x0f);
  return r;
}

//TIMEUP
//7   = IRQ acknowledge
//6-0 = MDR
uint8 bCPU::mmio_r4211() {
uint8 r;
  r  = regs.mdr & 0x7f;
  r |= uint8(!time.irq_read) << 7;

  if(!irq_trigger_pos_match(0) && !irq_trigger_pos_match(2)) {
    time.irq_read = 1;
    time.irq_line = 1;
    time.irq_transition = 0;
  }

  return r;
}

//HVBJOY
//7   = in vblank
//6   = in hblank
//5-1 = MDR
//0   = joypad ready
uint8 bCPU::mmio_r4212() {
uint8 r;
  r = regs.mdr & 0x3e;

uint16 vs = overscan() ? 240 : 225;

//auto joypad polling
  if(time.v >= vs && time.v <= (vs + 2))r |= 0x01;

//hblank
  if(time.hc <= 2 || time.hc >= 1096)r |= 0x40;

//vblank
  if(time.v >= vs)r |= 0x80;

  return r;
}

//RDIO
uint8 bCPU::mmio_r4213() {
  return status.pio;
}

//RDDIVL
uint8 bCPU::mmio_r4214() {
  return status.r4214;
}

//RDDIVH
uint8 bCPU::mmio_r4215() {
  return status.r4214 >> 8;
}

//RDMPYL
uint8 bCPU::mmio_r4216() {
  return status.r4216;
}

//RDMPYH
uint8 bCPU::mmio_r4217() {
  return status.r4216 >> 8;
}

//JOY1L
uint8 bCPU::mmio_r4218() {
uint8  r = 0x00;
uint16 v = vcounter();
  if(status.auto_joypad_poll == false)return 0x00; //can't read joypad if auto polling not enabled
//if(v >= 225 && v <= 227)return 0x00; //can't read joypad while SNES is polling input
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_A) << 7;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_X) << 6;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_L) << 5;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_R) << 4;
  return r;
}

//JOY1H
uint8 bCPU::mmio_r4219() {
uint8  r = 0x00;
uint16 v = vcounter();
  if(status.auto_joypad_poll == false)return 0x00; //can't read joypad if auto polling not enabled
//if(v >= 225 && v <= 227)return 0x00; //can't read joypad while SNES is polling input
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_B)      << 7;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_Y)      << 6;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_SELECT) << 5;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_START)  << 4;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_UP)     << 3;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_DOWN)   << 2;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_LEFT)   << 1;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_RIGHT);
  return r;
}

//JOY2L
uint8 bCPU::mmio_r421a() {
uint8  r = 0x00;
uint16 v = vcounter();
  if(status.auto_joypad_poll == false)return 0x00; //can't read joypad if auto polling not enabled
//if(v >= 225 && v <= 227)return 0x00; //can't read joypad while SNES is polling input
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_A) << 7;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_X) << 6;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_L) << 5;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_R) << 4;
  return r;
}

//JOY2H
uint8 bCPU::mmio_r421b() {
uint8  r = 0x00;
uint16 v = vcounter();
  if(status.auto_joypad_poll == false)return 0x00; //can't read joypad if auto polling not enabled
//if(v >= 225 && v <= 227)return 0x00; //can't read joypad while SNES is polling input
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_B)      << 7;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_Y)      << 6;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_SELECT) << 5;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_START)  << 4;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_UP)     << 3;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_DOWN)   << 2;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_LEFT)   << 1;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD2, SNES::JOYPAD_RIGHT);
  return r;
}

//DMAPx
uint8 bCPU::mmio_r43x0(uint8 i) {
  return channel[i].dmap;
}

//BBADx
uint8 bCPU::mmio_r43x1(uint8 i) {
  return channel[i].destaddr;
}

//A1TxL
uint8 bCPU::mmio_r43x2(uint8 i) {
  return channel[i].srcaddr;
}

//A1TxH
uint8 bCPU::mmio_r43x3(uint8 i) {
  return channel[i].srcaddr >> 8;
}

//A1Bx
uint8 bCPU::mmio_r43x4(uint8 i) {
  return channel[i].srcbank;
}

//DASxL
uint8 bCPU::mmio_r43x5(uint8 i) {
  return channel[i].xfersize;
}

//DASxH
uint8 bCPU::mmio_r43x6(uint8 i) {
  return channel[i].xfersize >> 8;
}

//DASBx
uint8 bCPU::mmio_r43x7(uint8 i) {
  return channel[i].hdma_ibank;
}

//A2AxL
uint8 bCPU::mmio_r43x8(uint8 i) {
  return channel[i].hdma_addr;
}

//A2AxH
uint8 bCPU::mmio_r43x9(uint8 i) {
  return channel[i].hdma_addr >> 8;
}

//NTRLx
uint8 bCPU::mmio_r43xa(uint8 i) {
  return channel[i].hdma_line_counter;
}

//???
uint8 bCPU::mmio_r43xb(uint8 i) {
  return channel[i].hdma_unknown;
}

uint8 bCPU::mmio_read(uint16 addr) {
//APU
  if((addr & 0xffc0) == 0x2140) { //$2140-$217f
    return r_apu->port_read(addr & 3);
  }

//HDMA
  if((addr & 0xff80) == 0x4300) { //$4300-$437f
  uint i = (addr >> 4) & 7;
    switch(addr & 0xf) {
    case 0x0: return mmio_r43x0(i);
    case 0x1: return mmio_r43x1(i);
    case 0x2: return mmio_r43x2(i);
    case 0x3: return mmio_r43x3(i);
    case 0x4: return mmio_r43x4(i);
    case 0x5: return mmio_r43x5(i);
    case 0x6: return mmio_r43x6(i);
    case 0x7: return mmio_r43x7(i);
    case 0x8: return mmio_r43x8(i);
    case 0x9: return mmio_r43x9(i);
    case 0xa: return mmio_r43xa(i);
    case 0xb: return mmio_r43xb(i);
    case 0xc: return regs.mdr; //unmapped
    case 0xd: return regs.mdr; //unmapped
    case 0xe: return regs.mdr; //unmapped
    case 0xf: return mmio_r43xb(i); //mirror of 43xb
    }
  }

  switch(addr) {
  case 0x2180: return mmio_r2180(); //WMDATA
  case 0x4016: return mmio_r4016(); //JOYSER0
  case 0x4017: return mmio_r4017(); //JOYSER1
  case 0x4210: return mmio_r4210(); //RDNMI
  case 0x4211: return mmio_r4211(); //TIMEUP
  case 0x4212: return mmio_r4212(); //HVBJOY
  case 0x4213: return mmio_r4213(); //RDIO
  case 0x4214: return mmio_r4214(); //RDDIVL
  case 0x4215: return mmio_r4215(); //RDDIVH
  case 0x4216: return mmio_r4216(); //RDMPYL
  case 0x4217: return mmio_r4217(); //RDMPYH
  case 0x4218: return mmio_r4218(); //JOY1L
  case 0x4219: return mmio_r4219(); //JOY1H
  case 0x421a: return mmio_r421a(); //JOY2L
  case 0x421b: return mmio_r421b(); //JOY2H
  case 0x421c: return 0x00;
  case 0x421d: return 0x00;
  case 0x421e: return 0x00;
  case 0x421f: return 0x00;
  }

  return regs.mdr;
}

//WMDATA
void bCPU::mmio_w2180(uint8 value) {
  r_mem->write(0x7e0000 | status.wram_addr, value);
  status.wram_addr++;
  status.wram_addr &= 0x01ffff;
}

//WMADDL
void bCPU::mmio_w2181(uint8 value) {
  status.wram_addr = (status.wram_addr & 0xffff00) | value;
  status.wram_addr &= 0x01ffff;
}

//WMADDM
void bCPU::mmio_w2182(uint8 value) {
  status.wram_addr = (status.wram_addr & 0xff00ff) | (value << 8);
  status.wram_addr &= 0x01ffff;
}

//WMADDH
void bCPU::mmio_w2183(uint8 value) {
  status.wram_addr = (status.wram_addr & 0x00ffff) | (value << 16);
  status.wram_addr &= 0x01ffff;
}

//JOYSER0
//bit 0 is shared between JOYSER0 and JOYSER1, therefore
//strobing $4016.d0 affects both controller port latches.
//$4017 bit 0 writes are ignored.
void bCPU::mmio_w4016(uint8 value) {
  status.joypad_strobe_latch = bool(value & 1);

  if(status.joypad_strobe_latch == 1) {
    snes->poll_input(SNES::DEV_JOYPAD1);
    snes->poll_input(SNES::DEV_JOYPAD2);
    status.joypad1_read_pos = 0;
    status.joypad2_read_pos = 0;
  }
}

//NMITIMEN
void bCPU::mmio_w4200(uint8 value) {
  status.nmi_enabled      = bool(value & 0x80);
  status.virq_enabled     = bool(value & 0x20);
  status.hirq_enabled     = bool(value & 0x10);
  status.auto_joypad_poll = bool(value & 0x01);

  if(time.nmi_read == 0) {
    if(time.nmi_line == 1 && !status.nmi_enabled == 0) {
      time.nmi_transition = 1;
    }
    time.nmi_line = !status.nmi_enabled;
  }

  if(status.virq_enabled == false && status.hirq_enabled == false) {
    time.irq_line = 1;
    time.irq_read = 1;
    time.irq_transition = 0;
  }

  update_interrupts();
}

//WRIO
void bCPU::mmio_w4201(uint8 value) {
  if((status.pio & 0x80) && !(value & 0x80)) {
    r_ppu->latch_counters();
  }
  status.pio = value;
}

//WRMPYA
void bCPU::mmio_w4202(uint8 value) {
  status.mul_a = value;
}

//WRMPYB
void bCPU::mmio_w4203(uint8 value) {
  status.mul_b = value;
  status.r4216 = status.mul_a * status.mul_b;
}

//WRDIVL
void bCPU::mmio_w4204(uint8 value) {
  status.div_a = (status.div_a & 0xff00) | (value);
}

//WRDIVH
void bCPU::mmio_w4205(uint8 value) {
  status.div_a = (status.div_a & 0x00ff) | (value << 8);
}

//WRDIVB
void bCPU::mmio_w4206(uint8 value) {
  status.div_b = value;
  status.r4214 = (status.div_b) ? status.div_a / status.div_b : 0xffff;
  status.r4216 = (status.div_b) ? status.div_a % status.div_b : status.div_a;
}

//HTIMEL
void bCPU::mmio_w4207(uint8 value) {
  status.hirq_pos = ((status.hirq_pos & 0xff00) | value) & 0x01ff;
  update_interrupts();
}

//HTIMEH
void bCPU::mmio_w4208(uint8 value) {
  status.hirq_pos = ((status.hirq_pos & 0x00ff) | (value << 8)) & 0x01ff;
  update_interrupts();
}

//VTIMEL
void bCPU::mmio_w4209(uint8 value) {
  status.virq_pos = ((status.virq_pos & 0xff00) | value) & 0x01ff;
  update_interrupts();
}

//VTIMEH
void bCPU::mmio_w420a(uint8 value) {
  status.virq_pos = ((status.virq_pos & 0x00ff) | (value << 8)) & 0x01ff;
  update_interrupts();
}

//DMAEN
//DMA enable does not disable active HDMA channels
void bCPU::mmio_w420b(uint8 value) {
  if(value != 0x00) {
    run_state.dma = true;
    status.dma_state = DMASTATE_DMASYNC;
  }

  for(int i = 0; i < 8; i++) {
    channel[i].dma_enabled = bool(value & (1 << i));
  //TODO: clearing read_index may interfere with DMA+HDMA occurring simultaneously
    if(channel[i].dma_enabled)channel[i].read_index = 0;
  }
}

//HDMAEN
void bCPU::mmio_w420c(uint8 value) {
  for(int i = 0; i < 8; i++) {
    channel[i].hdma_enabled = bool(value & (1 << i));
  }
}

//MEMSEL
void bCPU::mmio_w420d(uint8 value) {
  r_mem->set_speed(value & 1);
}

//DMAPx
void bCPU::mmio_w43x0(uint8 value, uint8 i) {
  channel[i].dmap          = value;
  channel[i].direction     = bool(value & 0x80);
  channel[i].hdma_indirect = bool(value & 0x40);
  channel[i].incmode       = (value & 0x10) ? -1 : 1;
  channel[i].fixedxfer     = bool(value & 0x08);
  channel[i].xfermode      = value & 7;
}

//BBADx
void bCPU::mmio_w43x1(uint8 value, uint8 i) {
  channel[i].destaddr = value;
}

//A1TxL
void bCPU::mmio_w43x2(uint8 value, uint8 i) {
  channel[i].srcaddr = (channel[i].srcaddr & 0xff00) | value;
}

//A1TxH
void bCPU::mmio_w43x3(uint8 value, uint8 i) {
  channel[i].srcaddr = (channel[i].srcaddr & 0x00ff) | (value << 8);
}

//A1Bx
void bCPU::mmio_w43x4(uint8 value, uint8 i) {
  channel[i].srcbank = value;
}

//DASxL
void bCPU::mmio_w43x5(uint8 value, uint8 i) {
  channel[i].xfersize = (channel[i].xfersize & 0xff00) | value;
}

//DASxH
void bCPU::mmio_w43x6(uint8 value, uint8 i) {
  channel[i].xfersize = (channel[i].xfersize & 0x00ff) | (value << 8);
}

//DASBx
void bCPU::mmio_w43x7(uint8 value, uint8 i) {
  channel[i].hdma_ibank = value;
}

//A2AxL
void bCPU::mmio_w43x8(uint8 value, uint8 i) {
  channel[i].hdma_addr = (channel[i].hdma_addr & 0xff00) | value;
}

//A2AxH
void bCPU::mmio_w43x9(uint8 value, uint8 i) {
  channel[i].hdma_addr = (channel[i].hdma_addr & 0x00ff) | (value << 8);
}

//NTRLx
void bCPU::mmio_w43xa(uint8 value, uint8 i) {
  channel[i].hdma_line_counter = value;
}

//???
void bCPU::mmio_w43xb(uint8 value, uint8 i) {
  channel[i].hdma_unknown = value;
}

void bCPU::mmio_write(uint16 addr, uint8 data) {
//APU
  if((addr & 0xffc0) == 0x2140) { //$2140-$217f
    port_write(addr & 3, data);
    return;
  }

//HDMA
  if((addr & 0xff80) == 0x4300) { //$4300-$437f
  uint i = (addr >> 4) & 7;
    switch(addr & 0xf) {
    case 0x0: mmio_w43x0(data, i); return;
    case 0x1: mmio_w43x1(data, i); return;
    case 0x2: mmio_w43x2(data, i); return;
    case 0x3: mmio_w43x3(data, i); return;
    case 0x4: mmio_w43x4(data, i); return;
    case 0x5: mmio_w43x5(data, i); return;
    case 0x6: mmio_w43x6(data, i); return;
    case 0x7: mmio_w43x7(data, i); return;
    case 0x8: mmio_w43x8(data, i); return;
    case 0x9: mmio_w43x9(data, i); return;
    case 0xa: mmio_w43xa(data, i); return;
    case 0xb: mmio_w43xb(data, i); return;
    case 0xc: return; //unmapped
    case 0xd: return; //unmapped
    case 0xe: return; //unmapped
    case 0xf: mmio_w43xb(data, i); return; //mirror of 43xb
    }
  }

  switch(addr) {
  case 0x2180: mmio_w2180(data); return; //WMDATA
  case 0x2181: mmio_w2181(data); return; //WMADDL
  case 0x2182: mmio_w2182(data); return; //WMADDM
  case 0x2183: mmio_w2183(data); return; //WMADDH
  case 0x4016: mmio_w4016(data); return; //JOYSER0
  case 0x4017: return; //unmapped
  case 0x4200: mmio_w4200(data); return; //NMITIMEN
  case 0x4201: mmio_w4201(data); return; //WRIO
  case 0x4202: mmio_w4202(data); return; //WRMPYA
  case 0x4203: mmio_w4203(data); return; //WRMPYB
  case 0x4204: mmio_w4204(data); return; //WRDIVL
  case 0x4205: mmio_w4205(data); return; //WRDIVH
  case 0x4206: mmio_w4206(data); return; //WRDIVB
  case 0x4207: mmio_w4207(data); return; //HTIMEL
  case 0x4208: mmio_w4208(data); return; //HTIMEH
  case 0x4209: mmio_w4209(data); return; //VTIMEL
  case 0x420a: mmio_w420a(data); return; //VTIMEH
  case 0x420b: mmio_w420b(data); return; //DMAEN
  case 0x420c: mmio_w420c(data); return; //HDMAEN
  case 0x420d: mmio_w420d(data); return; //MEMSEL
  }
}
