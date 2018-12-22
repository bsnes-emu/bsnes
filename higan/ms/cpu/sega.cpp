auto CPU::readSega(uint16 address) -> uint8 {
  uint8 data = 0xff;
  if(auto result = cartridge.read(address)) {
    data = result();
  } else if(address >= 0xc000) {
    data = ram.read(address);
  }
  if(auto result = cheat.find(address, data)) data = result();
  return data;
}

auto CPU::writeSega(uint16 address, uint8 data) -> void {
  if(cartridge.write(address, data)) {
  } else if(address >= 0xc000) {
    ram.write(address, data);
  }
}

auto CPU::inSega(uint8 addr) -> uint8 {
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
    if(Model::SG1000() || Model::SC3000()) {
      auto port1 = controllerPort1.device->readData();
      auto port2 = controllerPort2.device->readData();
      if(addr.bit(0) == 0) {
        return port1.bits(0,5) << 0 | port2.bits(0,1) << 6;
      } else {
        return port2.bits(2,5) << 0 | 1 << 4 | 1 << 5 | port1.bit(6) << 6 | port2.bit(6) << 7;
      }
    }

    if(Model::MasterSystem()) {
      bool reset = !platform->inputPoll(ID::Port::Hardware, ID::Device::MasterSystemControls, 0);
      auto port1 = controllerPort1.device->readData();
      auto port2 = controllerPort2.device->readData();
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

auto CPU::outSega(uint8 addr, uint8 data) -> void {
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
