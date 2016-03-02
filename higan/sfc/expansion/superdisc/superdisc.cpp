#include <sfc/sfc.hpp>

namespace SuperFamicom {

SuperDisc superdisc;

auto SuperDisc::init() -> void {
}

auto SuperDisc::load() -> void {
  bus.map({&SuperDisc::read, &superdisc}, {&SuperDisc::write, &superdisc}, 0x00, 0x3f, 0x21e0, 0x21e5);
  bus.map({&SuperDisc::read, &superdisc}, {&SuperDisc::write, &superdisc}, 0x80, 0xbf, 0x21e0, 0x21e5);
}

auto SuperDisc::unload() -> void {
}

auto SuperDisc::power() -> void {
}

auto SuperDisc::reset() -> void {
}

auto SuperDisc::read(uint24 addr, uint8 data) -> uint8 {
  addr = 0x21e0 | (addr & 7);
  return data;
}

auto SuperDisc::write(uint24 addr, uint8 data) -> void {
  addr = 0x21e0 | (addr & 7);
}

}
