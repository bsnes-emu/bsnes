/*****
 * used by both DMA and HDMA
 * prevents transfer across same bus (a->a or b->b)
 *
 * DMA address bus a cannot read from or write to the following addresses :
 * $[00-3f|80-bf]:21[00-ff] <address bus b>
 * $[00-3f|80-bf]:43[00-7f] <DMA control registers>
 * $[00-3f|80-bf]:420b      <DMA enable register>
 * $[00-3f|80-bf]:420c      <HDMA enable register>
 *
 * DMA address bus b cannot read from $2180
 *****/
void sCPU::dma_transfer(bool direction, uint8 bbus, uint32 abus) {
uint8 r;
  if(direction == 0) { //a->b
    if((abus & 0x40ff00) == 0x2100 || (abus & 0x40ff80) == 0x4300 ||
       (abus & 0x40ffff) == 0x420b || (abus & 0x40ffff) == 0x420c) {
      r = r_cpu->regs.mdr; //invalid reads return open bus
    } else {
      r = r_mem->read(abus);
    }
    r_mem->write(0x2100 | bbus, r);
  } else { //b->a
  uint8 r = (bbus == 0x80) ? r_cpu->regs.mdr : r_mem->read(0x2100 | bbus);
    if((abus & 0x40ff00) == 0x2100 || (abus & 0x40ff80) == 0x4300 ||
       (abus & 0x40ffff) == 0x420b || (abus & 0x40ffff) == 0x420c)return;
    r_mem->write(abus, r);
  }

  add_clocks(8);
  co_return();
  cycle_edge();
}

/*****
 * address calculation functions
 *****/

uint8 sCPU::dma_bbus(uint8 i, uint8 index) {
  switch(channel[i].xfermode) {
  default:
  case 0: return (channel[i].destaddr);                      break; //0
  case 1: return (channel[i].destaddr + (index & 1));        break; //0,1
  case 2: return (channel[i].destaddr);                      break; //0,0
  case 3: return (channel[i].destaddr + ((index >> 1) & 1)); break; //0,0,1,1
  case 4: return (channel[i].destaddr + (index & 3));        break; //0,1,2,3
  case 5: return (channel[i].destaddr + (index & 1));        break; //0,1,0,1
  case 6: return (channel[i].destaddr);                      break; //0,0     [2]
  case 7: return (channel[i].destaddr + ((index >> 1) & 1)); break; //0,0,1,1 [3]
  }
}

uint32 sCPU::dma_addr(uint8 i) {
uint32 r = (channel[i].srcbank << 16) | (channel[i].srcaddr);

  if(channel[i].fixedxfer == false) {
    if(channel[i].reversexfer == false) {
      channel[i].srcaddr++;
    } else {
      channel[i].srcaddr--;
    }
  }

  return r;
}

uint32 sCPU::hdma_addr(uint8 i) {
  return (channel[i].srcbank << 16) | (channel[i].hdma_addr++);
}

uint32 sCPU::hdma_iaddr(uint8 i) {
  return (channel[i].hdma_ibank << 16) | (channel[i].hdma_iaddr++);
}

/*****
 * DMA functions
 *****/

void sCPU::dma_transfertobusb(uint8 i, uint8 bbus) {
  if(cartridge.info.sdd1 == true && sdd1->dma_active() == true) {
    r_mem->write(0x2100 | bbus, sdd1->dma_read());
  } else {
    dma_transfer(0, bbus, dma_addr(i));
  }
  channel[i].xfersize--;
}

void sCPU::dma_transfertobusa(uint8 i, uint8 bbus) {
  dma_transfer(1, bbus, dma_addr(i));
  channel[i].xfersize--;
}

void sCPU::dma_write(uint8 i, uint8 index) {
//cannot use dma_transfer() directly,
//due to current S-DD1 implementation
  if(channel[i].direction == 0) {
    dma_transfertobusb(i, index);
  } else {
    dma_transfertobusa(i, index);
  }
}

void sCPU::dma_run() {
  add_clocks(18);

  for(int i = 0; i < 8; i++) {
    if(channel[i].dma_enabled == false)continue;
    add_clocks(8);

    if(cartridge.info.sdd1 == true) {
      sdd1->dma_begin(i, (channel[i].srcbank << 16) | (channel[i].srcaddr),
        channel[i].xfersize);
    }

  uint index = 0;
    do {
      dma_write(i, dma_bbus(i, index++));
    } while(channel[i].dma_enabled && channel[i].xfersize);

    channel[i].dma_enabled = false;
  }

  set_irq_delay(24);
}

/*****
 * HDMA functions
 *****/

bool sCPU::hdma_active(uint8 i) {
  return (channel[i].hdma_enabled && !channel[i].hdma_completed);
}

uint8 sCPU::hdma_enabled_channels() {
uint8 r = 0;
  for(int i = 0; i < 8; i++) {
    if(channel[i].hdma_enabled)r++;
  }
  return r;
}

uint8 sCPU::hdma_active_channels() {
uint8 r = 0;
  for(int i = 0; i < 8; i++) {
    if(hdma_active(i) == true)r++;
  }
  return r;
}

void sCPU::hdma_update(uint8 i) {
  channel[i].hdma_line_counter = r_mem->read(hdma_addr(i));
  add_clocks(8);

  if(channel[i].hdma_indirect) {
    channel[i].hdma_iaddr = r_mem->read(hdma_addr(i)) << 8;
    add_clocks(8);
  }

  if(channel[i].hdma_line_counter == 0) {
    channel[i].hdma_completed   = true;
    channel[i].hdma_do_transfer = false;
    return;
  }

  channel[i].hdma_do_transfer = true;

  if(channel[i].hdma_indirect) {
    channel[i].hdma_iaddr >>= 8;
    channel[i].hdma_iaddr  |= r_mem->read(hdma_addr(i)) << 8;
    add_clocks(8);
  }
}

void sCPU::hdma_run() {
  if(hdma_active_channels() == 0)return;

  add_clocks(18);

static uint8 hdma_xferlen[8] = { 1, 2, 2, 4, 4, 4, 2, 4 };
  for(int i = 0; i < 8; i++) {
    if(hdma_active(i) == false)continue;
    channel[i].dma_enabled = false; //HDMA run during DMA will stop DMA mid-transfer
    add_clocks(8);

    if(channel[i].hdma_do_transfer) {
    int xferlen = hdma_xferlen[channel[i].xfermode];
      for(int index = 0; index < xferlen; index++) {
        if(bool(config::cpu.hdma_enable) == true) {
          dma_transfer(channel[i].direction, dma_bbus(i, index),
            !channel[i].hdma_indirect ? hdma_addr(i) : hdma_iaddr(i));
        } else {
          add_clocks(8);
          co_return();
          cycle_edge();
        }
      }
    }

    channel[i].hdma_line_counter--;
    channel[i].hdma_do_transfer = bool(channel[i].hdma_line_counter & 0x80);
    if((channel[i].hdma_line_counter & 0x7f) == 0) {
      hdma_update(i);
    }
  }

  set_irq_delay(24);
}

void sCPU::hdma_init() {
  for(int i = 0; i < 8; i++) {
    channel[i].hdma_completed   = false;
    channel[i].hdma_do_transfer = false;
  }

  if(hdma_enabled_channels() == 0)return;

  add_clocks(18);

  for(int i = 0; i < 8; i++) {
    if(!channel[i].hdma_enabled)continue;
    channel[i].dma_enabled = false; //HDMA init during DMA will stop DMA mid-transfer

    channel[i].hdma_addr = channel[i].srcaddr;
    hdma_update(i);
  }

  set_irq_delay(24);
}

/*****
 * power / reset functions
 *****/

void sCPU::dma_power() {
  for(int i = 0; i < 8; i++) {
    channel[i].dma_enabled       = false;
    channel[i].hdma_enabled      = false;

    channel[i].dmap              = 0xff;
    channel[i].direction         = 1;
    channel[i].hdma_indirect     = true;
    channel[i].reversexfer       = true;
    channel[i].fixedxfer         = true;
    channel[i].xfermode          = 7;

    channel[i].destaddr          = 0xff;

    channel[i].srcaddr           = 0xffff;
    channel[i].srcbank           = 0xff;

    channel[i].xfersize          = 0xffff;
  //channel[i].hdma_iaddr        = 0xffff; //union with xfersize
    channel[i].hdma_ibank        = 0xff;

    channel[i].hdma_addr         = 0xffff;
    channel[i].hdma_line_counter = 0xff;
    channel[i].unknown           = 0xff;
  }
}

void sCPU::dma_reset() {
  for(int i = 0; i < 8; i++) {
    channel[i].hdma_completed    = false;
    channel[i].hdma_do_transfer  = false;
  }
}
