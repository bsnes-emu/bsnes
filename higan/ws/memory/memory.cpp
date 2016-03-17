#include <ws/ws.hpp>

namespace WonderSwan {

InternalRAM iram;
Bus bus;

auto InternalRAM::power() -> void {
  for(auto& byte : memory) byte = 0x00;
}

auto InternalRAM::read(uint16 addr, uint size) -> uint32 {
  if(size == Long) return read(addr + 0, Word) << 0 | read(addr + 2, Word) << 16;
  if(size == Word) return read(addr + 0, Byte) << 0 | read(addr + 1, Byte) <<  8;

  if(addr >= 0x4000 && !system.r.depth) return 0x90;
  return memory[addr];
}

auto InternalRAM::write(uint16 addr, uint8 data) -> void {
  if(addr >= 0x4000 && !system.r.depth) return;
  memory[addr] = data;
}

auto Bus::power() -> void {
  for(auto& io : port) io = nullptr;
}

auto Bus::read(uint20 addr) -> uint8 {
  if(addr.bits(16,19) == 0) return iram.read(addr);
  if(addr.bits(16,19) == 1) return cartridge.ramRead(addr);
  if(addr.bits(16,19) >= 2) return cartridge.romRead(addr);
  unreachable;
}

auto Bus::write(uint20 addr, uint8 data) -> void {
  if(addr.bits(16,19) == 0) return iram.write(addr, data);
  if(addr.bits(16,19) == 1) return cartridge.ramWrite(addr, data);
  if(addr.bits(16,19) >= 2) return cartridge.romWrite(addr, data);
}

auto Bus::map(IO* io, uint16_t lo, maybe<uint16_t> hi) -> void {
  for(uint addr = lo; addr <= (hi ? hi() : lo); addr++) port[addr] = io;
}

auto Bus::portRead(uint16 addr) -> uint8 {
  if(auto io = port[addr]) return io->portRead(addr);
  return 0x00;
}

auto Bus::portWrite(uint16 addr, uint8 data) -> void {
  if(auto io = port[addr]) return io->portWrite(addr, data);
}

}
