#define DMATRANSFER_CPUTOMMIO 0
#define DMATRANSFER_MMIOTOCPU 1

#define DMAINDEX_ABSOLUTE 0
#define DMAINDEX_INDIRECT 1

#define DMAWRITE_INC 0
#define DMAWRITE_DEC 1

#define HDMAMODE_NORMAL     0
#define HDMAMODE_CONTINUOUS 1

typedef struct {
  byte  transfer_mode;
  byte  index_mode;
  bool  fixed_address;
  byte  write_dir;
  byte  transfer_type;
  word  dest_addr;
  ulong src_addr;
  word  transfer_size;
  byte  indirect_bank_address;
  byte  hdma_mode;
  word  hdma_indirect_pointer;
}dmachannel;

dmachannel dma_channel[8];

/*
  $43x0 : DMA control
  da-ifttt

  d: (dma only)
     0=read from cpu mem, write to $21??
     1=read from $21??, write to cpu mem
  a: (hdma only)
     0=absolute addressing
     1=indirect addressing
  f: 1=fixed address, 0=inc/dec address
  i: 0=increment address, 1=decrement address
  t: transfer type
     000: 1 reg  1 write  (01->2118 23->2118 45->2118 67->2118 89->2118)
     001: 2 regs 1 write  (01->2118 23->2119 45->2118 67->2119 89->2118)
     010: 1 reg  2 writes (01->2118 23->2118 45->2118 67->2118 89->2118)
     011: 2 regs 2 writes (01->2118 23->2118 45->2119 67->2119 89->2118)
     100: 4 regs 1 write  (01->2118 23->2119 45->211a 67->211b 89->2118)
     101-111: unknown
*/
void mmio_w43x0(byte c, byte value) {
  dma_channel[c].transfer_mode = (value & 0x80)?DMATRANSFER_MMIOTOCPU:DMATRANSFER_CPUTOMMIO;
  dma_channel[c].index_mode    = (value & 0x40)?DMAINDEX_INDIRECT:DMAINDEX_ABSOLUTE;
  dma_channel[c].write_dir     = (value & 0x10)?DMAWRITE_DEC:DMAWRITE_INC;
  dma_channel[c].fixed_address = (value & 0x08)?true:false;
  dma_channel[c].transfer_type = (value & 0x07);
}

/*
  $43x1 : DMA destination address
  bbbbbbbb

  b: $2100 | b = destination register - limited to $21xx regs only
*/
void mmio_w43x1(byte c, byte value) {
  dma_channel[c].dest_addr = 0x2100 | value;
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
  dma_channel[c].indirect_bank_address = value;
}

void dma_cputommio(byte c, byte a) {
byte x;
  x = gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, dma_channel[c].src_addr);
  mmio_write(dma_channel[c].dest_addr + a, x);
  if(dma_channel[c].fixed_address == false) {
    if(dma_channel[c].write_dir == DMAWRITE_INC)dma_channel[c].src_addr++;
    if(dma_channel[c].write_dir == DMAWRITE_DEC)dma_channel[c].src_addr--;
  }
  dma_channel[c].transfer_size--;
}

void dma_mmiotocpu(byte c, byte a) {
byte x;
  x = mmio_read(dma_channel[c].dest_addr + a);
  gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, dma_channel[c].src_addr, x);
  if(dma_channel[c].fixed_address == false) {
    if(dma_channel[c].write_dir == DMAWRITE_INC)dma_channel[c].src_addr++;
    if(dma_channel[c].write_dir == DMAWRITE_DEC)dma_channel[c].src_addr--;
  }
  dma_channel[c].transfer_size--;
}

void dma_transfer_type0(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    while(1) {
      dma_cputommio(c, 0); if(dma_channel[c].transfer_size == 0)break;
    }
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    while(1) {
      dma_mmiotocpu(c, 0); if(dma_channel[c].transfer_size == 0)break;
    }
  }
}

void dma_transfer_type1(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    while(1) {
      dma_cputommio(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_cputommio(c, 1); if(dma_channel[c].transfer_size == 0)break;
    }
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    while(1) {
      dma_mmiotocpu(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_mmiotocpu(c, 1); if(dma_channel[c].transfer_size == 0)break;
    }
  }
}

void dma_transfer_type2(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    while(1) {
      dma_cputommio(c, 0); if(dma_channel[c].transfer_size == 0)break;
    }
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    while(1) {
      dma_mmiotocpu(c, 0); if(dma_channel[c].transfer_size == 0)break;
    }
  }
}

void dma_transfer_type3(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    while(1) {
      dma_cputommio(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_cputommio(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_cputommio(c, 1); if(dma_channel[c].transfer_size == 0)break;
      dma_cputommio(c, 1); if(dma_channel[c].transfer_size == 0)break;
    }
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    while(1) {
      dma_mmiotocpu(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_mmiotocpu(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_mmiotocpu(c, 1); if(dma_channel[c].transfer_size == 0)break;
      dma_mmiotocpu(c, 1); if(dma_channel[c].transfer_size == 0)break;
    }
  }
}

void dma_transfer_type4(byte c) {
  if(dma_channel[c].transfer_mode == DMATRANSFER_CPUTOMMIO) {
    while(1) {
      dma_cputommio(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_cputommio(c, 1); if(dma_channel[c].transfer_size == 0)break;
      dma_cputommio(c, 2); if(dma_channel[c].transfer_size == 0)break;
      dma_cputommio(c, 3); if(dma_channel[c].transfer_size == 0)break;
    }
  } else if(dma_channel[c].transfer_mode == DMATRANSFER_MMIOTOCPU) {
    while(1) {
      dma_mmiotocpu(c, 0); if(dma_channel[c].transfer_size == 0)break;
      dma_mmiotocpu(c, 1); if(dma_channel[c].transfer_size == 0)break;
      dma_mmiotocpu(c, 2); if(dma_channel[c].transfer_size == 0)break;
      dma_mmiotocpu(c, 3); if(dma_channel[c].transfer_size == 0)break;
    }
  }
}

/*
  $420b : DMA  enable
  $420c : HDMA enable

  each bit corresponds to the respecting DMA channel (7,6,5,4,3,2,1,0)
  setting a bit in this register will perform the DMA transfer. multiple
  transfers can be done at once. requires one cycle per byte transferred
  hdma bits are sticky (they are not cleared automatically after write)
*/
void mmio_w420b(byte value) {
int i;
  for(i=0;i<8;i++) {
    if(value & (1 << i)) {
      snes_time->add_cpu_cycles(dma_channel[i].transfer_size * 6);
      if     (dma_channel[i].transfer_type == 0)dma_transfer_type0(i);
      else if(dma_channel[i].transfer_type == 1)dma_transfer_type1(i);
      else if(dma_channel[i].transfer_type == 2)dma_transfer_type2(i);
      else if(dma_channel[i].transfer_type == 3)dma_transfer_type3(i);
      else if(dma_channel[i].transfer_type == 4)dma_transfer_type4(i);
      else dprintf("* mmio_w420b(): Unknown DMA transfer type: %d", dma_channel[i].transfer_type);
    }
  }
}

void mmio_w420c(byte value) {
//don't actually enable hdma channels until the start of the next frame
  ppu.toggle_active_hdma_channels = value;
}

byte hdma_read_absolute(byte c) {
ulong x;
  x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr + ppu.hdma_index_pointer[c]);
  ppu.hdma_index_pointer[c]++;
  return x;
}

byte hdma_read_indirect(byte c) {
ulong x;
  x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_WORD, dma_channel[c].src_addr + ppu.hdma_index_pointer[c]);
  x += dma_channel[c].hdma_indirect_pointer;
  x = (dma_channel[c].indirect_bank_address << 16) | (x & 0xffff);
  x = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, x);
  dma_channel[c].hdma_indirect_pointer++;
  return x;
}

void hdma_transfer_type0_absolute(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_absolute(c));
}

void hdma_transfer_type0_indirect(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_indirect(c));
  if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL)dma_channel[c].hdma_indirect_pointer -= 1;
}

void hdma_transfer_type1_absolute(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_absolute(c));
}

void hdma_transfer_type1_indirect(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_indirect(c));
  if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL)dma_channel[c].hdma_indirect_pointer -= 2;
}

void hdma_transfer_type2_absolute(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr,     hdma_read_absolute(c));
}

void hdma_transfer_type2_indirect(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr,     hdma_read_indirect(c));
  if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL)dma_channel[c].hdma_indirect_pointer -= 2;
}

void hdma_transfer_type3_absolute(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr,     hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_absolute(c));
}

void hdma_transfer_type3_indirect(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr,     hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_indirect(c));
  if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL)dma_channel[c].hdma_indirect_pointer -= 4;
}

void hdma_transfer_type4_absolute(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr + 2, hdma_read_absolute(c));
  mmio_write(dma_channel[c].dest_addr + 3, hdma_read_absolute(c));
}

void hdma_transfer_type4_indirect(byte c) {
  mmio_write(dma_channel[c].dest_addr,     hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr + 1, hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr + 2, hdma_read_indirect(c));
  mmio_write(dma_channel[c].dest_addr + 3, hdma_read_indirect(c));
  if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL)dma_channel[c].hdma_indirect_pointer -= 4;
}

byte hdma_transfer_type_size_table[8] = { 1, 2, 2, 4, 4, 0, 0, 0 };

void HDMAFirstWrite(byte c) {
  if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL     && ppu.hdma_scanlines_remaining[c] == 0x00) {
    return;
  }
  if(dma_channel[c].hdma_mode == HDMAMODE_CONTINUOUS && ppu.hdma_scanlines_remaining[c] == 0x80) {
    return;
  }

  if(dma_channel[c].index_mode == DMAINDEX_ABSOLUTE) {
    if     (dma_channel[c].transfer_type == 0)hdma_transfer_type0_absolute(c);
    else if(dma_channel[c].transfer_type == 1)hdma_transfer_type1_absolute(c);
    else if(dma_channel[c].transfer_type == 2)hdma_transfer_type2_absolute(c);
    else if(dma_channel[c].transfer_type == 3)hdma_transfer_type3_absolute(c);
    else if(dma_channel[c].transfer_type == 4)hdma_transfer_type4_absolute(c);
    else dprintf("* mmio_w420c(): Unknown HDMA transfer type: %d", dma_channel[c].transfer_type);
  } else { //indirect
    if     (dma_channel[c].transfer_type == 0)hdma_transfer_type0_indirect(c);
    else if(dma_channel[c].transfer_type == 1)hdma_transfer_type1_indirect(c);
    else if(dma_channel[c].transfer_type == 2)hdma_transfer_type2_indirect(c);
    else if(dma_channel[c].transfer_type == 3)hdma_transfer_type3_indirect(c);
    else if(dma_channel[c].transfer_type == 4)hdma_transfer_type4_indirect(c);
    else dprintf("* mmio_w420c(): Unknown HDMA transfer type: %d", dma_channel[c].transfer_type);
  }
}

void UpdateHDMAAbsoluteNormal(byte c) {
  if(ppu.hdma_scanlines_remaining[c] == 0x00) {
    ppu.hdma_scanlines_remaining[c] = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr + ppu.hdma_index_pointer[c]);

    if(ppu.hdma_scanlines_remaining[c] == 0) {
      ppu.hdma_completed[c] = true;
      return;
    }

    dma_channel[c].hdma_indirect_pointer = 0;
    if(ppu.hdma_scanlines_remaining[c] > 0x80) {
      dma_channel[c].hdma_mode = HDMAMODE_CONTINUOUS;
    } else {
      dma_channel[c].hdma_mode = HDMAMODE_NORMAL;
    }

    ppu.hdma_index_pointer[c]++;
    HDMAFirstWrite(c);
  }

  ppu.hdma_scanlines_remaining[c]--;
}

void UpdateHDMAAbsoluteContinuous(byte c) {
  if(ppu.hdma_scanlines_remaining[c] == 0x80) {
    ppu.hdma_scanlines_remaining[c] = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr + ppu.hdma_index_pointer[c]);

    if(ppu.hdma_scanlines_remaining[c] == 0) {
      ppu.hdma_completed[c] = true;
      return;
    }

    dma_channel[c].hdma_indirect_pointer = 0;
    if(ppu.hdma_scanlines_remaining[c] > 0x80) {
      dma_channel[c].hdma_mode = HDMAMODE_CONTINUOUS;
    } else {
      dma_channel[c].hdma_mode = HDMAMODE_NORMAL;
    }

    ppu.hdma_index_pointer[c]++;
    HDMAFirstWrite(c);
    ppu.hdma_scanlines_remaining[c]--;
  } else {
    if     (dma_channel[c].transfer_type == 0)hdma_transfer_type0_absolute(c);
    else if(dma_channel[c].transfer_type == 1)hdma_transfer_type1_absolute(c);
    else if(dma_channel[c].transfer_type == 2)hdma_transfer_type2_absolute(c);
    else if(dma_channel[c].transfer_type == 3)hdma_transfer_type3_absolute(c);
    else if(dma_channel[c].transfer_type == 4)hdma_transfer_type4_absolute(c);
    else dprintf("* mmio_w420c(): Unknown HDMA transfer type: %d", dma_channel[c].transfer_type);

    ppu.hdma_scanlines_remaining[c]--;
  }
}

void UpdateHDMAIndirectNormal(byte c) {
  if(ppu.hdma_scanlines_remaining[c] == 0x00) {
    ppu.hdma_index_pointer[c] += 2;
    ppu.hdma_scanlines_remaining[c] = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr + ppu.hdma_index_pointer[c]);

    if(ppu.hdma_scanlines_remaining[c] == 0) {
      ppu.hdma_completed[c] = true;
      return;
    }

    dma_channel[c].hdma_indirect_pointer = 0;
    if(ppu.hdma_scanlines_remaining[c] > 0x80) {
      dma_channel[c].hdma_mode = HDMAMODE_CONTINUOUS;
    } else {
      dma_channel[c].hdma_mode = HDMAMODE_NORMAL;
    }

    ppu.hdma_index_pointer[c]++;
    HDMAFirstWrite(c);
  }

  ppu.hdma_scanlines_remaining[c]--;
}

void UpdateHDMAIndirectContinuous(byte c) {
  if(ppu.hdma_scanlines_remaining[c] == 0x80) {
    ppu.hdma_index_pointer[c] += 2;
    ppu.hdma_scanlines_remaining[c] = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr + ppu.hdma_index_pointer[c]);

    if(ppu.hdma_scanlines_remaining[c] == 0) {
      ppu.hdma_completed[c] = true;
      return;
    }

    dma_channel[c].hdma_indirect_pointer = 0;
    if(ppu.hdma_scanlines_remaining[c] > 0x80) {
      dma_channel[c].hdma_mode = HDMAMODE_CONTINUOUS;
    } else {
      dma_channel[c].hdma_mode = HDMAMODE_NORMAL;
    }
    ppu.hdma_index_pointer[c]++;
    HDMAFirstWrite(c);
    ppu.hdma_scanlines_remaining[c]--;
  } else {
    if     (dma_channel[c].transfer_type == 0)hdma_transfer_type0_indirect(c);
    else if(dma_channel[c].transfer_type == 1)hdma_transfer_type1_indirect(c);
    else if(dma_channel[c].transfer_type == 2)hdma_transfer_type2_indirect(c);
    else if(dma_channel[c].transfer_type == 3)hdma_transfer_type3_indirect(c);
    else if(dma_channel[c].transfer_type == 4)hdma_transfer_type4_indirect(c);
    else dprintf("* mmio_w420c(): Unknown HDMA transfer type: %d", dma_channel[c].transfer_type);

    ppu.hdma_scanlines_remaining[c]--;
  }
}

void UpdateHDMA(void) {
int c;
  for(c=0;c<8;c++) {
    if(ppu.active_hdma_channels & (1 << c)) {
      if(ppu.vline_pos == 0)continue;
      if(ppu.vline_pos == 1) {
        ppu.hdma_completed[c]     = false;
        ppu.hdma_index_pointer[c] = 0;
        ppu.hdma_scanlines_remaining[c] = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, dma_channel[c].src_addr);
        ppu.hdma_index_pointer[c]++;

        if(ppu.hdma_scanlines_remaining[c] == 0) {
          ppu.hdma_completed[c] = true;
          continue;
        }

        dma_channel[c].hdma_indirect_pointer = 0;
        if(ppu.hdma_scanlines_remaining[c] > 0x80) {
          dma_channel[c].hdma_mode = HDMAMODE_CONTINUOUS;
        } else {
          dma_channel[c].hdma_mode = HDMAMODE_NORMAL;
        }

        HDMAFirstWrite(c);
        ppu.hdma_scanlines_remaining[c]--;
      } else {
        if(ppu.hdma_completed[c] == true)continue;

        if(dma_channel[c].index_mode == DMAINDEX_ABSOLUTE) {
          if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL) {
            UpdateHDMAAbsoluteNormal(c);
          } else {
            UpdateHDMAAbsoluteContinuous(c);
          }
        } else { //indirect
          if(dma_channel[c].hdma_mode == HDMAMODE_NORMAL) {
            UpdateHDMAIndirectNormal(c);
          } else {
            UpdateHDMAIndirectContinuous(c);
          }
        }
      }
    }
  }
}
