uint16 bCPU::dma_cputommio(uint8 i, uint8 index) {
uint8 x;
  x = mem_bus->read(channel[i].srcaddr);
  mem_bus->write(0x2100 | ((channel[i].destaddr + index) & 0xff), x);
  if(channel[i].fixedxfer == false) {
    channel[i].srcaddr = (channel[i].srcaddr & 0xff0000) + ((channel[i].srcaddr + channel[i].incmode) & 0xffff);
  }
  add_cycles(8);
  return --channel[i].xfersize;
}

uint16 bCPU::dma_mmiotocpu(uint8 i, uint8 index) {
uint8 x;
  x = mem_bus->read(0x2100 | ((channel[i].destaddr + index) & 0xff));
  mem_bus->write(channel[i].srcaddr, x);
  if(channel[i].fixedxfer == false) {
    channel[i].srcaddr = (channel[i].srcaddr & 0xff0000) + ((channel[i].srcaddr + channel[i].incmode) & 0xffff);
  }
  add_cycles(8);
  return --channel[i].xfersize;
}

void bCPU::dma_xfer_type0(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
  }
}

void bCPU::dma_xfer_type1(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 1) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 1) == 0)return;
  }
}

void bCPU::dma_xfer_type2(uint8 i) {
  if(channel[i].direction == 0) {
    if(dma_cputommio(i, 0) == 0)return;
    if(dma_cputommio(i, 0) == 0)return;
  } else {
    if(dma_mmiotocpu(i, 0) == 0)return;
    if(dma_mmiotocpu(i, 0) == 0)return;
  }
}

void bCPU::dma_xfer_type3(uint8 i) {
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

void bCPU::dma_xfer_type4(uint8 i) {
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

void bCPU::dma_xfer_type5(uint8 i) {
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

void bCPU::dma_run() {
int i;
  for(i=0;i<8;i++) {
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

  status.dma_state = DMASTATE_CPUSYNC;
}

void bCPU::hdma_write(uint8 i, uint8 l, uint8 x) {
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

void bCPU::hdma_run() {
int l, xferlen;
uint8 x, active_channels = 0;
  for(int i=0;i<8;i++) {
    if(channel[i].hdma_completed == true)continue;
//    add_cycles(8);
    active_channels++;
    if(channel[i].hdma_line_counter == 0) {
      channel[i].hdma_line_counter = mem_bus->read(channel[i].hdma_taddr++);
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
        channel[i].hdma_itaddr = channel[i].hdma_taddr;
      } else {
        channel[i].hdma_itaddr  = mem_bus->read(channel[i].hdma_taddr++);
        channel[i].hdma_itaddr |= mem_bus->read(channel[i].hdma_taddr++) << 8;
        channel[i].hdma_itaddr |= channel[i].hdma_indirect_bank << 16;
//        add_cycles(16);
      }
    }

    channel[i].hdma_line_counter--;
    if(channel[i].hdma_first_line == false && channel[i].hdma_repeat == false)continue;
    channel[i].hdma_first_line = false;

    if(channel[i].hdma_indirect == false) {
      channel[i].hdma_itaddr = channel[i].hdma_taddr;
    }

    switch(channel[i].xfermode) {
    case 0:                     xferlen = 1;break;
    case 1:case 2:case 6:       xferlen = 2;break;
    case 3:case 4:case 5:case 7:xferlen = 4;break;
    }

    for(l=0;l<xferlen;l++) {
      x = mem_bus->read(channel[i].hdma_itaddr++);
      if(channel[i].hdma_indirect == false) {
        channel[i].hdma_taddr++;
      }
      hdma_write(i, l, x);
//    add_cycles(8);
    }
  }

  if(active_channels != 0) {
//    add_cycles(18);
  }
}

void bCPU::hdma_initialize() {
uint8 active_channels = 0;
  for(int i=0;i<8;i++) {
    if(channel[i].hdma_active == false) {
      channel[i].hdma_completed = true;
      continue;
    }
    active_channels++;
    channel[i].hdma_first_line   = true;
    channel[i].hdma_repeat       = false;
    channel[i].hdma_taddr        = channel[i].srcaddr;
    channel[i].hdma_line_counter = 0x00;
    channel[i].hdma_completed    = false;
    if(channel[i].hdma_indirect == false) {
      add_cycles(8);
    } else {
      add_cycles(24);
    }
  }

  if(active_channels != 0) {
    add_cycles(18);
  }
}

void bCPU::dma_reset() {
  for(int i=0;i<8;i++) {
    channel[i].active             = false;
    channel[i].hdma_active        = false;
    channel[i].dmap               = 0x00;
    channel[i].direction          = 0;
    channel[i].hdma_indirect      = false;
    channel[i].incmode            = 1;
    channel[i].fixedxfer          = false;
    channel[i].xfermode           = 0;
    channel[i].destaddr           = 0;
    channel[i].srcaddr            = 0;
    channel[i].xfersize           = 0;
    channel[i].hdma_indirect_bank = 0;
    channel[i].hdma_taddr         = 0x000000;
    channel[i].hdma_line_counter  = 0x00;
    channel[i].hdma_unknown       = 0x00;

    channel[i].hdma_first_line    = false;
    channel[i].hdma_repeat        = false;
    channel[i].hdma_itaddr        = 0x000000;
    channel[i].hdma_completed     = true;
  }
}
