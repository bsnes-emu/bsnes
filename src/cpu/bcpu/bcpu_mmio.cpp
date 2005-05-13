void bCPU::mmio_reset() {
//$2181-$2183
  status.wram_addr = 0x000000;

//$4016
  status.joypad1_strobe_value = 0x00;
  status.joypad1_read_pos     = 0;

//$4200
  status.nmi_enabled      = false;
  status.hirq_enabled     = false;
  status.virq_enabled     = false;
  status.auto_joypad_poll = false;

//$4201
  status.pio = 0xff;

//$4202-$4203
  status.mul_a = 0x00;
  status.mul_b = 0x00;

//$4204-$4206
  status.div_a = 0x0000;
  status.div_b = 0x00;

//$4207-$420a
  status.hirq_pos = 0;
  status.virq_pos = 0;

//$4214-$4217
  status.r4214 = 0x0000;
  status.r4216 = 0x0000;
}

//WMDATA
uint8 bCPU::mmio_r2180() {
uint8 r;
  r = mem_bus->read(0x7e0000 | status.wram_addr);
  status.wram_addr++;
  status.wram_addr &= 0x01ffff;
  return r;
}

//???
uint8 bCPU::mmio_r21c2() {
  return 0x20;
}

//???
uint8 bCPU::mmio_r21c3() {
  return 0x00;
}

/*
  The joypad contains a small bit shifter that has 16 bits.
  Reading from 4016 reads one bit from this buffer, then moves
  the buffer left one, and adds a '1' to the rightmost bit.
  Writing a one to $4016 will fill the buffer with the current
  joypad button states, and lock the bit shifter at position
  zero. All reads will be the first buffer state, or 'B'.
  A zero must be written back to $4016 to unlock the buffer,
  so that reads will increment the bit shifting position.
*/
//JOYSER0
uint8 bCPU::mmio_r4016() {
uint8 r = 0x00;
  if(status.joypad1_strobe_value == 1) {
    r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_B);
  } else {
    switch(status.joypad1_read_pos) {
    case  0:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_B);     break;
    case  1:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_Y);     break;
    case  2:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_SELECT);break;
    case  3:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_START); break;
    case  4:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_UP);    break;
    case  5:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_DOWN);  break;
    case  6:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_LEFT);  break;
    case  7:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_RIGHT); break;
    case  8:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_A);     break;
    case  9:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_X);     break;
    case 10:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_L);     break;
    case 11:r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_R);     break;
    case 16:r |= 1;break; //joypad connected bit
    default:r |= 1;break; //after 16th read, all subsequent reads return 1
    }
    if(++status.joypad1_read_pos > 17)status.joypad1_read_pos = 17;
  }
  return r;
}

//RDNMI
uint8 bCPU::mmio_r4210() {
uint8 r = 0x00;
uint16 v, h, hcycles, visible_scanlines;
  v = clock->vcounter();
  h = clock->hcounter();
  hcycles = clock->hcycles();
  visible_scanlines = clock->visible_scanlines();
  if(status.r4210_read == false) {
    if(v == 0 && hcycles < 2) {
      r |= 0x80;
      status.r4210_read = true;
    } else if(v == (visible_scanlines + 1) && hcycles >= 2) {
      r |= 0x80;
      status.r4210_read = true;
    } else if(v >= (visible_scanlines + 1)) {
      r |= 0x80;
      status.r4210_read = true;
    }
  }
  r |= 0x40;
  r |= 0x02; //cpu version number
  status.nmi_pin = 1;
  return r;
}

//TIMEUP
uint8 bCPU::mmio_r4211() {
uint8 r = 0x00;
  r |= 0x40;
  if(status.irq_triggered == true)r |= 0x80;
  status.irq_triggered = false;
  return r;
}

//HVBJOY
uint8 bCPU::mmio_r4212() {
uint8  r;
uint16 v, h, hcycles, visible_scanlines;
  r = 0x00;

  v = clock->vcounter();
  h = clock->hcounter();
  hcycles = clock->hcycles();
  visible_scanlines = clock->visible_scanlines();

//auto joypad polling
  if(v >= (visible_scanlines + 1) && v <= (visible_scanlines + 3))r |= 0x01;

//hblank
  if(hcycles <= 4 || hcycles >= 1098)r |= 0x40;

//vblank
  if(v == 0 && hcycles < 2)r |= 0x80;
  if(v == (visible_scanlines + 1) && hcycles >= 2)r |= 0x80;
  if(v > (visible_scanlines + 1))r |= 0x80;
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
uint8 r = 0x00;
uint16 v = clock->vcounter();
  if(status.auto_joypad_poll == false)return 0x00; //can't read joypad if auto polling not enabled
  if(v >= 225 && v <= 227)return 0x00; //can't read joypad while SNES is polling input
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_A) << 7;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_X) << 6;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_L) << 5;
  r |= (uint8)snes->get_input_status(SNES::DEV_JOYPAD1, SNES::JOYPAD_R) << 4;
  return r;
}

//JOY1H
uint8 bCPU::mmio_r4219() {
uint8 r = 0x00;
uint16 v = clock->vcounter();
  if(status.auto_joypad_poll == false)return 0x00; //can't read joypad if auto polling not enabled
  if(v >= 225 && v <= 227)return 0x00; //can't read joypad while SNES is polling input
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

uint8 bCPUMMIO::read(uint32 addr) {
  clock->sync();
  switch(addr) {
  case 0x2180:return cpu->mmio_r2180(); //WMDATA
  case 0x21c2:return cpu->mmio_r21c2(); //???
  case 0x21c3:return cpu->mmio_r21c3(); //???
  case 0x4016:return cpu->mmio_r4016(); //JOYSER0
  case 0x4210:return cpu->mmio_r4210(); //RDNMI
  case 0x4211:return cpu->mmio_r4211(); //TIMEUP
  case 0x4212:return cpu->mmio_r4212(); //HVBJOY
  case 0x4213:return cpu->mmio_r4213(); //RDIO
  case 0x4214:return cpu->mmio_r4214(); //RDDIVL
  case 0x4215:return cpu->mmio_r4215(); //RDDIVH
  case 0x4216:return cpu->mmio_r4216(); //RDMPYL
  case 0x4217:return cpu->mmio_r4217(); //RDMPYH
  case 0x4218:return cpu->mmio_r4218(); //JOY1L
  case 0x4219:return cpu->mmio_r4219(); //JOY1H
  }
  return 0x00;
}

//WMDATA
void bCPU::mmio_w2180(uint8 value) {
  mem_bus->write(0x7e0000 | status.wram_addr, value);
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
void bCPU::mmio_w4016(uint8 value) {
  status.joypad1_strobe_value = value;
  if(value == 1)snes->poll_input();
  if(value == 0)status.joypad1_read_pos = 0;
}

//NMITIMEN
void bCPU::mmio_w4200(uint8 value) {
  status.nmi_enabled      = !!(value & 0x80);
  status.virq_enabled     = !!(value & 0x20);
  status.hirq_enabled     = !!(value & 0x10);
  status.auto_joypad_poll = !!(value & 0x01);
}

//WRIO
void bCPU::mmio_w4201(uint8 value) {
  if((status.pio & 0x80) && !(value & 0x80)) {
    ppu->latch_counters();
  }
  status.pio = value;
}

//WRMPYA
void bCPU::mmio_w4202(uint8 value) {
  status.mul_a = value;
}

void bCPU::mmio_w4203(uint8 value) {
  status.mul_b = value;
  status.r4216 = status.mul_a * status.mul_b;
}

//WRDIVL
void bCPU::mmio_w4204(uint8 value) {
  status.div_a = (status.div_a & 0xff00) | value;
}

//WRDIVH
void bCPU::mmio_w4205(uint8 value) {
  status.div_a = (status.div_a & 0x00ff) | (value << 8);
}

//WRDIVB
void bCPU::mmio_w4206(uint8 value) {
  status.div_b = value;
  status.r4214 = (status.div_b)?status.div_a / status.div_b:0;
  status.r4216 = (status.div_b)?status.div_a % status.div_b:0;
}

//HTIMEL
void bCPU::mmio_w4207(uint8 value) {
  status.hirq_pos = (status.hirq_pos & 0xff00) | value;
}

//HTIMEH
void bCPU::mmio_w4208(uint8 value) {
  status.hirq_pos = (status.hirq_pos & 0x00ff) | (value << 8);
}

//VTIMEL
void bCPU::mmio_w4209(uint8 value) {
  status.virq_pos = (status.virq_pos & 0xff00) | value;
}

//VTIMEH
void bCPU::mmio_w420a(uint8 value) {
  status.virq_pos = (status.virq_pos & 0x00ff) | (value << 8);
}

//DMAEN
void bCPU::mmio_w420b(uint8 value) {
  for(int i=0;i<8;i++) {
    dma->channel[i].active = !!(value & (1 << i));
    if(dma->channel[i].active == true) {
      dma->channel[i].hdma_active = false;
    }
  }
  cpustate = CPUSTATE_DMA;
}

//HDMAEN
void bCPU::mmio_w420c(uint8 value) {
  for(int i=0;i<8;i++) {
    dma->channel[i].hdma_active = !!(value & (1 << i));
    if(dma->channel[i].hdma_active == true) {
      dma->channel[i].active = false;
    }
  }
}

//MEMSEL
void bCPU::mmio_w420d(uint8 value) {
  mem_bus->fastROM = value & 1;
}

//DMAPx
void bCPU::mmio_w43x0(uint8 value, uint8 i) {
  dma->channel[i].direction     = !!(value & 0x80);
  dma->channel[i].hdma_indirect = !!(value & 0x40);
  dma->channel[i].incmode       = (value & 0x10)?-1:1;
  dma->channel[i].fixedxfer     = !!(value & 0x08);
  dma->channel[i].xfermode      = value & 7;
}

//BBADx
void bCPU::mmio_w43x1(uint8 value, uint8 i) {
  dma->channel[i].destaddr = value;
}

//A1TxL
void bCPU::mmio_w43x2(uint8 value, uint8 i) {
  dma->channel[i].srcaddr = (dma->channel[i].srcaddr & 0xffff00) | value;
}

//A1TxH
void bCPU::mmio_w43x3(uint8 value, uint8 i) {
  dma->channel[i].srcaddr = (dma->channel[i].srcaddr & 0xff00ff) | (value << 8);
}

//A1Bx
void bCPU::mmio_w43x4(uint8 value, uint8 i) {
  dma->channel[i].srcaddr = (dma->channel[i].srcaddr & 0x00ffff) | (value << 16);
}

//DASxL
void bCPU::mmio_w43x5(uint8 value, uint8 i) {
  dma->channel[i].xfersize = (dma->channel[i].xfersize & 0xff00) | value;
}

//DASxH
void bCPU::mmio_w43x6(uint8 value, uint8 i) {
  dma->channel[i].xfersize = (dma->channel[i].xfersize & 0x00ff) | (value << 8);
}

//DASBx
void bCPU::mmio_w43x7(uint8 value, uint8 i) {
  dma->channel[i].hdma_indirect_bank = value;
}

void bCPUMMIO::write(uint32 addr, uint8 value) {
int i;
  clock->sync();
  switch(addr) {
  case 0x2180:cpu->mmio_w2180(value);return; //WMDATA
  case 0x2181:cpu->mmio_w2181(value);return; //WMADDL
  case 0x2182:cpu->mmio_w2182(value);return; //WMADDM
  case 0x2183:cpu->mmio_w2183(value);return; //WMADDH
  case 0x4016:cpu->mmio_w4016(value);return; //JOYSER0
  case 0x4200:cpu->mmio_w4200(value);return; //NMITIMEN
  case 0x4201:cpu->mmio_w4201(value);return; //WRIO
  case 0x4202:cpu->mmio_w4202(value);return; //WRMPYA
  case 0x4203:cpu->mmio_w4203(value);return; //WRMPYB
  case 0x4204:cpu->mmio_w4204(value);return; //WRDIVL
  case 0x4205:cpu->mmio_w4205(value);return; //WRDIVH
  case 0x4206:cpu->mmio_w4206(value);return; //WRDIVB
  case 0x4207:cpu->mmio_w4207(value);return; //HTIMEL
  case 0x4208:cpu->mmio_w4208(value);return; //HTIMEH
  case 0x4209:cpu->mmio_w4209(value);return; //VTIMEL
  case 0x420a:cpu->mmio_w420a(value);return; //VTIMEH
  case 0x420b:cpu->mmio_w420b(value);return; //DMAEN
  case 0x420c:cpu->mmio_w420c(value);return; //HDMAEN
  case 0x420d:cpu->mmio_w420d(value);return; //MEMSEL
//DMAPx
  case 0x4300:case 0x4310:case 0x4320:case 0x4330:
  case 0x4340:case 0x4350:case 0x4360:case 0x4370:
    cpu->mmio_w43x0(value, (addr >> 4) & 7);
    return;
//BBADx
  case 0x4301:case 0x4311:case 0x4321:case 0x4331:
  case 0x4341:case 0x4351:case 0x4361:case 0x4371:
    cpu->mmio_w43x1(value, (addr >> 4) & 7);
    return;
//A1TxL
  case 0x4302:case 0x4312:case 0x4322:case 0x4332:
  case 0x4342:case 0x4352:case 0x4362:case 0x4372:
    cpu->mmio_w43x2(value, (addr >> 4) & 7);
    return;
//A1TxH
  case 0x4303:case 0x4313:case 0x4323:case 0x4333:
  case 0x4343:case 0x4353:case 0x4363:case 0x4373:
    cpu->mmio_w43x3(value, (addr >> 4) & 7);
    return;
//A1Bx
  case 0x4304:case 0x4314:case 0x4324:case 0x4334:
  case 0x4344:case 0x4354:case 0x4364:case 0x4374:
    cpu->mmio_w43x4(value, (addr >> 4) & 7);
    return;
//DASxL
  case 0x4305:case 0x4315:case 0x4325:case 0x4335:
  case 0x4345:case 0x4355:case 0x4365:case 0x4375:
    cpu->mmio_w43x5(value, (addr >> 4) & 7);
    return;
//DASxH
  case 0x4306:case 0x4316:case 0x4326:case 0x4336:
  case 0x4346:case 0x4356:case 0x4366:case 0x4376:
    cpu->mmio_w43x6(value, (addr >> 4) & 7);
    return;
//DASBx
  case 0x4307:case 0x4317:case 0x4327:case 0x4337:
  case 0x4347:case 0x4357:case 0x4367:case 0x4377:
    cpu->mmio_w43x7(value, (addr >> 4) & 7);
    return;
  }
}

bCPUMMIO::bCPUMMIO(bCPU *_cpu) {
  cpu = _cpu;
}

uint16 bDMA::dma_cputommio(uint8 i, uint8 index) {
uint8 x;
  x = mem_bus->read(channel[i].srcaddr);
  mem_bus->write(0x2100 | ((channel[i].destaddr + index) & 0xff), x);
  if(channel[i].fixedxfer == false) {
    channel[i].srcaddr = (channel[i].srcaddr & 0xff0000) + ((channel[i].srcaddr + channel[i].incmode) & 0xffff);
  }
  clock->add_cc1_cycles(8);
  return --channel[i].xfersize;
}

uint16 bDMA::dma_mmiotocpu(uint8 i, uint8 index) {
uint8 x;
  x = mem_bus->read(0x2100 | ((channel[i].destaddr + index) & 0xff));
  mem_bus->write(channel[i].srcaddr, x);
  if(channel[i].fixedxfer == false) {
    channel[i].srcaddr = (channel[i].srcaddr & 0xff0000) + ((channel[i].srcaddr + channel[i].incmode) & 0xffff);
  }
  clock->add_cc1_cycles(8);
  return --channel[i].xfersize;
}

void bDMA::dma_xfer_type0(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
  }
}

void bDMA::dma_xfer_type1(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 1) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 1) == 0)return;
  }
}

void bDMA::dma_xfer_type2(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 0) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 0) == 0)return;
  }
}

void bDMA::dma_xfer_type3(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 1) == 0)return;
    if(dma_cputommio(i, 1) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 1) == 0)return;
    if(dma_mmiotocpu(i, 1) == 0)return;
  }
}

void bDMA::dma_xfer_type4(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 1) == 0)return;
    if(dma_cputommio(i, 2) == 0)return;
    if(dma_cputommio(i, 3) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 1) == 0)return;
    if(dma_mmiotocpu(i, 2) == 0)return;
    if(dma_mmiotocpu(i, 3) == 0)return;
  }
}

void bDMA::dma_xfer_type5(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 1) == 0)return;
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 1) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 1) == 0)return;
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 1) == 0)return;
  }
}

void bDMA::run() {
  for(int i=0;i<8;i++) {
    if(channel[i].active == false)continue;
    switch(channel[i].xfermode) {
    case 0:dma_xfer_type0(i);break;
    case 1:dma_xfer_type1(i);break;
    case 2:dma_xfer_type2(i);break;
    case 3:dma_xfer_type3(i);break;
    case 4:dma_xfer_type4(i);break;
    case 5:dma_xfer_type5(i);break;
    case 6:dma_xfer_type2(i);break;
    case 7:dma_xfer_type3(i);break;
    }
    if(channel[i].xfersize == 0) {
      channel[i].active = false;
    }
    return;
  }
  cpu->cpustate = bCPU::CPUSTATE_RUN;
}

void bDMA::hdma_write(uint8 i, uint8 l, uint8 x) {
  switch(channel[i].xfermode) {
  case 0:mem_bus->write(0x2100 | ((channel[i].destaddr)            & 0xff), x);break;
  case 1:mem_bus->write(0x2100 | ((channel[i].destaddr + l)        & 0xff), x);break;
  case 2:mem_bus->write(0x2100 | ((channel[i].destaddr)            & 0xff), x);break;
  case 3:mem_bus->write(0x2100 | ((channel[i].destaddr + (l >> 1)) & 0xff), x);break;
  case 4:mem_bus->write(0x2100 | ((channel[i].destaddr + l)        & 0xff), x);break;
  case 5:mem_bus->write(0x2100 | ((channel[i].destaddr + (l & 1))  & 0xff), x);break;
  case 6:mem_bus->write(0x2100 | ((channel[i].destaddr)            & 0xff), x);break;
  case 7:mem_bus->write(0x2100 | ((channel[i].destaddr + (l >> 1)) & 0xff), x);break;
  }
}

void bDMA::hdma_run() {
int l, xferlen;
uint8 x, active_channels = 0;
  for(int i=0;i<8;i++) {
    if(channel[i].hdma_completed == true)continue;
    clock->add_cc1_cycles(8);
    active_channels++;
    if(channel[i].hdma_line_counter == 0) {
      channel[i].hdma_line_counter = mem_bus->read(channel[i].hdma_address++);
//    channel[i].r43x8 = channel[i].hdma_address;
      if(channel[i].hdma_line_counter == 0) {
        channel[i].hdma_completed = true;
        continue;
      }

      if(channel[i].hdma_line_counter > 0x80) {
        channel[i].hdma_repeat = true;
        channel[i].hdma_line_counter -= 0x80;
      } else {
        channel[i].hdma_repeat = false;
      }

      channel[i].hdma_first_line = true;
      if(channel[i].hdma_indirect == false) {
        channel[i].hdma_iaddress = channel[i].hdma_address;
      } else {
        channel[i].hdma_iaddress  = mem_bus->read(channel[i].hdma_address++);
        channel[i].hdma_iaddress |= mem_bus->read(channel[i].hdma_address++) << 8;
        channel[i].hdma_iaddress |= channel[i].hdma_indirect_bank << 16;
        clock->add_cc1_cycles(16);
      }
    }

    channel[i].hdma_line_counter--;
    if(channel[i].hdma_first_line == false && channel[i].hdma_repeat == false)continue;
    channel[i].hdma_first_line = false;

    if(channel[i].hdma_indirect == false) {
      channel[i].hdma_iaddress = channel[i].hdma_address;
    }

    switch(channel[i].xfermode) {
    case 0:                     xferlen = 1;break;
    case 1:case 2:case 6:       xferlen = 2;break;
    case 3:case 4:case 5:case 7:xferlen = 4;break;
    }

    for(l=0;l<xferlen;l++) {
      x = mem_bus->read(channel[i].hdma_iaddress++);
      if(channel[i].hdma_indirect == false) {
        channel[i].hdma_address++;
      }
      hdma_write(i, l, x);
      clock->add_cc1_cycles(8);
    }
  }
  if(active_channels != 0) {
    clock->add_cc1_cycles(18);
  }
}

void bDMA::hdma_initialize() {
uint8 active_channels = 0;
  for(int i=0;i<8;i++) {
    if(!channel[i].hdma_active) {
      channel[i].hdma_completed = true;
      continue;
    }
    active_channels++;
    channel[i].hdma_first_line   = true;
    channel[i].hdma_repeat       = false;
    channel[i].hdma_line_counter = 0;
    channel[i].hdma_address      = channel[i].srcaddr;
    channel[i].hdma_completed    = false;
    if(channel[i].hdma_indirect == false) {
      clock->add_cc1_cycles(8);
    } else {
      clock->add_cc1_cycles(24);
    }
  }
  if(active_channels != 0) {
    clock->add_cc1_cycles(18);
  }
}

void bDMA::reset() {
  for(int i=0;i<8;i++) {
    channel[i].active             = false;
    channel[i].hdma_active        = false;
    channel[i].direction          = 0;
    channel[i].hdma_indirect      = false;
    channel[i].incmode            = 1;
    channel[i].fixedxfer          = false;
    channel[i].xfermode           = 0;
    channel[i].destaddr           = 0;
    channel[i].srcaddr            = 0;
    channel[i].xfersize           = 0;
    channel[i].hdma_indirect_bank = 0;
    channel[i].hdma_first_line    = false;
    channel[i].hdma_repeat        = false;
    channel[i].hdma_line_counter  = 0;
    channel[i].hdma_address       = 0;
    channel[i].hdma_iaddress      = 0;
    channel[i].hdma_completed     = false;
  }
  hdma_triggered = false;
}

bDMA::bDMA(bCPU *_cpu) {
  cpu = _cpu;
  reset();
}
