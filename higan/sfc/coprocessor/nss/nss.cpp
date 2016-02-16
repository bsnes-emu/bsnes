#include <sfc/sfc.hpp>

namespace SuperFamicom {

NSS nss;

auto NSS::init() -> void {
}

auto NSS::load() -> void {
}

auto NSS::unload() -> void {
}

auto NSS::power() -> void {
}

auto NSS::reset() -> void {
}

auto NSS::set_dip(uint16 dip) -> void {
  this->dip = dip;
}

auto NSS::read(uint24, uint8) -> uint8 {
  return dip;
}

auto NSS::write(uint24, uint8) -> void {
}

}
