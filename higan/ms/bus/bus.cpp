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
    if(Model::GameGear()) {
      bool start = !platform->inputPoll(ID::Port::Hardware, ID::Device::GameGearControls, 6);
      return start << 7 | 0x7f;
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
    if(Model::MasterSystem()) {
      bool reset = !platform->inputPoll(ID::Port::Hardware, ID::Device::MasterSystemControls, 0);
      auto port1 = peripherals.controllerPort1->readData();
      auto port2 = peripherals.controllerPort2->readData();
      if(addr.bit(0) == 0) {
        return port1.bits(0,5) << 0 | port2.bits(0,1) << 6;
      } else {
        return port2.bits(2,5) << 0 | reset << 4 | 1 << 5 | port1.bit(6) << 6 | port2.bit(6) << 7;
      }
    }

    if(Model::GameGear()) {
      bool up    = !platform->inputPoll(ID::Port::Hardware, ID::Device::GameGearControls, 0);
      bool down  = !platform->inputPoll(ID::Port::Hardware, ID::Device::GameGearControls, 1);
      bool left  = !platform->inputPoll(ID::Port::Hardware, ID::Device::GameGearControls, 2);
      bool right = !platform->inputPoll(ID::Port::Hardware, ID::Device::GameGearControls, 3);
      bool one   = !platform->inputPoll(ID::Port::Hardware, ID::Device::GameGearControls, 4);
      bool two   = !platform->inputPoll(ID::Port::Hardware, ID::Device::GameGearControls, 5);
      if(!up && !down) up = 1, down = 1;
      if(!left && !right) left = 1, right = 1;
      if(addr.bit(0) == 0) {
        return up << 0 | down << 1 | left << 2 | right << 3 | one << 4 | two << 5 | 1 << 6 | 1 << 7;
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
  if(addr == 0x06) {
    if(Model::GameGear()) return psg.balance(data);
  }

  switch(addr >> 6) {

  case 1: {
    return psg.write(data);
  }

  case 2: {
    return !addr.bit(0) ? vdp.data(data) : vdp.control(data);
  }

  case 3: {
    return;  //unmapped
  }

  }
}

}
