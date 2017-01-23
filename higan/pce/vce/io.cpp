auto VCE::read(uint3 addr) -> uint8 {
  if(addr == 0x04) {
    //CTR
    uint8 data = cram.read(cram.address).bits(0,7);
    return data;
  }

  if(addr == 0x05) {
    //CTR
    uint1 data = cram.read(cram.address).bit(8);
    cram.address++;
    return 0xfe | data;
  }

  return 0xff;
}

auto VCE::write(uint3 addr, uint8 data) -> void {
  if(addr == 0x00) {
    //CR
    if(data.bits(0,1) == 0) io.clock = 4;
    if(data.bits(0,1) == 1) io.clock = 3;
    if(data.bits(0,1) == 2) io.clock = 2;
    if(data.bits(0,1) == 3) io.clock = 2;
    io.extraLine = data.bit(2);
    io.grayscale = data.bit(7);
    return;
  }

  if(addr == 0x02) {
    //CTA
    cram.address.bits(0,7) = data.bits(0,7);
    return;
  }

  if(addr == 0x03) {
    //CTA
    cram.address.bit(8) = data.bit(0);
    return;
  }

  if(addr == 0x04) {
    //CTW
    cram.write(cram.address, 0, data.bits(0,7));
    return;
  }

  if(addr == 0x05) {
    //CTW
    cram.write(cram.address, 1, data.bit(0));
    cram.address++;
    return;
  }
}
