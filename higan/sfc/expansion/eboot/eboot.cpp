#include <sfc/sfc.hpp>

namespace SuperFamicom {

eBoot eboot;

auto eBoot::init() -> void {
}

auto eBoot::load() -> void {
  resetVector.byte(0) = bus.read(0xfffc, 0x00);
  resetVector.byte(1) = bus.read(0xfffd, 0x00);

  for(auto& byte : ram) byte = 0xdb;  //stp
  ram[0] = 0x6c;  //jmp ($fffc)
  ram[1] = 0xfc;
  ram[2] = 0xff;

  bus.map({&eBoot::read, &eboot}, {&eBoot::write, &eboot}, 0x00, 0x00, 0xfffc, 0xfffd);
  bus.map({&eBoot::read, &eboot}, {&eBoot::write, &eboot}, 0x00, 0x3f, 0x2184, 0x21ff);
  bus.map({&eBoot::read, &eboot}, {&eBoot::write, &eboot}, 0x80, 0xbf, 0x2184, 0x21ff);

  if(auto buffer = file::read({interface->path(ID::System), "eboot.rom"})) {
    memory::copy(ram, sizeof(ram), buffer.data(), buffer.size());
  }
}

auto eBoot::unload() -> void {
}

auto eBoot::power() -> void {
}

auto eBoot::reset() -> void {
  booted = false;
}

auto eBoot::read(uint24 addr, uint8 data) -> uint8 {
  addr &= 0x40ffff;
  if(addr == 0xfffc) return booted ? resetVector.byte(0) : (uint8)0x84;
  if(addr == 0xfffd) return booted ? resetVector.byte(1) : (booted = true, (uint8)0x21);
  if(addr >= 0x2184 && addr <= 0x21ff) return ram[addr - 0x2184];
  return data;
}

auto eBoot::write(uint24 addr, uint8 data) -> void {
}

}
