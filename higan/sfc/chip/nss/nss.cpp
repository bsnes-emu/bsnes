#include <sfc/sfc.hpp>

#define NSS_CPP
namespace SuperFamicom {

NSS nss;

void NSS::init() {
  dip = 0x00;
}

void NSS::load() {
}

void NSS::unload() {
}

void NSS::power() {
}

void NSS::reset() {
}

void NSS::set_dip(uint16 dip) {
  this->dip = dip;
}

uint8 NSS::read(unsigned addr) {
  return dip;
}

void NSS::write(unsigned addr, uint8 data) {
}

}
