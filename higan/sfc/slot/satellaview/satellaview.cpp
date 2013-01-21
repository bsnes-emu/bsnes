#include <sfc/sfc.hpp>

#define SATELLAVIEW_CARTRIDGE_CPP
namespace SuperFamicom {

SatellaviewCartridge satellaviewcartridge;

void SatellaviewCartridge::init() {
}

void SatellaviewCartridge::load() {
  if(memory.size() == 0) {
    memory.map(allocate<uint8>(1024 * 1024, 0xff), 1024 * 1024);
  }
}

void SatellaviewCartridge::unload() {
  memory.reset();
}

void SatellaviewCartridge::power() {
}

void SatellaviewCartridge::reset() {
  regs.command   = 0;
  regs.write_old = 0x00;
  regs.write_new = 0x00;

  regs.flash_enable = false;
  regs.read_enable  = false;
  regs.write_enable = false;
  memory.write_protect(!regs.write_enable);
}

unsigned SatellaviewCartridge::size() const {
  return memory.size();
}

uint8 SatellaviewCartridge::read(unsigned addr) {
  if(readonly) return memory.read(bus.mirror(addr, memory.size()));

  if(addr == 0x0002) {
    if(regs.flash_enable) return 0x80;
  }

  if(addr == 0x5555) {
    if(regs.flash_enable) return 0x80;
  }

  if(regs.read_enable && addr >= 0xff00 && addr <= 0xff13) {
    //read flash cartridge vendor information
    switch(addr - 0xff00) {
    case 0x00: return 0x4d;
    case 0x01: return 0x00;
    case 0x02: return 0x50;
    case 0x03: return 0x00;
    case 0x04: return 0x00;
    case 0x05: return 0x00;
    case 0x06: return 0x2a;  //0x2a = 8mbit, 0x2b = 16mbit (not known to exist, though BIOS recognizes ID)
    case 0x07: return 0x00;
    default:   return 0x00;
    }
  }

  return memory.read(addr);
}

void SatellaviewCartridge::write(unsigned addr, uint8 data) {
  if(readonly) return;

  if((addr & 0xff0000) == 0) {
    regs.write_old = regs.write_new;
    regs.write_new = data;

    if(regs.write_enable && regs.write_old == regs.write_new) {
      return memory.write(addr, data);
    }
  } else {
    if(regs.write_enable) {
      return memory.write(addr, data);
    }
  }

  if(addr == 0x0000) {
    regs.command <<= 8;
    regs.command  |= data;

    if((regs.command & 0xffff) == 0x38d0) {
      regs.flash_enable = true;
      regs.read_enable  = true;
    }
  }

  if(addr == 0x2aaa) {
    regs.command <<= 8;
    regs.command  |= data;
  }

  if(addr == 0x5555) {
    regs.command <<= 8;
    regs.command  |= data;

    if((regs.command & 0xffffff) == 0xaa5570) {
      regs.write_enable = false;
    }

    if((regs.command & 0xffffff) == 0xaa55a0) {
      regs.write_old = 0x00;
      regs.write_new = 0x00;
      regs.flash_enable = true;
      regs.write_enable = true;
    }

    if((regs.command & 0xffffff) == 0xaa55f0) {
      regs.flash_enable = false;
      regs.read_enable  = false;
      regs.write_enable = false;
    }

    memory.write_protect(!regs.write_enable);
  }
}

}
