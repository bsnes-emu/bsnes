#define DMATRANSFER_CPUTOMMIO 0
#define DMATRANSFER_MMIOTOCPU 1

#define DMAINDEX_ABSOLUTE 0
#define DMAINDEX_INDIRECT 1

#define DMAWRITE_INC 0
#define DMAWRITE_DEC 1

#define HDMAMODE_NORMAL     0
#define HDMAMODE_CONTINUOUS 1

typedef struct {
  bool  active;
  byte  transfer_mode;
  bool  indirect;
  bool  fixed_address;
  int   write_dir;
  byte  transfer_type;
  byte  dest_addr;
  ulong src_addr;
  word  transfer_size;
  byte  indirect_bank;
  byte  hdma_mode;
  word  hdma_indirect_pointer;

//hdma specific
  bool  first_line;
  bool  repeat;
  bool  completed;
  byte  line_counter;
  ulong address, iaddress;

  word  r43x8;
}dmachannel;

dmachannel dma_channel[8];

/*
  $43x0 : DMA control
  da-ifttt

  d: (dma only)
     0=read from cpu mem, write to $21xx
     1=read from $21xx, write to cpu mem
  a: (hdma only)
     0=absolute addressing
     1=indirect addressing
  i: 0=increment address, 1=decrement address
  f: 1=fixed address, 0=inc/dec address
  t: transfer type
*/
void mmio_w43x0(byte c, byte value) {
  dma_channel[c].transfer_mode = (value & 0x80)?DMATRANSFER_MMIOTOCPU:DMATRANSFER_CPUTOMMIO;
  dma_channel[c].indirect      = (value & 0x40)?true:false;
  dma_channel[c].write_dir     = (value & 0x10)?-1:1;
  dma_channel[c].fixed_address = (value & 0x08)?true:false;
  dma_channel[c].transfer_type = (value & 0x07);
}

/*
  $43x1 : DMA destination address
  bbbbbbbb

  b: $2100 | b = destination register - limited to $21xx regs only
*/
void mmio_w43x1(byte c, byte value) {
  dma_channel[c].dest_addr = value;
}

/*
  $43x2-$43x4 : 24-bit DMA source address

  after a dma transfer, this address must be incremented
*/
void mmio_w43x2(byte c, byte value) {
  dma_channel[c].src_addr = (dma_channel[c].src_addr & 0xffff00) | value;
}

void mmio_w43x3(byte c, byte value) {
  dma_channel[c].src_addr = (dma_channel[c].src_addr & 0xff00ff) | (value << 8);
}

void mmio_w43x4(byte c, byte value) {
  dma_channel[c].src_addr = (dma_channel[c].src_addr & 0x00ffff) | (value << 16);
}

/*
  $43x5/$43x6 : DMA transfer size
*/
void mmio_w43x5(byte c, byte value) {
  dma_channel[c].transfer_size = (dma_channel[c].transfer_size & 0xff00) | value;
}

void mmio_w43x6(byte c, byte value) {
  dma_channel[c].transfer_size = (dma_channel[c].transfer_size & 0x00ff) | (value << 8);
}

/*
  $43x7 : HDMA indirect bank address
*/
void mmio_w43x7(byte c, byte value) {
  dma_channel[c].indirect_bank = value;
}

void dma_mmio_write(byte reg, byte value) {
  mmio_write(0x2100 | reg, value);
}

byte dma_mmio_read(byte reg) {
  return mmio_read(0x2100 | reg);
}

word dma_cputommio(byte c, byte a) {
byte x;
  x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr);
  dma_mmio_write(dma_channel[c].dest_addr + a, x);
  if(dma_channel[c].fixed_address == false) {
    dma_channel[c].src_addr = (dma_channel[c].src_addr & 0xff0000) | ((dma_channel[c].src_addr + dma_channel[c].write_dir) & 0xffff);
  }
  snes_time->add_cpu_cycles(1, 8);
  return --dma_channel[c].transfer_size;
}

word dma_mmiotocpu(byte c, byte a) {
byte x;
  x = dma_mmio_read(dma_channel[c].dest_addr + a);
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr, x);
  if(dma_channel[c].fixed_address == false) {
    dma_channel[c].src_addr = (dma_channel[c].src_addr & 0xff0000) | ((dma_channel[c].src_addr + dma_channel[c].write_dir) & 0xffff);
  }
  snes_time->add_cpu_cycles(1, 8);
  return --dma_channel[c].transfer_size;
}

void dma_transfer_type0(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    if(dma_cputommio(c, 0) == 0)return;
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    if(dma_mmiotocpu(c, 0) == 0)return;
  }
}

void dma_transfer_type1(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    if(dma_cputommio(c, 0) == 0)return;
    if(dma_cputommio(c, 1) == 0)return;
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    if(dma_mmiotocpu(c, 0) == 0)return;
    if(dma_mmiotocpu(c, 1) == 0)return;
  }
}

void dma_transfer_type2(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    if(dma_cputommio(c, 0) == 0)return;
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    if(dma_mmiotocpu(c, 0) == 0)return;
  }
}

void dma_transfer_type3(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    if(dma_cputommio(c, 0) == 0)return;
    if(dma_cputommio(c, 0) == 0)return;
    if(dma_cputommio(c, 1) == 0)return;
    if(dma_cputommio(c, 1) == 0)return;
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    if(dma_mmiotocpu(c, 0) == 0)return;
    if(dma_mmiotocpu(c, 0) == 0)return;
    if(dma_mmiotocpu(c, 1) == 0)return;
    if(dma_mmiotocpu(c, 1) == 0)return;
  }
}

void dma_transfer_type4(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    if(dma_cputommio(c, 0) == 0)return;
    if(dma_cputommio(c, 1) == 0)return;
    if(dma_cputommio(c, 2) == 0)return;
    if(dma_cputommio(c, 3) == 0)return;
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    if(dma_mmiotocpu(c, 0) == 0)return;
    if(dma_mmiotocpu(c, 1) == 0)return;
    if(dma_mmiotocpu(c, 2) == 0)return;
    if(dma_mmiotocpu(c, 3) == 0)return;
  }
}

void dma_transfer_type5(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    if(dma_cputommio(c, 0) == 0)return;
    if(dma_cputommio(c, 1) == 0)return;
    if(dma_cputommio(c, 0) == 0)return;
    if(dma_cputommio(c, 1) == 0)return;
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    if(dma_mmiotocpu(c, 0) == 0)return;
    if(dma_mmiotocpu(c, 1) == 0)return;
    if(dma_mmiotocpu(c, 0) == 0)return;
    if(dma_mmiotocpu(c, 1) == 0)return;
  }
}

/*
  This function is called consecutively until all DMA transfers are completed.
  Rather than transfer all of the DMA data immediately, control is returned to
  the main loop after each DMA transfer so that the APU, PPU, renderer, etc.
  can keep in sync. Otherwise, a transfer of 65536 bytes could prevent the renderer
  from drawing all scanlines, for example.
  Each DMA channel must be completed incrementally. The entire transfer from channel 0
  must complete before any data is transferred for channel 1, etc.
*/
void ppu_update_dma(void) {
int i, z;
  z = 0;
  for(i=0;i<8;i++) {
    if(dma_channel[i].active == false)continue;
    switch(dma_channel[i].transfer_type) {
    case 0:dma_transfer_type0(i);break;
    case 1:dma_transfer_type1(i);break;
    case 2:dma_transfer_type2(i);break;
    case 3:dma_transfer_type3(i);break;
    case 4:dma_transfer_type4(i);break;
    case 5:dma_transfer_type5(i);break;
    case 6:dma_transfer_type2(i);break; //6 is the same as 2
    case 7:dma_transfer_type3(i);break; //7 is the same as 3
    }
    if(dma_channel[i].transfer_size == 0) {
      dma_channel[i].active = false;
    }
    return;
  }
  gx816->cpu_state = CPUSTATE_RUN;
}

/*
  $420b : DMA  enable
  $420c : HDMA enable

  Each bit corresponds to the respecting DMA channel (7,6,5,4,3,2,1,0)
  Setting a bit in this register will perform the DMA transfer. Multiple
  transfers can be done at once. Requires one cycle per byte transferred.
*/
void mmio_w420b(byte value) {
int i;
  ppu.active_hdma_channels &= ~value;
  for(i=0;i<8;i++) {
    if(value & (1 << i)) {
      dma_channel[i].active = true;
      gx816->cpu_state = CPUSTATE_DMA;
    }
  }
}

void mmio_w420c(byte value) {
  ppu.active_hdma_channels = value;
}

byte hdma_transfer_lentbl[8] = { 1, 2, 2, 4, 4, 4, 2, 4 };

void hdma_write_byte(byte i, byte l, byte x) {
  switch(dma_channel[i].transfer_type) {
  case 0:
    dma_mmio_write(dma_channel[i].dest_addr, x);
    break;
  case 1:
    dma_mmio_write(dma_channel[i].dest_addr + l, x);
    break;
  case 2:
    dma_mmio_write(dma_channel[i].dest_addr, x);
    break;
  case 3:
    dma_mmio_write(dma_channel[i].dest_addr + (l >> 1), x);
    break;
  case 4:
    dma_mmio_write(dma_channel[i].dest_addr + l, x);
    break;
  case 5:
    dma_mmio_write(dma_channel[i].dest_addr + (l & 1), x);
    break;
  case 6:
    dma_mmio_write(dma_channel[i].dest_addr, x);
    break;
  case 7:
    dma_mmio_write(dma_channel[i].dest_addr + (l >> 1), x);
    break;
  }
}

void hdma_update(void) {
int  i, l;
byte x, channels_active = 0;
  if(snes_time->vscan_pos > ppu.visible_scanlines)return;
  for(i=0;i<8;i++) {
    if(dma_channel[i].completed == true)continue;
    snes_time->add_cpu_cycles(1, 8);
    channels_active++;
    if(dma_channel[i].line_counter == 0) {
      dma_channel[i].line_counter = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[i].address++);
      dma_channel[i].r43x8 = dma_channel[i].address;
      if(dma_channel[i].line_counter == 0) {
        dma_channel[i].completed = true;
        continue;
      }

      if(dma_channel[i].line_counter > 0x80) {
        dma_channel[i].repeat = true;
        dma_channel[i].line_counter -= 0x80;
      } else {
        dma_channel[i].repeat = false;
      }

      dma_channel[i].first_line = true;
      if(dma_channel[i].indirect == false) {
        dma_channel[i].iaddress = dma_channel[i].address;
      } else {
        dma_channel[i].iaddress  = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dma_channel[i].address);
        dma_channel[i].iaddress |= (dma_channel[i].indirect_bank << 16);
        dma_channel[i].address  += 2;
        snes_time->add_cpu_cycles(1, 16);
      }
    }

    dma_channel[i].line_counter--;
    if(dma_channel[i].first_line == false && dma_channel[i].repeat == false)continue;
    dma_channel[i].first_line = false;

    if(dma_channel[i].indirect == false) {
      dma_channel[i].iaddress = dma_channel[i].address;
    }

    for(l=0;l<hdma_transfer_lentbl[dma_channel[i].transfer_type];l++) {
      x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[i].iaddress++);
      if(dma_channel[i].indirect == false) {
        dma_channel[i].address++;
      }
      hdma_write_byte(i, l, x);
      snes_time->add_cpu_cycles(1, 8);
    }
  }
  if(channels_active != 0) {
    snes_time->add_cpu_cycles(1, 18);
  }
}

void hdma_initialize(void) {
int i, active_channels = 0;
  for(i=0;i<8;i++) {
    if((ppu.active_hdma_channels & (1 << i)) == 0) {
      dma_channel[i].completed = true;
      continue;
    }
    active_channels++;
    dma_channel[i].first_line   = true;
    dma_channel[i].repeat       = false;
    dma_channel[i].line_counter = 0;
    dma_channel[i].address      = dma_channel[i].src_addr;
    dma_channel[i].completed    = false;
    if(dma_channel[i].indirect == false) {
      snes_time->add_cpu_cycles(1, 8);
    } else {
      snes_time->add_cpu_cycles(1, 24);
    }
  }
  if(active_channels != 0) {
    snes_time->add_cpu_cycles(1, 18);
  }
}

byte mmio_r43x8(byte c) {
  return (dma_channel[c].r43x8);
}

byte mmio_r43x9(byte c) {
  return (dma_channel[c].r43x8 >> 8);
}

byte mmio_r43xa(byte c) {
  return (dma_channel[c].line_counter + 1);
}

void mmio_w43x8(byte c, byte x) {
  dma_channel[c].address = (dma_channel[c].address & 0xff00) | x;
}

void mmio_w43x9(byte c, byte x) {
  dma_channel[c].address = (dma_channel[c].address & 0x00ff) | (x << 8);
}

void mmio_w43xa(byte c, byte x) {
  dma_channel[c].line_counter = x;
}
