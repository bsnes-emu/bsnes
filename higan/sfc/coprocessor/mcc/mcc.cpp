#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
MCC mcc;

auto MCC::init() -> void {
}

auto MCC::load() -> void {
}

auto MCC::unload() -> void {
  rom.reset();
  ram.reset();
}

auto MCC::power() -> void {
  for(auto n : range(16)) r[n] = 0x00;
  r[0x07] = 0x80;
  r[0x08] = 0x80;
  commit();
}

auto MCC::memoryAccess(bool write, Memory& memory, uint24 addr, uint8 data) -> uint8 {
  addr = bus.mirror(addr, memory.size());
  if(!write) {
    return memory.read(addr, data);
  } else {
    memory.write(addr, data);
  }
}

//map address=00-3f,80-bf:8000-ffff mask=0x408000
//map address=40-7d,c0-ff:0000-ffff
auto MCC::mcuAccess(bool write, uint24 addr, uint8 data) -> uint8 {
  if(addr < 0x400000) {
    //note: manifest maps 00-3f,80-bf:8000-ffff mask=0x408000 => 00-3f:0000-ffff
    //the intention is consistency in pre-decoding as much as possible
    //however, the MCC code is intended to be rewritten; and is too convoluted
    //so for right now, I'm simply transforming it back to its original state
    //this is very wasteful; but will be addressed once things are rewritten
    addr = ((addr & 0x200000) << 2) | ((addr & 0x1f8000) << 1) | 0x8000 | (addr & 0x7fff);
  }

  if((addr & 0xe08000) == 0x008000) {  //$00-1f:8000-ffff
    if(r07 == 1) {
      addr = ((addr & 0x1f0000) >> 1) | (addr & 0x7fff);
      return memoryAccess(write, rom, addr, data);
    }
  }

  if((addr & 0xe08000) == 0x808000) {  //$80-9f:8000-ffff
    if(r08 == 1) {
      addr = ((addr & 0x1f0000) >> 1) | (addr & 0x7fff);
      return memoryAccess(write, rom, addr, data);
    }
  }

  if((addr & 0xf00000) == 0x400000) {  //$40-4f:0000-ffff
    if(r05 == 0) return memoryAccess(write, ram, addr & 0x0fffff, data);
  }

  if((addr & 0xf00000) == 0x500000) {  //$50-5f:0000-ffff
    if(r06 == 0) return memoryAccess(write, ram, addr & 0x0fffff, data);
  }

  if((addr & 0xf00000) == 0x600000) {  //$60-6f:0000-ffff
    if(r03 == 1) return memoryAccess(write, ram, addr & 0x0fffff, data);
  }

  if((addr & 0xf80000) == 0x700000) {  //$70-77:0000-ffff
    return memoryAccess(write, ram, addr & 0x07ffff, data);
  }

  if(((addr & 0x408000) == 0x008000)  //$00-3f,80-bf:8000-ffff
  || ((addr & 0x400000) == 0x400000)  //$40-7f,c0-ff:0000-ffff
  ) {
    if(r02 == 0) addr = ((addr & 0x7f0000) >> 1) | (addr & 0x7fff);
    Memory& memory = (r01 == 0 ? (Memory&)bsmemory : (Memory&)ram);
    return memoryAccess(write, memory, addr & 0x7fffff, data);
  }

  return 0x00;
}

auto MCC::mcuRead(uint24 addr, uint8 data) -> uint8 {
  return mcuAccess(false, addr, data);
}

auto MCC::mcuWrite(uint24 addr, uint8 data) -> void {
  mcuAccess(true, addr, data);
}

auto MCC::read(uint24 addr, uint8 data) -> uint8 {
  if((addr & 0xf0ffff) == 0x005000) {  //$00-0f:5000
    uint8 n = (addr >> 16) & 15;
    return r[n];
  }

  return data;
}

auto MCC::write(uint24 addr, uint8 data) -> void {
  if((addr & 0xf0ffff) == 0x005000) {  //$00-0f:5000
    uint8 n = (addr >> 16) & 15;
    r[n] = data;
    if(n == 0x0e && data & 0x80) commit();
    return;
  }
}

auto MCC::commit() -> void {
  r00 = r[0x00] & 0x80;
  r01 = r[0x01] & 0x80;
  r02 = r[0x02] & 0x80;
  r03 = r[0x03] & 0x80;
  r04 = r[0x04] & 0x80;
  r05 = r[0x05] & 0x80;
  r06 = r[0x06] & 0x80;
  r07 = r[0x07] & 0x80;
  r08 = r[0x08] & 0x80;
  r09 = r[0x09] & 0x80;
  r0a = r[0x0a] & 0x80;
  r0b = r[0x0b] & 0x80;
  r0c = r[0x0c] & 0x80;
  r0d = r[0x0d] & 0x80;
  r0e = r[0x0e] & 0x80;
  r0f = r[0x0f] & 0x80;
}

}
