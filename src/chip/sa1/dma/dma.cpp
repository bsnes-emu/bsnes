#ifdef SA1_CPP

//====================
//direct data transfer
//====================

void SA1::dma_normal() {
  while(mmio.dtc--) {
    uint8_t data = regs.mdr;
    uint32_t dsa = mmio.dsa++;
    uint32_t dda = mmio.dda++;
    add_clocks(4);
    scheduler.sync_copcpu();

    if(mmio.sd == DMA::SourceBWRAM && mmio.dd == DMA::DestBWRAM) continue;
    if(mmio.sd == DMA::SourceIRAM  && mmio.dd == DMA::DestIRAM ) continue;

    switch(mmio.sd) {
      case DMA::SourceROM: {
        if((dsa & 0x408000) == 0x008000 || (dsa & 0xc00000) == 0xc00000) {
          data = sa1bus.read(dsa);
        }
      } break;

      case DMA::SourceBWRAM: {
        if((dsa & 0x40e000) == 0x006000 || (dsa & 0xf00000) == 0x400000) {
          data = sa1bus.read(dsa);
        }
      } break;

      case DMA::SourceIRAM: {
        data = iram[dsa & 0x07ff];
      } break;
    }

    switch(mmio.dd) {
      case DMA::DestBWRAM: {
        if((dda & 0x40e000) == 0x006000 || (dda & 0xf00000) == 0x400000) {
          sa1bus.write(dda, data);
        }
      } break;

      case DMA::DestIRAM: {
        iram[dda & 0x07ff] = data;
      } break;
    }
  }

  dma.mode = DMA::Inactive;
  mmio.dma_irqfl = true;
  if(mmio.dma_irqen) mmio.dma_irqcl = 0;
}

//===========================
//type-1 character conversion
//===========================

void SA1::dma_cc1() {
  memory::cpu::bwram.cc1dma = true;

  dma.tile = 0;
  dma.mode = DMA::Inactive;
  mmio.chdma_irqfl = true;
  if(mmio.chdma_irqen) {
    mmio.chdma_irqcl = 0;
    cpu.regs.irq = 1;
  }
}

uint8_t SA1::dma_cc1_read(unsigned addr) {
  //16 bytes/char (2bpp); 32 bytes/char (4bpp); 64 bytes/char (8bpp)
  unsigned charmask = (1 << (6 - mmio.dmacb)) - 1;

  if((addr & charmask) == 0) {
    //buffer next character to I-RAM
    unsigned bpp = 2 << (2 - mmio.dmacb);
    unsigned bpl = (8 << mmio.dmasize) >> mmio.dmacb;
    unsigned bwmask = memory::sa1::bwram.size() - 1;
    unsigned tile = ((addr - mmio.dsa) & bwmask) >> (6 - mmio.dmacb);
    unsigned ty = (tile >> mmio.dmasize);
    unsigned tx = tile & ((1 << mmio.dmasize) - 1);
    unsigned bwaddr = mmio.dsa + ty * 8 * bpl + tx * bpp;

    for(unsigned y = 0; y < 8; y++) {
      uint64_t data = 0;
      for(unsigned n = 0; n < bpp; n++) {
        data |= (uint64_t)memory::sa1::bwram.read((bwaddr + n) & bwmask) << (n << 3);
      }
      bwaddr += bpl;

      uint8_t out[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      for(unsigned x = 0; x < 8; x++) {
        out[0] |= (data & 1) << (7 - x); data >>= 1;
        out[1] |= (data & 1) << (7 - x); data >>= 1;
        if(mmio.dmacb == 2) continue;
        out[2] |= (data & 1) << (7 - x); data >>= 1;
        out[3] |= (data & 1) << (7 - x); data >>= 1;
        if(mmio.dmacb == 1) continue;
        out[4] |= (data & 1) << (7 - x); data >>= 1;
        out[5] |= (data & 1) << (7 - x); data >>= 1;
        out[6] |= (data & 1) << (7 - x); data >>= 1;
        out[7] |= (data & 1) << (7 - x); data >>= 1;
      }

      for(unsigned n = 0; n < bpp; n++) {
        static const unsigned index[] = { 0, 1, 16, 17, 32, 33, 48, 49 };
        unsigned p = mmio.dda + (y << 1) + index[n];
        iram[p & 0x07ff] = out[n];
      }
    }
  }

  return iram[(mmio.dda + (addr & charmask)) & 0x07ff];
}

//===========================
//type-2 character conversion
//===========================

void SA1::dma_cc2() {
  //select register file index (0-7 or 8-F)
  const uint8_t *brf = &mmio.brf[(dma.line & 1) << 3];
  unsigned bpp = 2 << (2 - mmio.dmacb);
  unsigned addr = mmio.dda & 0x07ff;
  addr &= ~((1 << (7 - mmio.dmacb)) - 1);
  addr += (dma.line & 8) * bpp;
  addr += (dma.line & 7) * 2;

  for(unsigned byte = 0; byte < bpp; byte++) {
    uint8_t output = 0;
    for(unsigned bit = 0; bit < 8; bit++) {
      output |= ((brf[bit] >> byte) & 1) << (7 - bit);
    }

    static const unsigned index[] = { 0, 1, 16, 17, 32, 33, 48, 49 };
    iram[addr + index[byte]] = output;
    add_clocks(4);
    scheduler.sync_copcpu();
  }

  dma.mode = DMA::Inactive;
  dma.line = (dma.line + 1) & 15;
}

#endif
