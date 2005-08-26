uint32 bCPU::dma_addr(uint8 i) {
uint32 r;
  r  = channel[i].srcaddr;
  r |= (channel[i].srcbank << 16);
  return r;
}

void bCPU::dma_cputommio(uint8 i, uint8 index) {
uint8 x;
  if(sdd1->dma_active() == true) {
    x = sdd1->dma_read();
  } else {
    x = mem_bus->read(dma_addr(i));
  }

  mem_bus->write(0x2100 | ((channel[i].destaddr + index) & 0xff), x);

  if(channel[i].fixedxfer == false) {
    channel[i].srcaddr += channel[i].incmode;
  }

  add_cycles(8);
  channel[i].xfersize--;
}

void bCPU::dma_mmiotocpu(uint8 i, uint8 index) {
uint8 x;
  x = mem_bus->read(0x2100 | ((channel[i].destaddr + index) & 0xff));
  mem_bus->write(dma_addr(i), x);

  if(channel[i].fixedxfer == false) {
    channel[i].srcaddr += channel[i].incmode;
  }

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
      sdd1->dma_begin(i, dma_addr(i), channel[i].xfersize);
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

    if(channel[i].xfersize == 0) {
      channel[i].active = false;
    }

    return;
  }

  status.dma_state = DMASTATE_CPUSYNC;
}

uint32 bCPU::hdma_addr(uint8 i) {
uint32 r;
  r  = channel[i].hdma_addr;
  r |= (channel[i].srcbank << 16);
  channel[i].hdma_addr++;
  return r;
}

uint32 bCPU::hdma_iaddr(uint8 i) {
uint32 r;
  r  = channel[i].hdma_iaddr;
  r |= (channel[i].hdma_ibank << 16);
  channel[i].hdma_iaddr++;
  return r;
}

void bCPU::hdma_write(uint8 i, uint8 l, uint8 x) {
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

  index = 0x2100 | ((channel[i].destaddr + index) & 0xff);
  mem_bus->write(index, x);
}

void bCPU::hdma_run() {
int l, xferlen;
uint8 x, active_channels = 0;
static hdma_xferlen[8] = { 1, 2, 2, 4, 4, 4, 2, 4 };
  for(int i=0;i<8;i++) {
    if(channel[i].hdma_active == false)continue;

//  add_cycles(8);
    active_channels++;

    if(channel[i].hdma_line_counter == 0) {
      channel[i].hdma_line_counter = mem_bus->read(hdma_addr(i));

      if(channel[i].hdma_line_counter == 0) {
        channel[i].hdma_active = false;
        continue;
      }

      if(channel[i].hdma_line_counter > 0x80) {
        channel[i].hdma_repeat = true;
        channel[i].hdma_line_counter -= 0x80;
      } else {
        channel[i].hdma_repeat = false;
      }

      channel[i].hdma_first_line = true;

      if(channel[i].hdma_indirect == true) {
        channel[i].hdma_iaddr  = mem_bus->read(hdma_addr(i));
        channel[i].hdma_iaddr |= mem_bus->read(hdma_addr(i)) << 8;
//      add_cycles(16);
      }
    }

    channel[i].hdma_line_counter--;
    if(channel[i].hdma_first_line == false && channel[i].hdma_repeat == false)continue;
    channel[i].hdma_first_line = false;

    xferlen = hdma_xferlen[channel[i].xfermode];
    for(l=0;l<xferlen;l++) {
      if(channel[i].hdma_indirect == false) {
        x = mem_bus->read(hdma_addr(i));
      } else {
        x = mem_bus->read(hdma_iaddr(i));
      }

      hdma_write(i, l, x);
//    add_cycles(8);
    }
  }

  if(active_channels != 0) {
//  add_cycles(18);
  }
}

void bCPU::hdma_initialize() {
uint8 active_channels = 0;
  for(int i=0;i<8;i++) {
  //does this happen when $420c channel bit is clear?
    channel[i].hdma_addr         = channel[i].srcaddr;
    channel[i].hdma_line_counter = 0x00;

    if(channel[i].hdma_enabled == false) {
      channel[i].hdma_active = false;
      continue;
    }

    channel[i].hdma_active = true;
    active_channels++;

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
    channel[i].xfersize          = 0;
    channel[i].hdma_ibank        = 0;
    channel[i].hdma_addr         = 0x0000;
    channel[i].hdma_line_counter = 0x00;
    channel[i].hdma_unknown      = 0x00;

    channel[i].hdma_active       = false;
    channel[i].hdma_first_line   = false;
    channel[i].hdma_repeat       = false;
    channel[i].hdma_iaddr        = 0x0000;
  }
}
