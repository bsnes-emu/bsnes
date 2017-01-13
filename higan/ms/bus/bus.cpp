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
    if(system.model() == Model::GameGear) {
      auto hardware = peripherals.hardware->readData();
      return hardware.bit(6) << 7 | 0x7f;
    }

    return 0xff;  //SMS1 = MDR, SMS2 = 0xff
  }

  case 1: {
    return !addr.bit(0) ? vdp.vcounter() : vdp.hcounter();
  }

  case 2: {
    return !addr.bit(0) ? vdp.data() : vdp.status();
  }

  case 3: {
    if(system.model() == Model::MasterSystem) {
      auto hardware = peripherals.hardware->readData();
      auto port1 = peripherals.controllerPort1->readData();
      auto port2 = peripherals.controllerPort2->readData();
      if(addr.bit(0) == 0) {
        return port1.bits(0,5) << 0 | port2.bits(0,1) << 6;
      } else {
        return port2.bits(2,5) << 0 | hardware.bit(0) << 4 | 1 << 5 | port1.bit(6) << 6 | port2.bit(6) << 7;
      }
    }
    if(system.model() == Model::GameGear) {
      auto hardware = peripherals.hardware->readData();
      if(addr.bit(0) == 0) {
        return hardware.bits(0,5) << 0 | 0xc0;
      } else {
        return 0xff;
      }
    }
    return 0xff;
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
