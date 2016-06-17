//====================
//direct data transfer
//====================

auto SA1::dmaNormal() -> void {
  while(mmio.dtc--) {
    uint8 data = r.mdr;
    uint32 dsa = mmio.dsa++;
    uint32 dda = mmio.dda++;

    //source and destination cannot be the same
    if(mmio.sd == DMA::SourceBWRAM && mmio.dd == DMA::DestBWRAM) continue;
    if(mmio.sd == DMA::SourceIRAM  && mmio.dd == DMA::DestIRAM ) continue;

    switch(mmio.sd) {
    case DMA::SourceROM:
      if((dsa & 0x408000) == 0x008000 || (dsa & 0xc00000) == 0xc00000) {
        data = busRead(dsa, data);
      }
      break;

    case DMA::SourceBWRAM:
      if((dsa & 0x40e000) == 0x006000 || (dsa & 0xf00000) == 0x400000) {
        data = busRead(dsa, data);
      }
      break;

    case DMA::SourceIRAM:
      data = iram.read(dsa & 0x07ff);
      break;
    }

    switch(mmio.dd) {
    case DMA::DestBWRAM:
      if((dda & 0x40e000) == 0x006000 || (dda & 0xf00000) == 0x400000) {
        busWrite(dda, data);
      }
      break;

    case DMA::DestIRAM:
      iram.write(dda & 0x07ff, data);
      break;
    }
  }

  mmio.dma_irqfl = true;
  if(mmio.dma_irqen) mmio.dma_irqcl = 0;
}

//((byte & 6) << 3) + (byte & 1) explanation:
//transforms a byte index (0-7) into a planar index:
//result[] = {  0,  1, 16, 17, 32, 33, 48, 49 };
//works for 2bpp, 4bpp and 8bpp modes

//===========================
//type-1 character conversion
//===========================

auto SA1::dmaCC1() -> void {
  cpubwram.dma = true;
  mmio.chdma_irqfl = true;
  if(mmio.chdma_irqen) {
    mmio.chdma_irqcl = 0;
    cpu.r.irq = 1;
  }
}

auto SA1::dmaCC1Read(uint addr) -> uint8 {
  //16 bytes/char (2bpp); 32 bytes/char (4bpp); 64 bytes/char (8bpp)
  uint charmask = (1 << (6 - mmio.dmacb)) - 1;

  if((addr & charmask) == 0) {
    //buffer next character to I-RAM
    uint bpp = 2 << (2 - mmio.dmacb);
    uint bpl = (8 << mmio.dmasize) >> mmio.dmacb;
    uint bwmask = bwram.size() - 1;
    uint tile = ((addr - mmio.dsa) & bwmask) >> (6 - mmio.dmacb);
    uint ty = (tile >> mmio.dmasize);
    uint tx = tile & ((1 << mmio.dmasize) - 1);
    uint bwaddr = mmio.dsa + ty * 8 * bpl + tx * bpp;

    for(auto y : range(8)) {
      uint64 data = 0;
      for(auto byte : range(bpp)) {
        data |= (uint64)bwram.read((bwaddr + byte) & bwmask) << (byte << 3);
      }
      bwaddr += bpl;

      uint8 out[] = {0, 0, 0, 0, 0, 0, 0, 0};
      for(auto x : range(8)) {
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

      for(auto byte : range(bpp)) {
        uint p = mmio.dda + (y << 1) + ((byte & 6) << 3) + (byte & 1);
        iram.write(p & 0x07ff, out[byte]);
      }
    }
  }

  return iram.read((mmio.dda + (addr & charmask)) & 0x07ff);
}

//===========================
//type-2 character conversion
//===========================

auto SA1::dmaCC2() -> void {
  //select register file index (0-7 or 8-15)
  const uint8* brf = &mmio.brf[(dma.line & 1) << 3];
  uint bpp = 2 << (2 - mmio.dmacb);
  uint addr = mmio.dda & 0x07ff;
  addr &= ~((1 << (7 - mmio.dmacb)) - 1);
  addr += (dma.line & 8) * bpp;
  addr += (dma.line & 7) * 2;

  for(auto byte : range(bpp)) {
    uint8 output = 0;
    for(auto bit : range(8)) {
      output |= ((brf[bit] >> byte) & 1) << (7 - bit);
    }
    iram.write(addr + ((byte & 6) << 3) + (byte & 1), output);
  }

  dma.line = (dma.line + 1) & 15;
}
