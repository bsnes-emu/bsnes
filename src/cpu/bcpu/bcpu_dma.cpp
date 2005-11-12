void bCPU::dma_add_cycles(uint32 cycles) {
  status.dma_cycle_count += cycles;
}

void bCPU::hdma_add_cycles(uint32 cycles) {
  if(run_state.dma) {
    status.dma_cycle_count += cycles;
  }
  status.hdma_cycle_count += cycles;
}

uint32 bCPU::dma_addr(uint8 i) {
uint32 r;
  r = (channel[i].srcbank << 16) | (channel[i].srcaddr);

  if(channel[i].fixedxfer == false) {
    channel[i].srcaddr += channel[i].incmode;
  }

  return r;
}

void bCPU::dma_cputommio(uint8 i, uint8 index) {
uint8 x;
  if(sdd1->dma_active() == true) {
    x = sdd1->dma_read();
  } else {
    x = r_mem->read(dma_addr(i));
  }

  r_mem->write(0x2100 | ((channel[i].destaddr + index) & 0xff), x);

  add_cycles(8);
  channel[i].xfersize--;
}

void bCPU::dma_mmiotocpu(uint8 i, uint8 index) {
uint8 x;
  x = r_mem->read(0x2100 | ((channel[i].destaddr + index) & 0xff));
  r_mem->write(dma_addr(i), x);

  add_cycles(8);
  channel[i].xfersize--;
}

void bCPU::dma_write(uint8 i, uint8 index) {
  if(channel[i].direction == 0) {
    dma_cputommio(i, index);
  } else {
    dma_mmiotocpu(i, index);
  }
}

void bCPU::dma_run() {
int i;
  for(i=0;i<8;i++) {
    if(channel[i].active == false)continue;

  //first byte transferred?
    if(channel[i].read_index == 0) {
      sdd1->dma_begin(i, (channel[i].srcbank << 16) | (channel[i].srcaddr),
        channel[i].xfersize);
    }

    switch(channel[i].xfermode) {
    case 0:dma_write(i, 0);                               break; //0
    case 1:dma_write(i, channel[i].read_index & 1);       break; //0,1
    case 2:dma_write(i, 0);                               break; //0,0
    case 3:dma_write(i, (channel[i].read_index >> 1) & 1);break; //0,0,1,1
    case 4:dma_write(i, channel[i].read_index & 3);       break; //0,1,2,3
    case 5:dma_write(i, channel[i].read_index & 1);       break; //0,1,0,1
    case 6:dma_write(i, 0);                               break; //0,0     [2]
    case 7:dma_write(i, (channel[i].read_index >> 1) & 1);break; //0,0,1,1 [3]
    }

    channel[i].read_index++;
    dma_add_cycles(8);

    if(channel[i].xfersize == 0) {
      channel[i].active = false;
    }

    return;
  }

  status.dma_state = DMASTATE_CPUSYNC;
}

uint32 bCPU::hdma_addr(uint8 i) {
  return (channel[i].srcbank << 16) | (channel[i].hdma_addr++);
}

uint32 bCPU::hdma_iaddr(uint8 i) {
  return (channel[i].hdma_ibank << 16) | (channel[i].hdma_iaddr++);
}

uint16 bCPU::hdma_mmio(uint8 i) {
uint8  l = channel[i].read_index;
uint16 index;
  switch(channel[i].xfermode) {
  case 0:index = 0;           break; //0
  case 1:index = l & 1;       break; //0,1
  case 2:index = 0;           break; //0,0
  case 3:index = (l >> 1) & 1;break; //0,0,1,1
  case 4:index = l & 3;       break; //0,1,2,3
  case 5:index = l & 1;       break; //0,1,0,1
  case 6:index = 0;           break; //0,0     [2]
  case 7:index = (l >> 1) & 1;break; //0,0,1,1 [3]
  }

  return (0x2100 | ((channel[i].destaddr + index) & 0xff));
}

uint8 bCPU::hdma_read(uint8 i) {
  if(channel[i].direction == DMA_MMIOTOCPU) {
    return r_mem->read(hdma_mmio(i));
  } else if(!channel[i].hdma_indirect) {
    return r_mem->read(hdma_addr(i));
  } else {
    return r_mem->read(hdma_iaddr(i));
  }
}

void bCPU::hdma_write(uint8 i, uint8 x) {
  if(channel[i].direction == DMA_CPUTOMMIO) {
    r_mem->write(hdma_mmio(i), x);
  } else if(!channel[i].hdma_indirect) {
    r_mem->write(hdma_addr(i), x);
  } else {
    r_mem->write(hdma_iaddr(i), x);
  }

  add_cycles(8);
  hdma_add_cycles(8);
}

void bCPU::hdma_update(uint8 i) {
  channel[i].hdma_line_counter = r_mem->read(hdma_addr(i));
  add_cycles(8);
  hdma_add_cycles(8);

  if(channel[i].hdma_indirect) {
    channel[i].hdma_iaddr = r_mem->read(hdma_addr(i)) << 8;
    add_cycles(8);
    hdma_add_cycles(8);
  }

  if(channel[i].hdma_line_counter == 0) {
    channel[i].hdma_active      = false;
    channel[i].hdma_do_transfer = false;
    return;
  }

  channel[i].hdma_do_transfer = true;

  if(channel[i].hdma_indirect) {
    channel[i].hdma_iaddr >>= 8;
    channel[i].hdma_iaddr  |= r_mem->read(hdma_addr(i)) << 8;
    add_cycles(8);
    hdma_add_cycles(8);
  }
}

void bCPU::hdma_run() {
static uint8 hdma_xferlen[8] = { 1, 2, 2, 4, 4, 4, 2, 4 };
  for(int i=0;i<8;i++) {
    if(!channel[i].hdma_enabled || !channel[i].hdma_active)continue;

    if(channel[i].hdma_do_transfer) {
    int xferlen = hdma_xferlen[channel[i].xfermode];
      for(channel[i].read_index=0;channel[i].read_index<xferlen;channel[i].read_index++) {
        hdma_write(i, hdma_read(i));
      }
    }

    channel[i].hdma_line_counter--;
    channel[i].hdma_do_transfer = !!(channel[i].hdma_line_counter & 0x80);
    if((channel[i].hdma_line_counter & 0x7f) == 0) {
      hdma_update(i);
    }
  }
}

uint8 bCPU::hdma_enabled_channels() {
int r = 0;
  for(int i=0;i<8;i++) {
    if(channel[i].hdma_enabled)r++;
  }
  return r;
}

uint8 bCPU::hdma_active_channels() {
int r = 0;
  for(int i=0;i<8;i++) {
    if(channel[i].hdma_enabled && channel[i].hdma_active)r++;
  }
  return r;
}

/* hdmainit_activate()
 * hdma_activate()
 *
 * Functions are called by CPU timing routine
 * when an HDMA event (init or run) occurs.
 */

void bCPU::hdmainit_activate() {
  for(int i=0;i<8;i++) {
    channel[i].hdma_active      = false;
    channel[i].hdma_do_transfer = false;
  }

  if(hdma_enabled_channels() != 0) {
    status.hdma_state = HDMASTATE_IDMASYNC;
    run_state.hdma = true;
  }
}

void bCPU::hdma_activate() {
  if(hdma_active_channels() != 0) {
    status.hdma_state = HDMASTATE_DMASYNC;
    run_state.hdma = true;
  }
}

void bCPU::dma_reset() {
  for(int i=0;i<8;i++) {
    channel[i].read_index        = 0;
    channel[i].active            = false;
    channel[i].hdma_enabled      = false;
    channel[i].dmap              = 0x00;
    channel[i].direction         = 0;
    channel[i].hdma_indirect     = false;
    channel[i].incmode           = 1;
    channel[i].fixedxfer         = false;
    channel[i].xfermode          = 0;
    channel[i].destaddr          = 0;
    channel[i].srcaddr           = 0;
    channel[i].xfersize          = 0x0000;
  //xfersize and hdma_iaddr are of union { uint16 };
  //channel[i].hdma_iaddr        = 0x0000;
    channel[i].hdma_ibank        = 0;
    channel[i].hdma_addr         = 0x0000;
    channel[i].hdma_line_counter = 0x00;
    channel[i].hdma_unknown      = 0x00;

    channel[i].hdma_active       = false;
    channel[i].hdma_do_transfer  = false;
  }
}
