//(CCNT) SA-1 control
auto SA1::mmio_w2200(uint8 data) -> void {
  if(mmio.sa1_resb && !(data & 0x80)) {
    //reset SA-1 CPU
    r.pc.w = mmio.crv;
    r.pc.b = 0x00;
  }

  mmio.sa1_irq  = (data & 0x80);
  mmio.sa1_rdyb = (data & 0x40);
  mmio.sa1_resb = (data & 0x20);
  mmio.sa1_nmi  = (data & 0x10);
  mmio.smeg     = (data & 0x0f);

  if(mmio.sa1_irq) {
    mmio.sa1_irqfl = true;
    if(mmio.sa1_irqen) mmio.sa1_irqcl = 0;
  }

  if(mmio.sa1_nmi) {
    mmio.sa1_nmifl = true;
    if(mmio.sa1_nmien) mmio.sa1_nmicl = 0;
  }
}

//(SIE) S-CPU interrupt enable
auto SA1::mmio_w2201(uint8 data) -> void {
  if(!mmio.cpu_irqen && (data & 0x80)) {
    if(mmio.cpu_irqfl) {
      mmio.cpu_irqcl = 0;
      cpu.r.irq = 1;
    }
  }

  if(!mmio.chdma_irqen && (data & 0x20)) {
    if(mmio.chdma_irqfl) {
      mmio.chdma_irqcl = 0;
      cpu.r.irq = 1;
    }
  }

  mmio.cpu_irqen   = (data & 0x80);
  mmio.chdma_irqen = (data & 0x20);
}

//(SIC) S-CPU interrupt clear
auto SA1::mmio_w2202(uint8 data) -> void {
  mmio.cpu_irqcl   = (data & 0x80);
  mmio.chdma_irqcl = (data & 0x20);

  if(mmio.cpu_irqcl  ) mmio.cpu_irqfl   = false;
  if(mmio.chdma_irqcl) mmio.chdma_irqfl = false;

  if(!mmio.cpu_irqfl && !mmio.chdma_irqfl) cpu.r.irq = 0;
}

//(CRV) SA-1 reset vector
auto SA1::mmio_w2203(uint8 data) -> void { mmio.crv = (mmio.crv & 0xff00) | data; }
auto SA1::mmio_w2204(uint8 data) -> void { mmio.crv = (data << 8) | (mmio.crv & 0xff); }

//(CNV) SA-1 NMI vector
auto SA1::mmio_w2205(uint8 data) -> void { mmio.cnv = (mmio.cnv & 0xff00) | data; }
auto SA1::mmio_w2206(uint8 data) -> void { mmio.cnv = (data << 8) | (mmio.cnv & 0xff); }

//(CIV) SA-1 IRQ vector
auto SA1::mmio_w2207(uint8 data) -> void { mmio.civ = (mmio.civ & 0xff00) | data; }
auto SA1::mmio_w2208(uint8 data) -> void { mmio.civ = (data << 8) | (mmio.civ & 0xff); }

//(SCNT) S-CPU control
auto SA1::mmio_w2209(uint8 data) -> void {
  mmio.cpu_irq  = (data & 0x80);
  mmio.cpu_ivsw = (data & 0x40);
  mmio.cpu_nvsw = (data & 0x10);
  mmio.cmeg     = (data & 0x0f);

  if(mmio.cpu_irq) {
    mmio.cpu_irqfl = true;
    if(mmio.cpu_irqen) {
      mmio.cpu_irqcl = 0;
      cpu.r.irq = 1;
    }
  }
}

//(CIE) SA-1 interrupt enable
auto SA1::mmio_w220a(uint8 data) -> void {
  if(!mmio.sa1_irqen   && (data & 0x80) && mmio.sa1_irqfl  ) mmio.sa1_irqcl   = 0;
  if(!mmio.timer_irqen && (data & 0x40) && mmio.timer_irqfl) mmio.timer_irqcl = 0;
  if(!mmio.dma_irqen   && (data & 0x20) && mmio.dma_irqfl  ) mmio.dma_irqcl   = 0;
  if(!mmio.sa1_nmien   && (data & 0x10) && mmio.sa1_nmifl  ) mmio.sa1_nmicl   = 0;

  mmio.sa1_irqen   = (data & 0x80);
  mmio.timer_irqen = (data & 0x40);
  mmio.dma_irqen   = (data & 0x20);
  mmio.sa1_nmien   = (data & 0x10);
}

//(CIC) SA-1 interrupt clear
auto SA1::mmio_w220b(uint8 data) -> void {
  mmio.sa1_irqcl   = (data & 0x80);
  mmio.timer_irqcl = (data & 0x40);
  mmio.dma_irqcl   = (data & 0x20);
  mmio.sa1_nmicl   = (data & 0x10);

  if(mmio.sa1_irqcl)   mmio.sa1_irqfl   = false;
  if(mmio.timer_irqcl) mmio.timer_irqfl = false;
  if(mmio.dma_irqcl)   mmio.dma_irqfl   = false;
  if(mmio.sa1_nmicl)   mmio.sa1_nmifl   = false;
}

//(SNV) S-CPU NMI vector
auto SA1::mmio_w220c(uint8 data) -> void { mmio.snv = (mmio.snv & 0xff00) | data;      }
auto SA1::mmio_w220d(uint8 data) -> void { mmio.snv = (data << 8) | (mmio.snv & 0xff); }

//(SIV) S-CPU IRQ vector
auto SA1::mmio_w220e(uint8 data) -> void { mmio.siv = (mmio.siv & 0xff00) | data;      }
auto SA1::mmio_w220f(uint8 data) -> void { mmio.siv = (data << 8) | (mmio.siv & 0xff); }

//(TMC) H/V timer control
auto SA1::mmio_w2210(uint8 data) -> void {
  mmio.hvselb = (data & 0x80);
  mmio.ven    = (data & 0x02);
  mmio.hen    = (data & 0x01);
}

//(CTR) SA-1 timer restart
auto SA1::mmio_w2211(uint8 data) -> void {
  status.vcounter = 0;
  status.hcounter = 0;
}

//(HCNT) H-count
auto SA1::mmio_w2212(uint8 data) -> void { mmio.hcnt = (mmio.hcnt & 0xff00) | (data << 0); }
auto SA1::mmio_w2213(uint8 data) -> void { mmio.hcnt = (mmio.hcnt & 0x00ff) | (data << 8); }

//(VCNT) V-count
auto SA1::mmio_w2214(uint8 data) -> void { mmio.vcnt = (mmio.vcnt & 0xff00) | (data << 0); }
auto SA1::mmio_w2215(uint8 data) -> void { mmio.vcnt = (mmio.vcnt & 0x00ff) | (data << 8); }

//(CXB) Super MMC bank C
auto SA1::mmio_w2220(uint8 data) -> void {
  mmio.cbmode = (data & 0x80);
  mmio.cb     = (data & 0x07);
}

//(DXB) Super MMC bank D
auto SA1::mmio_w2221(uint8 data) -> void {
  mmio.dbmode = (data & 0x80);
  mmio.db     = (data & 0x07);
}

//(EXB) Super MMC bank E
auto SA1::mmio_w2222(uint8 data) -> void {
  mmio.ebmode = (data & 0x80);
  mmio.eb     = (data & 0x07);
}

//(FXB) Super MMC bank F
auto SA1::mmio_w2223(uint8 data) -> void {
  mmio.fbmode = (data & 0x80);
  mmio.fb     = (data & 0x07);
}

//(BMAPS) S-CPU BW-RAM address mapping
auto SA1::mmio_w2224(uint8 data) -> void {
  mmio.sbm = (data & 0x1f);
}

//(BMAP) SA-1 BW-RAM address mapping
auto SA1::mmio_w2225(uint8 data) -> void {
  mmio.sw46 = (data & 0x80);
  mmio.cbm  = (data & 0x7f);
}

//(SWBE) S-CPU BW-RAM write enable
auto SA1::mmio_w2226(uint8 data) -> void {
  mmio.swen = (data & 0x80);
}

//(CWBE) SA-1 BW-RAM write enable
auto SA1::mmio_w2227(uint8 data) -> void {
  mmio.cwen = (data & 0x80);
}

//(BWPA) BW-RAM write-protected area
auto SA1::mmio_w2228(uint8 data) -> void {
  mmio.bwp = (data & 0x0f);
}

//(SIWP) S-CPU I-RAM write protection
auto SA1::mmio_w2229(uint8 data) -> void {
  mmio.siwp = data;
}

//(CIWP) SA-1 I-RAM write protection
auto SA1::mmio_w222a(uint8 data) -> void {
  mmio.ciwp = data;
}

//(DCNT) DMA control
auto SA1::mmio_w2230(uint8 data) -> void {
  mmio.dmaen = (data & 0x80);
  mmio.dprio = (data & 0x40);
  mmio.cden  = (data & 0x20);
  mmio.cdsel = (data & 0x10);
  mmio.dd    = (data & 0x04);
  mmio.sd    = (data & 0x03);

  if(mmio.dmaen == 0) dma.line = 0;
}

//(CDMA) character conversion DMA parameters
auto SA1::mmio_w2231(uint8 data) -> void {
  mmio.chdend  = (data & 0x80);
  mmio.dmasize = (data >> 2) & 7;
  mmio.dmacb   = (data & 0x03);

  if(mmio.chdend) cpubwram.dma = false;
  if(mmio.dmasize > 5) mmio.dmasize = 5;
  if(mmio.dmacb   > 2) mmio.dmacb   = 2;
}

//(SDA) DMA source device start address
auto SA1::mmio_w2232(uint8 data) -> void { mmio.dsa = (mmio.dsa & 0xffff00) | (data <<  0); }
auto SA1::mmio_w2233(uint8 data) -> void { mmio.dsa = (mmio.dsa & 0xff00ff) | (data <<  8); }
auto SA1::mmio_w2234(uint8 data) -> void { mmio.dsa = (mmio.dsa & 0x00ffff) | (data << 16); }

//(DDA) DMA destination start address
auto SA1::mmio_w2235(uint8 data) -> void {
  mmio.dda = (mmio.dda & 0xffff00) | (data <<  0);
}

auto SA1::mmio_w2236(uint8 data) -> void {
  mmio.dda = (mmio.dda & 0xff00ff) | (data <<  8);

  if(mmio.dmaen == true) {
    if(mmio.cden == 0 && mmio.dd == DMA::DestIRAM) {
      dma_normal();
    } else if(mmio.cden == 1 && mmio.cdsel == 1) {
      dma_cc1();
    }
  }
}

auto SA1::mmio_w2237(uint8 data) -> void {
  mmio.dda = (mmio.dda & 0x00ffff) | (data << 16);

  if(mmio.dmaen == true) {
    if(mmio.cden == 0 && mmio.dd == DMA::DestBWRAM) {
      dma_normal();
    }
  }
}

//(DTC) DMA terminal counter
auto SA1::mmio_w2238(uint8 data) -> void { mmio.dtc = (mmio.dtc & 0xff00) | (data << 0); }
auto SA1::mmio_w2239(uint8 data) -> void { mmio.dtc = (mmio.dtc & 0x00ff) | (data << 8); }

//(BBF) BW-RAM bitmap format
auto SA1::mmio_w223f(uint8 data) -> void {
  mmio.bbf = (data & 0x80);
}

//(BRF) bitmap register files
auto SA1::mmio_w2240(uint8 data) -> void { mmio.brf[ 0] = data; }
auto SA1::mmio_w2241(uint8 data) -> void { mmio.brf[ 1] = data; }
auto SA1::mmio_w2242(uint8 data) -> void { mmio.brf[ 2] = data; }
auto SA1::mmio_w2243(uint8 data) -> void { mmio.brf[ 3] = data; }
auto SA1::mmio_w2244(uint8 data) -> void { mmio.brf[ 4] = data; }
auto SA1::mmio_w2245(uint8 data) -> void { mmio.brf[ 5] = data; }
auto SA1::mmio_w2246(uint8 data) -> void { mmio.brf[ 6] = data; }
auto SA1::mmio_w2247(uint8 data) -> void { mmio.brf[ 7] = data;
  if(mmio.dmaen == true) {
    if(mmio.cden == 1 && mmio.cdsel == 0) {
      dma_cc2();
    }
  }
}

auto SA1::mmio_w2248(uint8 data) -> void { mmio.brf[ 8] = data; }
auto SA1::mmio_w2249(uint8 data) -> void { mmio.brf[ 9] = data; }
auto SA1::mmio_w224a(uint8 data) -> void { mmio.brf[10] = data; }
auto SA1::mmio_w224b(uint8 data) -> void { mmio.brf[11] = data; }
auto SA1::mmio_w224c(uint8 data) -> void { mmio.brf[12] = data; }
auto SA1::mmio_w224d(uint8 data) -> void { mmio.brf[13] = data; }
auto SA1::mmio_w224e(uint8 data) -> void { mmio.brf[14] = data; }
auto SA1::mmio_w224f(uint8 data) -> void { mmio.brf[15] = data;
  if(mmio.dmaen == true) {
    if(mmio.cden == 1 && mmio.cdsel == 0) {
      dma_cc2();
    }
  }
}

//(MCNT) arithmetic control
auto SA1::mmio_w2250(uint8 data) -> void {
  mmio.acm = (data & 0x02);
  mmio.md  = (data & 0x01);

  if(mmio.acm) mmio.mr = 0;
}

//(MAL) multiplicand / dividend low
auto SA1::mmio_w2251(uint8 data) -> void {
  mmio.ma = (mmio.ma & 0xff00) | data;
}

//(MAH) multiplicand / dividend high
auto SA1::mmio_w2252(uint8 data) -> void {
  mmio.ma = (data << 8) | (mmio.ma & 0x00ff);
}

//(MBL) multiplier / divisor low
auto SA1::mmio_w2253(uint8 data) -> void {
  mmio.mb = (mmio.mb & 0xff00) | data;
}

//(MBH) multiplier / divisor high
//multiplication / cumulative sum only resets MB
//division resets both MA and MB
auto SA1::mmio_w2254(uint8 data) -> void {
  mmio.mb = (data << 8) | (mmio.mb & 0x00ff);

  if(mmio.acm == 0) {
    if(mmio.md == 0) {
      //signed multiplication
      mmio.mr = (int16)mmio.ma * (int16)mmio.mb;
      mmio.mb = 0;
    } else {
      //unsigned division
      if(mmio.mb == 0) {
        mmio.mr = 0;
      } else {
        int16  quotient  = (int16)mmio.ma / (uint16)mmio.mb;
        uint16 remainder = (int16)mmio.ma % (uint16)mmio.mb;
        mmio.mr = (remainder << 16) | quotient;
      }
      mmio.ma = 0;
      mmio.mb = 0;
    }
  } else {
    //sigma (accumulative multiplication)
    mmio.mr += (int16)mmio.ma * (int16)mmio.mb;
    mmio.overflow = (mmio.mr >= (1ULL << 40));
    mmio.mr &= (1ULL << 40) - 1;
    mmio.mb = 0;
  }
}

//(VBD) variable-length bit processing
auto SA1::mmio_w2258(uint8 data) -> void {
  mmio.hl = (data & 0x80);
  mmio.vb = (data & 0x0f);
  if(mmio.vb == 0) mmio.vb = 16;

  if(mmio.hl == 0) {
    //fixed mode
    mmio.vbit += mmio.vb;
    mmio.va += (mmio.vbit >> 3);
    mmio.vbit &= 7;
  }
}

//(VDA) variable-length bit game pak ROM start address
auto SA1::mmio_w2259(uint8 data) -> void { mmio.va = (mmio.va & 0xffff00) | (data <<  0); }
auto SA1::mmio_w225a(uint8 data) -> void { mmio.va = (mmio.va & 0xff00ff) | (data <<  8); }
auto SA1::mmio_w225b(uint8 data) -> void { mmio.va = (mmio.va & 0x00ffff) | (data << 16); mmio.vbit = 0; }

//(SFR) S-CPU flag read
auto SA1::mmio_r2300() -> uint8 {
  uint8 data;
  data  = mmio.cpu_irqfl   << 7;
  data |= mmio.cpu_ivsw    << 6;
  data |= mmio.chdma_irqfl << 5;
  data |= mmio.cpu_nvsw    << 4;
  data |= mmio.cmeg;
  return data;
}

//(CFR) SA-1 flag read
auto SA1::mmio_r2301() -> uint8 {
  uint8 data;
  data  = mmio.sa1_irqfl   << 7;
  data |= mmio.timer_irqfl << 6;
  data |= mmio.dma_irqfl   << 5;
  data |= mmio.sa1_nmifl   << 4;
  data |= mmio.smeg;
  return data;
}

//(HCR) hcounter read
auto SA1::mmio_r2302() -> uint8 {
  //latch counters
  mmio.hcr = status.hcounter >> 2;
  mmio.vcr = status.vcounter;
  return mmio.hcr >> 0;
}

auto SA1::mmio_r2303() -> uint8 {
  return mmio.hcr >> 8;
}

//(VCR) vcounter read
auto SA1::mmio_r2304() -> uint8 { return mmio.vcr >> 0; }
auto SA1::mmio_r2305() -> uint8 { return mmio.vcr >> 8; }

//(MR) arithmetic result
auto SA1::mmio_r2306() -> uint8 { return mmio.mr >>  0; }
auto SA1::mmio_r2307() -> uint8 { return mmio.mr >>  8; }
auto SA1::mmio_r2308() -> uint8 { return mmio.mr >> 16; }
auto SA1::mmio_r2309() -> uint8 { return mmio.mr >> 24; }
auto SA1::mmio_r230a() -> uint8 { return mmio.mr >> 32; }

//(OF) arithmetic overflow flag
auto SA1::mmio_r230b() -> uint8 { return mmio.overflow << 7; }

//(VDPL) variable-length data read port low
auto SA1::mmio_r230c() -> uint8 {
  uint32 data = (vbr_read(mmio.va + 0) <<  0)
              | (vbr_read(mmio.va + 1) <<  8)
              | (vbr_read(mmio.va + 2) << 16);
  data >>= mmio.vbit;
  return data >> 0;
}

//(VDPH) variable-length data read port high
auto SA1::mmio_r230d() -> uint8 {
  uint32 data = (vbr_read(mmio.va + 0) <<  0)
              | (vbr_read(mmio.va + 1) <<  8)
              | (vbr_read(mmio.va + 2) << 16);
  data >>= mmio.vbit;

  if(mmio.hl == 1) {
    //auto-increment mode
    mmio.vbit += mmio.vb;
    mmio.va += (mmio.vbit >> 3);
    mmio.vbit &= 7;
  }

  return data >> 8;
}

//(VC) version code register
auto SA1::mmio_r230e() -> uint8 {
  return 0x01;  //true value unknown
}

auto SA1::mmio_read(uint24 addr, uint8) -> uint8 {
  (co_active() == cpu.thread ? cpu.synchronizeCoprocessors() : synchronizeCPU());
  addr &= 0xffff;

  switch(addr) {
  case 0x2300: return mmio_r2300();
  case 0x2301: return mmio_r2301();
  case 0x2302: return mmio_r2302();
  case 0x2303: return mmio_r2303();
  case 0x2304: return mmio_r2304();
  case 0x2305: return mmio_r2305();
  case 0x2306: return mmio_r2306();
  case 0x2307: return mmio_r2307();
  case 0x2308: return mmio_r2308();
  case 0x2309: return mmio_r2309();
  case 0x230a: return mmio_r230a();
  case 0x230b: return mmio_r230b();
  case 0x230c: return mmio_r230c();
  case 0x230d: return mmio_r230d();
  case 0x230e: return mmio_r230e();
  }

  return 0x00;
}

auto SA1::mmio_write(uint24 addr, uint8 data) -> void {
  (co_active() == cpu.thread ? cpu.synchronizeCoprocessors() : synchronizeCPU());
  addr &= 0xffff;

  switch(addr) {
  case 0x2200: return mmio_w2200(data);
  case 0x2201: return mmio_w2201(data);
  case 0x2202: return mmio_w2202(data);
  case 0x2203: return mmio_w2203(data);
  case 0x2204: return mmio_w2204(data);
  case 0x2205: return mmio_w2205(data);
  case 0x2206: return mmio_w2206(data);
  case 0x2207: return mmio_w2207(data);
  case 0x2208: return mmio_w2208(data);
  case 0x2209: return mmio_w2209(data);
  case 0x220a: return mmio_w220a(data);
  case 0x220b: return mmio_w220b(data);
  case 0x220c: return mmio_w220c(data);
  case 0x220d: return mmio_w220d(data);
  case 0x220e: return mmio_w220e(data);
  case 0x220f: return mmio_w220f(data);

  case 0x2210: return mmio_w2210(data);
  case 0x2211: return mmio_w2211(data);
  case 0x2212: return mmio_w2212(data);
  case 0x2213: return mmio_w2213(data);
  case 0x2214: return mmio_w2214(data);
  case 0x2215: return mmio_w2215(data);

  case 0x2220: return mmio_w2220(data);
  case 0x2221: return mmio_w2221(data);
  case 0x2222: return mmio_w2222(data);
  case 0x2223: return mmio_w2223(data);
  case 0x2224: return mmio_w2224(data);
  case 0x2225: return mmio_w2225(data);
  case 0x2226: return mmio_w2226(data);
  case 0x2227: return mmio_w2227(data);
  case 0x2228: return mmio_w2228(data);
  case 0x2229: return mmio_w2229(data);
  case 0x222a: return mmio_w222a(data);

  case 0x2230: return mmio_w2230(data);
  case 0x2231: return mmio_w2231(data);
  case 0x2232: return mmio_w2232(data);
  case 0x2233: return mmio_w2233(data);
  case 0x2234: return mmio_w2234(data);
  case 0x2235: return mmio_w2235(data);
  case 0x2236: return mmio_w2236(data);
  case 0x2237: return mmio_w2237(data);
  case 0x2238: return mmio_w2238(data);
  case 0x2239: return mmio_w2239(data);

  case 0x223f: return mmio_w223f(data);
  case 0x2240: return mmio_w2240(data);
  case 0x2241: return mmio_w2241(data);
  case 0x2242: return mmio_w2242(data);
  case 0x2243: return mmio_w2243(data);
  case 0x2244: return mmio_w2244(data);
  case 0x2245: return mmio_w2245(data);
  case 0x2246: return mmio_w2246(data);
  case 0x2247: return mmio_w2247(data);
  case 0x2248: return mmio_w2248(data);
  case 0x2249: return mmio_w2249(data);
  case 0x224a: return mmio_w224a(data);
  case 0x224b: return mmio_w224b(data);
  case 0x224c: return mmio_w224c(data);
  case 0x224d: return mmio_w224d(data);
  case 0x224e: return mmio_w224e(data);
  case 0x224f: return mmio_w224f(data);

  case 0x2250: return mmio_w2250(data);
  case 0x2251: return mmio_w2251(data);
  case 0x2252: return mmio_w2252(data);
  case 0x2253: return mmio_w2253(data);
  case 0x2254: return mmio_w2254(data);

  case 0x2258: return mmio_w2258(data);
  case 0x2259: return mmio_w2259(data);
  case 0x225a: return mmio_w225a(data);
  case 0x225b: return mmio_w225b(data);
  }
}
