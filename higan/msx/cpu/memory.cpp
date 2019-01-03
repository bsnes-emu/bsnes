auto CPU::read(uint16 address) -> uint8 {
  uint2 slot = io.slot[address.bits(14,15)];

  if(slot == 0) {
    if(!address.bit(15)) return system.bios.read(address);
    return 0xff;
  }

  if(slot == 1) {
    return cartridge.read(address);
  }

  if(slot == 2) {
    return ram.read(address);
  }

  if(slot == 3) {
  }

  return 0xff;
}

auto CPU::write(uint16 address, uint8 data) -> void {
  uint2 slot = io.slot[address.bits(14,15)];

  if(slot == 0) {
    return;
  }

  if(slot == 1) {
    return cartridge.write(address, data);
  }

  if(slot == 2) {
    return ram.write(address, data);
  }

  if(slot == 3) {
    return;
  }
}

auto CPU::in(uint8 address) -> uint8 {
  switch(address) {
  case 0x98: return vdp.data();
  case 0x99: return vdp.status();
  case 0xa8: return io.slot[0] << 0
                  | io.slot[1] << 2
                  | io.slot[2] << 4
                  | io.slot[3] << 6;
  }
  return 0xff;
}

auto CPU::out(uint8 address, uint8 data) -> void {
  switch(address) {
  case 0x98: return vdp.data(data);
  case 0x99: return vdp.control(data);
  case 0xa8: io.slot[0] = data.bits(0,1);
             io.slot[1] = data.bits(2,3);
             io.slot[2] = data.bits(4,5);
             io.slot[3] = data.bits(6,7);
             break;
  }
}
