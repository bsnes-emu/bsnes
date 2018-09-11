#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "serialization.cpp"
BSMemory bsmemory;

auto BSMemory::load() -> void {
  queryable(true);
  flashable(true);
}

auto BSMemory::unload() -> void {
  memory.reset();
}

auto BSMemory::power() -> void {
  memory.writable(false);
  io = {};
}

auto BSMemory::data() -> uint8* {
  return memory.data();
}

auto BSMemory::size() const -> uint {
  return memory.size();
}

auto BSMemory::read(uint24 address, uint8 data) -> uint8 {
  if(!size()) return data;
  address = bus.mirror(address, size());
  if(!pin.queryable) return memory.read(address, data);

  if(io.mode == 0x70) {
    return 0x80;
  }

  if(io.mode == 0x71) {
    if((uint16)address == 0x0002) return 0x80;
    if((uint16)address == 0x0004) return 0x87;
    if((uint16)address == 0x0006) return 0x00;  //unknown purpose (not always zero)
    return 0x00;
  }

  if(io.mode == 0x75) {
    if((uint8)address == 0x00) return 0x4d;  //'M' (memory)
    if((uint8)address == 0x02) return 0x50;  //'P' (pack)
    if((uint8)address == 0x04) return 0x04;  //unknown purpose
    if((uint8)address == 0x06) return Type << 4 | (uint4)log2(size() >> 10);
    return random();  //not actually random, but not ROM data either, yet varies per cartridge
  }

  return memory.read(address, data);
}

auto BSMemory::write(uint24 address, uint8 data) -> void {
  if(!size() || !pin.queryable) return;
  address = bus.mirror(address, size());

  //write byte
  if(io.mode == 0x10 || io.mode == 0x40) {
    if(!pin.flashable) return;
    memory.writable(true);
    memory.write(address, memory.read(address) & data);  //writes can only clear bits
    memory.writable(false);
    io.mode = 0x70;
    return;
  }

  //erase 64KB page
  if(io.mode == 0x20) {
    //completes even if !pin.flashable
    memory.writable(true);
    for(uint offset : range(1 << 16)) memory.write(address & 0xff0000 | offset, 0xff);
    memory.writable(false);
    io.mode = 0x70;
    return;
  }

  //erase all pages
  if(io.mode == 0xa7) {
    //completes even if !pin.flashable
    if(Type == 3) return;  //Type 3 doesn't support this command

    memory.writable(true);
    for(uint offset : range(size())) memory.write(offset, 0xff);
    memory.writable(false);
    io.mode = 0x70;
    return;
  }

  if((uint16)address == 0x0000) {
    io.mode = data;
  }
}

}
