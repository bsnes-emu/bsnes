#include <ms/ms.hpp>

namespace MasterSystem {

Bus bus;

auto Bus::read(uint16 addr) -> uint8 {
  if(auto data = cartridge.read(addr)) return data();
  if(addr >= 0xc000) return ram[addr & 0x1fff];
  return 0x00;
}

auto Bus::write(uint16 addr, uint8 data) -> void {
  if(cartridge.write(addr, data)) return;
  if(addr >= 0xc000) ram[addr & 0x1fff] = data;
}

auto Bus::in(uint8 addr) -> uint8 {
  switch(addr >> 6) {

  case 0: {
    return 0xff;  //SMS1 = MDR, SMS2 = 0xff
  }

  case 1: {
    return !addr.bit(0) ? vdp.vcounter() : vdp.hcounter();
  }

  case 2: {
    return !addr.bit(0) ? vdp.data() : vdp.status();
  }

  case 3: {
    auto A = peripherals.controllerPort1->readData();
    auto B = peripherals.controllerPort2->readData();
    if(addr.bit(0) == 0) {
      return A.bits(0,5) << 0 | B.bits(0,1) << 6;
    } else {
      //d4 = reset button
      //d5 = cartridge CONT pin
      return B.bits(2,5) << 0 | 1 << 4 | 1 << 5 | A.bit(6) << 6 | B.bit(6) << 7;
    }
  }

  }

  return 0xff;
}

auto Bus::out(uint8 addr, uint8 data) -> void {
  switch(addr >> 6) {

  case 2: {
    return !addr.bit(0) ? vdp.data(data) : vdp.control(data);
  }

  case 3: {
    return;  //unmapped
  }

  }
}

}
