auto APU::read(uint16 addr) -> uint8 {
  if((addr & 0xe000) == 0x0000) {
    return ram[addr];
  }

  if(addr == 0x4000) return ym2612.readStatus();
  if(addr == 0x4001) return ym2612.readStatus();
  if(addr == 0x4002) return ym2612.readStatus();
  if(addr == 0x4003) return ym2612.readStatus();

  if((addr & 0x8000) == 0x8000) {
    return cartridge.read(io.bank << 15 | (addr & 0x7ffe)).byte(!addr.bit(0));
  }
}

auto APU::write(uint16 addr, uint8 data) -> void {
  if((addr & 0xe000) == 0x0000) {
    ram[addr] = data;
    return;
  }

  if(addr == 0x4000) return ym2612.writeAddress(0 << 8 | data);
  if(addr == 0x4001) return ym2612.writeData(data);
  if(addr == 0x4002) return ym2612.writeAddress(1 << 8 | data);
  if(addr == 0x4003) return ym2612.writeData(data);

  if(addr == 0x6000) {
    //1-bit shift register
    io.bank = data.bit(0) << 8 | io.bank >> 1;
    return;
  }

  if(addr == 0x7f11) return psg.write(data);
  if(addr == 0x7f13) return psg.write(data);
  if(addr == 0x7f15) return psg.write(data);
  if(addr == 0x7f17) return psg.write(data);

  if((addr & 0x8000) == 0x8000) {
    //todo: do 8-bit writes mirror to 16-bits?
    return cartridge.write(io.bank << 15 | (addr & 0x7ffe), data << 8 | data << 0);
  }
}

//unused on Mega Drive
auto APU::in(uint8 addr) -> uint8 {
  return 0x00;
}

//unused on Mega Drive
auto APU::out(uint8 addr, uint8 data) -> void {
}
