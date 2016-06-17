#include <sfc/sfc.hpp>

namespace SuperFamicom {

BSMemory bsmemory;

auto BSMemory::init() -> void {
}

auto BSMemory::load() -> void {
  if(memory.size() == 0) {
    memory.map(allocate<uint8>(1024 * 1024, 0xff), 1024 * 1024);
  }
}

auto BSMemory::unload() -> void {
  memory.reset();
}

auto BSMemory::power() -> void {
}

auto BSMemory::reset() -> void {
  regs.command   = 0;
  regs.writeOld = 0x00;
  regs.writeNew = 0x00;

  regs.flashEnable = false;
  regs.readEnable  = false;
  regs.writeEnable = false;
  memory.writeProtect(!regs.writeEnable);
}

auto BSMemory::size() const -> uint {
  return memory.size();
}

auto BSMemory::read(uint24 addr, uint8 data) -> uint8 {
  if(readonly) {
    return memory.read(bus.mirror(addr, memory.size()), data);
  }

  if(addr == 0x0002) {
    if(regs.flashEnable) return 0x80;
  }

  if(addr == 0x5555) {
    if(regs.flashEnable) return 0x80;
  }

  if(regs.readEnable && addr >= 0xff00 && addr <= 0xff13) {
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

  return memory.read(addr, data);
}

auto BSMemory::write(uint24 addr, uint8 data) -> void {
  if(readonly) {
    return;
  }

  if((addr & 0xff0000) == 0) {
    regs.writeOld = regs.writeNew;
    regs.writeNew = data;

    if(regs.writeEnable && regs.writeOld == regs.writeNew) {
      return memory.write(addr, data);
    }
  } else {
    if(regs.writeEnable) {
      return memory.write(addr, data);
    }
  }

  if(addr == 0x0000) {
    regs.command <<= 8;
    regs.command  |= data;

    if((regs.command & 0xffff) == 0x38d0) {
      regs.flashEnable = true;
      regs.readEnable  = true;
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
      regs.writeEnable = false;
    }

    if((regs.command & 0xffffff) == 0xaa55a0) {
      regs.writeOld = 0x00;
      regs.writeNew = 0x00;
      regs.flashEnable = true;
      regs.writeEnable = true;
    }

    if((regs.command & 0xffffff) == 0xaa55f0) {
      regs.flashEnable = false;
      regs.readEnable  = false;
      regs.writeEnable = false;
    }

    memory.writeProtect(!regs.writeEnable);
  }
}

}
