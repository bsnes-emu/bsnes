void sCPU::dma_add_clocks(uint clocks) {
  status.dma_clocks += clocks;
  add_clocks(clocks);
}

/*****
 * used by both DMA and HDMA
 *
 * DMA address bus A cannot read from or write to the following addresses :
 * $[00-3f|80-bf]:43[00-7f] <DMA control registers>
 * $[00-3f|80-bf]:420b      <DMA enable register>
 * $[00-3f|80-bf]:420c      <HDMA enable register>
 * WRAM<>WRAM transfers via $2180
 *****/
void sCPU::dma_transfer(bool direction, uint8 bbus, uint32 abus) {
uint8 r;
  if(direction == 0) { //a->b
    if((abus & 0x40ff00) == 0x2100 || (abus & 0x40ff80) == 0x4300 ||
       (abus & 0x40ffff) == 0x420b || (abus & 0x40ffff) == 0x420c) {
      r = regs.mdr;
    } else {
      r = r_mem->read(abus);
    }
    r_mem->write(0x2100 | bbus, r);
  } else { //b->a
    if(bbus == 0x80 && ((abus & 0x7e0000) == 0x7e0000 || (abus & 0x40e000) == 0x0000)) {
    //prevent WRAM->WRAM transfers
      r = regs.mdr;
    } else {
      r = r_mem->read(0x2100 | bbus);
    }
    if((abus & 0x40ff00) == 0x2100 || (abus & 0x40ff80) == 0x4300 ||
       (abus & 0x40ffff) == 0x420b || (abus & 0x40ffff) == 0x420c)return;
    r_mem->write(abus, r);
  }

  dma_add_clocks(8);
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

inline uint32 sCPU::dma_addr(uint8 i) {
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

inline uint32 sCPU::hdma_addr(uint8 i) {
  return (channel[i].srcbank << 16) | (channel[i].hdma_addr++);
}

inline uint32 sCPU::hdma_iaddr(uint8 i) {
  return (channel[i].hdma_ibank << 16) | (channel[i].hdma_iaddr++);
}

/*****
 * DMA functions
 *****/

void sCPU::dma_transfertobusb(uint8 i, uint8 bbus) {
  if(cartridge.info.sdd1 == true && sdd1.dma_active() == true) {
    r_mem->write(0x2100 | bbus, sdd1.dma_read());
  } else {
    dma_transfer(0, bbus, dma_addr(i));
  }
  channel[i].xfersize--;
}

void sCPU::dma_transfertobusa(uint8 i, uint8 bbus) {
  dma_transfer(1, bbus, dma_addr(i));
  channel[i].xfersize--;
}

inline void sCPU::dma_write(uint8 i, uint8 index) {
//cannot use dma_transfer() directly, due to current S-DD1 implementation
  if(channel[i].direction == 0) {
    dma_transfertobusb(i, index);
  } else {
    dma_transfertobusa(i, index);
  }
}

void sCPU::dma_run() {
  for(int i = 0; i < 8; i++) {
    if(channel[i].dma_enabled == false)continue;
    dma_add_clocks(8);

    if(cartridge.info.sdd1 == true) {
      sdd1.dma_begin(i, (channel[i].srcbank << 16) | (channel[i].srcaddr), channel[i].xfersize);
    }

    if(tracer.enabled() == true && tracer.cpudma_enabled() == true) {
      tprintf("[DMA] channel:%d direction:%s reverse:%c fixed:%c mode:%d b_addr:$21%0.2x "
              "a_addr:$%0.2x%0.4x length:$%0.4x (%5d)",
        i, channel[i].direction ? "b->a" : "a->b", channel[i].reversexfer ? '1' : '0',
        channel[i].fixedxfer ? '1' : '0', channel[i].xfermode, channel[i].destaddr,
        channel[i].srcbank, channel[i].srcaddr,
        channel[i].xfersize, channel[i].xfersize ? channel[i].xfersize : 65536);
    }

  uint index = 0;
    do {
      dma_write(i, dma_bbus(i, index++));
    } while(channel[i].dma_enabled && channel[i].xfersize);

    channel[i].dma_enabled = false;
  }

  counter.set(counter.irq_delay, 2);
}

/*****
 * HDMA functions
 *****/

inline bool sCPU::hdma_active(uint8 i) {
  return (channel[i].hdma_enabled && !channel[i].hdma_completed);
}

inline bool sCPU::hdma_active_after(uint8 i) {
  for(int n = i + 1; n < 8; n++) {
    if(hdma_active(n) == true) { return true; }
  }

  return false;
}

inline uint8 sCPU::hdma_enabled_channels() {
uint8 r = 0;
  for(int i = 0; i < 8; i++) {
    if(channel[i].hdma_enabled)r++;
  }
  return r;
}

inline uint8 sCPU::hdma_active_channels() {
uint8 r = 0;
  for(int i = 0; i < 8; i++) {
    if(hdma_active(i) == true)r++;
  }
  return r;
}

void sCPU::hdma_update(uint8 i) {
  channel[i].hdma_line_counter = r_mem->read(hdma_addr(i));
  dma_add_clocks(8);

  channel[i].hdma_completed   = (channel[i].hdma_line_counter == 0);
  channel[i].hdma_do_transfer = !channel[i].hdma_completed;

  if(channel[i].hdma_indirect) {
    channel[i].hdma_iaddr = r_mem->read(hdma_addr(i)) << 8;
    dma_add_clocks(8);

    if(!channel[i].hdma_completed || hdma_active_after(i)) {
      channel[i].hdma_iaddr >>= 8;
      channel[i].hdma_iaddr  |= r_mem->read(hdma_addr(i)) << 8;
      dma_add_clocks(8);
    }
  }
}

void sCPU::hdma_run() {
static uint8 hdma_xferlen[8] = { 1, 2, 2, 4, 4, 4, 2, 4 };
  for(int i = 0; i < 8; i++) {
    if(hdma_active(i) == false)continue;
    channel[i].dma_enabled = false; //HDMA run during DMA will stop DMA mid-transfer
    dma_add_clocks(8);

    if(channel[i].hdma_do_transfer) {
    int xferlen = hdma_xferlen[channel[i].xfermode];
      for(int index = 0; index < xferlen; index++) {
        if(bool(config::cpu.hdma_enable) == true) {
          dma_transfer(channel[i].direction, dma_bbus(i, index),
            !channel[i].hdma_indirect ? hdma_addr(i) : hdma_iaddr(i));
        } else {
          dma_add_clocks(8);
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

  counter.set(counter.irq_delay, 2);
}

void sCPU::hdma_init_reset() {
  for(int i = 0; i < 8; i++) {
    channel[i].hdma_completed   = false;
    channel[i].hdma_do_transfer = false;
  }
}

void sCPU::hdma_init() {
  for(int i = 0; i < 8; i++) {
    if(!channel[i].hdma_enabled)continue;
    channel[i].dma_enabled = false; //HDMA init during DMA will stop DMA mid-transfer

    channel[i].hdma_addr = channel[i].srcaddr;
    hdma_update(i);
  }

  counter.set(counter.irq_delay, 2);
}

/*****
 * power / reset functions
 *****/

void sCPU::dma_power() {
  for(int i = 0; i < 8; i++) {
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
    channel[i].dma_enabled       = false;
    channel[i].hdma_enabled      = false;

    channel[i].hdma_completed    = false;
    channel[i].hdma_do_transfer  = false;
  }
}
