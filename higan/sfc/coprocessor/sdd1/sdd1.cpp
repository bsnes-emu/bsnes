#include <sfc/sfc.hpp>

namespace SuperFamicom {

SDD1 sdd1;

#include "decomp.cpp"
#include "serialization.cpp"

auto SDD1::init() -> void {
}

void SDD1::load() {
  //hook S-CPU DMA MMIO registers to gather information for struct dma[];
  //buffer address and transfer size information for use in SDD1::mcu_read()
  bus.map({&SDD1::dma_read, &sdd1}, {&SDD1::dma_write, &sdd1}, 0x00, 0x3f, 0x4300, 0x437f);
  bus.map({&SDD1::dma_read, &sdd1}, {&SDD1::dma_write, &sdd1}, 0x80, 0xbf, 0x4300, 0x437f);
}

auto SDD1::unload() -> void {
  rom.reset();
  ram.reset();
}

auto SDD1::power() -> void {
}

auto SDD1::reset() -> void {
  sdd1_enable = 0x00;
  xfer_enable = 0x00;
  dma_ready = false;

  mmc[0] = 0 << 20;
  mmc[1] = 1 << 20;
  mmc[2] = 2 << 20;
  mmc[3] = 3 << 20;

  for(auto n : range(8)) {
    dma[n].addr = 0;
    dma[n].size = 0;
  }
}

auto SDD1::read(uint addr, uint8 data) -> uint8 {
  addr = 0x4800 | (addr & 7);

  switch(addr) {
  case 0x4804: return mmc[0] >> 20;
  case 0x4805: return mmc[1] >> 20;
  case 0x4806: return mmc[2] >> 20;
  case 0x4807: return mmc[3] >> 20;
  }

  return data;
}

auto SDD1::write(uint addr, uint8 data) -> void {
  addr = 0x4800 | (addr & 7);

  switch(addr) {
  case 0x4800: sdd1_enable = data; break;
  case 0x4801: xfer_enable = data; break;

  case 0x4804: mmc[0] = data << 20; break;
  case 0x4805: mmc[1] = data << 20; break;
  case 0x4806: mmc[2] = data << 20; break;
  case 0x4807: mmc[3] = data << 20; break;
  }
}

auto SDD1::dma_read(uint addr, uint8 data) -> uint8 {
  return cpu.mmio_read(addr, data);
}

auto SDD1::dma_write(uint addr, uint8 data) -> void {
  uint channel = (addr >> 4) & 7;
  switch(addr & 15) {
  case 2: dma[channel].addr = (dma[channel].addr & 0xffff00) + (data <<  0); break;
  case 3: dma[channel].addr = (dma[channel].addr & 0xff00ff) + (data <<  8); break;
  case 4: dma[channel].addr = (dma[channel].addr & 0x00ffff) + (data << 16); break;

  case 5: dma[channel].size = (dma[channel].size &   0xff00) + (data <<  0); break;
  case 6: dma[channel].size = (dma[channel].size &   0x00ff) + (data <<  8); break;
  }
  return cpu.mmio_write(addr, data);
}

auto SDD1::mmc_read(uint addr) -> uint8 {
  return rom.read(mmc[(addr >> 20) & 3] + (addr & 0x0fffff));
}

//SDD1::mcu_read() is mapped to $c0-ff:0000-ffff
//the design is meant to be as close to the hardware design as possible, thus this code
//avoids adding S-DD1 hooks inside S-CPU::DMA emulation.
//
//the real S-DD1 cannot see $420b (DMA enable) writes, as they are not placed on the bus.
//however, $43x0-$43xf writes (DMAx channel settings) most likely do appear on the bus.
//the S-DD1 also requires fixed addresses for transfers, which wouldn't be necessary if
//it could see $420b writes (eg it would know when the transfer should begin.)
//
//the hardware needs a way to distinguish program code after $4801 writes from DMA
//decompression that follows soon after.
//
//the only plausible design for hardware would be for the S-DD1 to spy on DMAx settings,
//and begin spooling decompression on writes to $4801 that activate a channel. after that,
//it feeds decompressed data only when the ROM read address matches the DMA channel address.
//
//the actual S-DD1 transfer can occur on any channel, but it is most likely limited to
//one transfer per $420b write (for spooling purposes). however, this is not known for certain.
auto SDD1::mcurom_read(uint addr, uint8) -> uint8 {
  //map address=00-3f,80-bf:8000-ffff mask=0x808000 => 00-1f:0000-ffff
  if(addr < 0x200000) {
    return rom.read(addr);
  }

  //map address=c0-ff:0000-ffff
  if(sdd1_enable & xfer_enable) {
    //at least one channel has S-DD1 decompression enabled ...
    for(auto n : range(8)) {
      if(sdd1_enable & xfer_enable & (1 << n)) {
        //S-DD1 always uses fixed transfer mode, so address will not change during transfer
        if(addr == dma[n].addr) {
          if(!dma_ready) {
            //prepare streaming decompression
            decomp.init(addr);
            dma_ready = true;
          }

          //fetch a decompressed byte; once finished, disable channel and invalidate buffer
          uint8 data = decomp.read();
          if(--dma[n].size == 0) {
            dma_ready = false;
            xfer_enable &= ~(1 << n);
          }

          return data;
        }  //address matched
      }  //channel enabled
    }  //channel loop
  }  //S-DD1 decompressor enabled

  //S-DD1 decompression mode inactive; return ROM data
  return mmc_read(addr);
}

auto SDD1::mcurom_write(uint addr, uint8 data) -> void {
}

//map address=00-3f,80-bf:6000-7fff mask=0xe000
//map address=70-7d:0000-7fff mask=0x8000
auto SDD1::mcuram_read(uint addr, uint8 data) -> uint8 {
  return ram.read(addr & 0x1fff, data);
}

auto SDD1::mcuram_write(uint addr, uint8 data) -> void {
  return ram.write(addr & 0x1fff, data);
}

}
