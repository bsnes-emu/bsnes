#include <sfc/sfc.hpp>

#define SPC7110_CPP
namespace SuperFamicom {

#include "dcu.cpp"
#include "data.cpp"
#include "alu.cpp"
#include "serialization.cpp"
SPC7110 spc7110;

void SPC7110::Enter() { spc7110.enter(); }

void SPC7110::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(mul_wait) { if(--mul_wait == 0) alu_multiply(); }
    if(div_wait) { if(--div_wait == 0) alu_divide(); }

    step(1);
    synchronize_cpu();
  }
}

void SPC7110::init() {
}

void SPC7110::load() {
}

void SPC7110::unload() {
}

void SPC7110::power() {
}

void SPC7110::reset() {
  create(SPC7110::Enter, 32768 * 128);

  r4801 = 0x00;
  r4802 = 0x00;
  r4803 = 0x00;
  r4804 = 0x00;
  r4805 = 0x00;
  r4806 = 0x00;
  r4807 = 0x00;
  r4809 = 0x00;
  r480a = 0x00;
  r480b = 0x00;
  r480c = 0x00;

  dcu_mode = 0;
  dcu_addr = 0;
  dcu_sp = 0;
  dcu_dp = 0;

  r4810 = 0x00;
  r4811 = 0x00;
  r4812 = 0x00;
  r4813 = 0x00;
  r4814 = 0x00;
  r4815 = 0x00;
  r4816 = 0x00;
  r4817 = 0x00;
  r4818 = 0x00;
  r481a = 0x00;

  r4820 = 0x00;
  r4821 = 0x00;
  r4822 = 0x00;
  r4823 = 0x00;
  r4824 = 0x00;
  r4825 = 0x00;
  r4826 = 0x00;
  r4827 = 0x00;
  r4828 = 0x00;
  r4829 = 0x00;
  r482a = 0x00;
  r482b = 0x00;
  r482c = 0x00;
  r482d = 0x00;
  r482e = 0x00;
  r482f = 0x00;

  mul_wait = 0;
  div_wait = 0;

  r4830 = 0x00;
  r4831 = 0x00;
  r4832 = 0x01;
  r4833 = 0x02;
  r4834 = 0x00;
}

uint8 SPC7110::mmio_read(unsigned addr) {
  cpu.synchronize_coprocessors();
  addr = 0x4800 | (addr & 0x3f);

  switch(addr) {

  //==================
  //decompression unit
  //==================

  case 0x4800: {
    uint16 counter = r4809 | r480a << 8;
    counter--;
    r4809 = counter >> 0;
    r480a = counter >> 8;
    return dcu_read();
  }
  case 0x4801: return r4801;
  case 0x4802: return r4802;
  case 0x4803: return r4803;
  case 0x4804: return r4804;
  case 0x4805: return r4805;
  case 0x4806: return r4806;
  case 0x4807: return r4807;
  case 0x4808: return 0x00;
  case 0x4809: return r4809;
  case 0x480a: return r480a;
  case 0x480b: return r480b;
  case 0x480c: {
    uint8 status = r480c;
    r480c &= 0x7f;
    return status;
  }

  //==============
  //data port unit
  //==============

  case 0x4810: {
    uint8 data = r4810;
    data_port_increment_4810();
    data_port_read();
    return data;
  }
  case 0x4811: return r4811;
  case 0x4812: return r4812;
  case 0x4813: return r4813;
  case 0x4814: return r4814;
  case 0x4815: return r4815;
  case 0x4816: return r4816;
  case 0x4817: return r4817;
  case 0x4818: return r4818;
  case 0x481a: {
    data_port_increment_481a();
    return 0x00;
  }

  //=====================
  //arithmetic logic unit
  //=====================

  case 0x4820: return r4820;
  case 0x4821: return r4821;
  case 0x4822: return r4822;
  case 0x4823: return r4823;
  case 0x4824: return r4824;
  case 0x4825: return r4825;
  case 0x4826: return r4826;
  case 0x4827: return r4827;
  case 0x4828: return r4828;
  case 0x4829: return r4829;
  case 0x482a: return r482a;
  case 0x482b: return r482b;
  case 0x482c: return r482c;
  case 0x482d: return r482d;
  case 0x482e: return r482e;
  case 0x482f: return r482f;

  //===================
  //memory control unit
  //===================

  case 0x4830: return r4830;
  case 0x4831: return r4831;
  case 0x4832: return r4832;
  case 0x4833: return r4833;
  case 0x4834: return r4834;

  }

  return cpu.regs.mdr;
}

void SPC7110::mmio_write(unsigned addr, uint8 data) {
  cpu.synchronize_coprocessors();
  addr = 0x4800 | (addr & 0x3f);

  switch(addr) {

  //==================
  //decompression unit
  //==================

  case 0x4801: r4801 = data; break;
  case 0x4802: r4802 = data; break;
  case 0x4803: r4803 = data & 0x7f; break;
  case 0x4804: r4804 = data; dcu_load_address(); break;
  case 0x4805: r4805 = data; break;
  case 0x4806: r4806 = data; dcu_begin_transfer(); break;
  case 0x4807: r4807 = data; break;
  case 0x4808: break;
  case 0x4809: r4809 = data; break;
  case 0x480a: r480a = data; break;
  case 0x480b: r480b = data & 0x03; break;

  //==============
  //data port unit
  //==============

  case 0x4811: r4811 = data; break;
  case 0x4812: r4812 = data; break;
  case 0x4813: r4813 = data & 0x7f; data_port_read(); break;
  case 0x4814: r4814 = data; data_port_increment_4814(); break;
  case 0x4815: r4815 = data; data_port_increment_4815(); break;
  case 0x4816: r4816 = data; break;
  case 0x4817: r4817 = data; break;
  case 0x4818: r4818 = data & 0x7f; data_port_read(); break;

  //=====================
  //arithmetic logic unit
  //=====================

  case 0x4820: r4820 = data; break;
  case 0x4821: r4821 = data; break;
  case 0x4822: r4822 = data; break;
  case 0x4823: r4823 = data; break;
  case 0x4824: r4824 = data; break;
  case 0x4825: r4825 = data; r482f |= 0x81; mul_wait = mul_delay; break;
  case 0x4826: r4826 = data; break;
  case 0x4827: r4827 = data; r482f |= 0x80; div_wait = div_delay; break;
  case 0x482e: r482e = data & 0x01; break;

  //===================
  //memory control unit
  //===================

  case 0x4830: r4830 = data & 0x87; break;
  case 0x4831: r4831 = data & 0x07; break;
  case 0x4832: r4832 = data & 0x07; break;
  case 0x4833: r4833 = data & 0x07; break;
  case 0x4834: r4834 = data & 0x07; break;

  }
}

SPC7110::SPC7110() {
}

//============
//SPC7110::DCU
//============

uint8 SPC7110::dcu_read(unsigned) {
  return mmio_read(0x4800);
}

void SPC7110::dcu_write(unsigned, uint8) {
}

//===============
//SPC7110::MCUROM
//===============

uint8 SPC7110::mcurom_read(unsigned addr) {
  unsigned mask = (1 << (r4834 & 3)) - 1;  //8mbit, 16mbit, 32mbit, 64mbit DROM

  if((addr & 0x708000) == 0x008000  //$00-0f|80-8f:8000-ffff
  || (addr & 0xf00000) == 0xc00000  //      $c0-cf:0000-ffff
  ) {
    addr &= 0x0fffff;
    if(prom_size) {  //8mbit PROM
      return cartridge.rom.read(prom_base + bus.mirror(0x000000 + addr, prom_size));
    }
    addr |= 0x100000 * (r4830 & 7);
    return datarom_read(addr);
  }

  if((addr & 0x708000) == 0x108000  //$10-1f|90-9f:8000-ffff
  || (addr & 0xf00000) == 0xd00000  //      $d0-df:0000-ffff
  ) {
    addr &= 0x0fffff;
    if(r4834 & 4) {  //16mbit PROM
      return cartridge.rom.read(prom_base + bus.mirror(0x100000 + addr, prom_size));
    }
    addr |= 0x100000 * (r4831 & 7);
    return datarom_read(addr);
  }

  if((addr & 0x708000) == 0x208000  //$20-2f|a0-af:8000-ffff
  || (addr & 0xf00000) == 0xe00000  //      $e0-ef:0000-ffff
  ) {
    addr &= 0x0fffff;
    addr |= 0x100000 * (r4832 & 7);
    return datarom_read(addr);
  }

  if((addr & 0x708000) == 0x308000  //$30-3f|b0-bf:8000-ffff
  || (addr & 0xf00000) == 0xf00000  //      $f0-ff:0000-ffff
  ) {
    addr &= 0x0fffff;
    addr |= 0x100000 * (r4833 & 7);
    return datarom_read(addr);
  }

  return cpu.regs.mdr;
}

void SPC7110::mcurom_write(unsigned addr, uint8 data) {
}

//===============
//SPC7110::MCURAM
//===============

uint8 SPC7110::mcuram_read(unsigned addr) {
  //$00-3f|80-bf:6000-7fff
  if(r4830 & 0x80) {
    unsigned bank = (addr >> 16) & 0x3f;
    addr = bus.mirror(bank * 0x2000 + (addr & 0x1fff), cartridge.ram.size());
    return cartridge.ram.read(addr);
  }
  return 0x00;
}

void SPC7110::mcuram_write(unsigned addr, uint8 data) {
  //$00-3f|80-bf:6000-7fff
  if(r4830 & 0x80) {
    unsigned bank = (addr >> 16) & 0x3f;
    addr = bus.mirror(bank * 0x2000 + (addr & 0x1fff), cartridge.ram.size());
    cartridge.ram.write(addr, data);
  }
}

}
