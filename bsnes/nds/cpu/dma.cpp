
bool CPUCore::dmaTrigger(unsigned channels, unsigned value) {
  for(unsigned no = 0; no < 4; no++) {
    if((channels & 1<<no) && dma[no].enable && dma[no].trigger == value)
      dmaTransfer(no);
    else if(no+1 == 4)
      return false;
  }
  return true;
}

void CPUCore::dmaTransfer(unsigned no) {
  auto &dma = this->dma[no];
  
  uint32 size = dma.size? Word : Half;
  uint32 srcmask = config.arm7 && no == 0? 0x07fffffe : 0x0ffffffe;
  uint32 destmask = config.arm7 && no < 3? 0x07fffffe : 0x0ffffffe;
  uint32 countmask = 0x1fffff;
  
  if(config.arm7)
    countmask = no < 3? 0x3fff : 0xffff;
  
  int srcinc  = (dma.srcStep  == 1? -2 : dma.srcStep  != 2? +2 : 0);
  int destinc = (dma.destStep == 1? -2 : dma.destStep != 2? +2 : 0);
  
  if(size == Word)
    srcinc <<= 1, destinc <<= 1;
  
  do {
    uint32 data = read(dma.source, size, true);
    write(dma.dest, size, true, data);
    
    dma.source = (dma.source + srcinc) & srcmask;
    dma.dest   = (dma.dest + destinc) & destmask;
  } while(--dma.count & countmask);
  
  if(dma.destStep == 3) dma.dest   = dma.init.dest;
  if(dma.repeat == 1)   dma.count  = dma.init.count;
  if(dma.repeat == 0)   dma.enable = false;
  
  if(dma.irq) interrupt.flags |= irqDma << no;
}


uint32 CPUCore::regDmaControl(unsigned no) {
  return dma[no].init.count<<0 | dma[no].destStep<<21 | dma[no].srcStep<<23
       | dma[no].repeat<<25 | dma[no].size<<26 | dma[no].trigger<<27
       | dma[no].irq<<30 | dma[no].enable<<31;
}

uint32 CPUCore::regDmaSource(unsigned no) {
  return dma[no].init.source;
}

uint32 CPUCore::regDmaDest(unsigned no) {
  return dma[no].init.dest;
}

uint32 CPUCore::regDmaFill(unsigned no) {
  return dma[no].fill;
}

void CPUCore::regDmaControl(unsigned no, uint32 data, uint32 mask) {
  auto &dma = this->dma[no];
  
  uint32 countmask = 0x1fffff;
  if(config.arm7) // still limited to 16K / 64K transfers
    countmask = no < 3? 0x3fff : 0xffff;
  
  if(mask & countmask) {
    dma.init.count ^= (dma.init.count ^ data) & countmask;
  }
  if(mask & 0xffe00000) { // ??
    bool previously = dma.enable;
    
    dma.destStep = data>>21;
    dma.srcStep  = data>>23;
    dma.repeat   = data>>25;
    dma.size     = data>>26;
    dma.trigger  = data>>27;
    dma.irq      = data>>30;
    dma.enable   = data>>31;
    
    if(config.arm7)
      dma.trigger >>= 1;
    
    if(dma.enable) {
      if(previously == false) {
        // Latch new settings
        dma.source = dma.init.source;
        dma.dest   = dma.init.dest;
        dma.count  = dma.init.count;
      }
      if(dma.trigger == 0) {
        dmaTransfer(no);
      }
      else if(dma.trigger == 7) {
        // Geometry fifo - DMA display list
        // - just do it right here since we lack timing
        dmaTransfer(no);
      }
      //else {
      //  print(config.arm7?"arm7":"arm9",
      //    ": dma trigger ",dma.trigger,": unimplemented\n");
      //}
    }
  }
}

void CPUCore::regDmaSource(unsigned no, uint32 data, uint32 mask) {
  mask &= 0x0ffffffe;
  if(config.arm7 && no == 0)
    mask &= 0x07fffffe; // DMA0 is internal only
  
  dma[no].init.source ^= (dma[no].init.source ^ data) & mask;
}

void CPUCore::regDmaDest(unsigned no, uint32 data, uint32 mask) {
  mask &= 0x0ffffffe;
  if(config.arm7 && no != 3)
    mask &= 0x07fffffe; // only DMA3 can write to slot 2
  
  dma[no].init.dest ^= (dma[no].init.dest ^ data) & mask;
}

void CPUCore::regDmaFill(unsigned no, uint32 data, uint32 mask) {
  if(config.arm7)
    return; // not present
  
  dma[no].fill ^= (dma[no].fill ^ data) & mask;
}
