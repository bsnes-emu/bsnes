auto SA1::idle() -> void {
  r.rwb = 0;
  step(2);
}

//RTx, JMx, JSx
auto SA1::idleJump() -> void {
  //ROM access penalty cycle: does not apply to BWRAM or IRAM
  if((r.pc & 0x408000) == 0x008000 || (r.pc & 0xc00000) == 0xc00000) idle();
}

//Bxx
auto SA1::idleBranch() -> void {
  if(r.pc & 1) idleJump();
}

auto SA1::read(uint24 address) -> uint8 {
  r.rwb = 1;
  r.mar = address;
  uint8 data = r.mdr;

  //00-3f,80-bf:2200-23ff
  if((address & 0x40fe00) == 0x002200) {
    step(2);
    return r.mdr = readIO(address, data);
  }

  //00-3f,80-bf:8000-ffff
  if((address & 0x408000) == 0x008000) {
    step(rom.conflict() ? 4 : 2);
    return r.mdr = rom.readSA1(address, data);
  }

  //c0-ff:0000-ffff
  if((address & 0xc00000) == 0xc00000) {
    step(rom.conflict() ? 4 : 2);
    return r.mdr = rom.readSA1(address, data);
  }

  //00-3f,80-bf:6000-7fff
  if((address & 0x40e000) == 0x006000) {
    step(bwram.conflict() ? 8 : 4);
    return r.mdr = bwram.readSA1(address, data);
  }

  //00-3f,80-bf:0000-07ff
  if((address & 0x40f800) == 0x000000) {
    step(iram.conflict() ? 6 : 2);
    return r.mdr = iram.readSA1(address, data);
  }

  //00-3f,80-bf:3000-37ff
  if((address & 0x40f800) == 0x003000) {
    step(iram.conflict() ? 6 : 2);
    return r.mdr = iram.readSA1(address, data);
  }

  //40-4f:0000-ffff
  if((address & 0xf00000) == 0x400000) {
    step(bwram.conflict() ? 8 : 4);
    return r.mdr = bwram.readLinear(address, data);
  }

  //60-6f:0000-ffff
  if((address & 0xf00000) == 0x600000) {
    step(bwram.conflict() ? 8 : 4);
    return r.mdr = bwram.readBitmap(address, data);
  }

  //unmapped region
  step(2);
  return data;
}

auto SA1::write(uint24 address, uint8 data) -> void {
  r.rwb = 1;
  r.mar = address;
  r.mdr = data;

  //00-3f,80-bf:2200-23ff
  if((address & 0x40fe00) == 0x002200) {
    step(2);
    return writeIO(address, data);
  }

  //00-3f,80-bf:8000-ffff
  if((address & 0x408000) == 0x008000) {
    step(rom.conflict() ? 4 : 2);
    return rom.writeSA1(address, data);
  }

  //c0-ff:0000-ffff
  if((address & 0xc00000) == 0xc00000) {
    step(rom.conflict() ? 4 : 2);
    return rom.writeSA1(address, data);
  }

  //00-3f,80-bf:6000-7fff
  if((address & 0x40e000) == 0x006000) {
    step(bwram.conflict() ? 8 : 4);
    return bwram.writeSA1(address, data);
  }

  //00-3f,80-bf:0000-07ff
  if((address & 0x40f800) == 0x000000) {
    step(iram.conflict() ? 6 : 2);
    return iram.writeSA1(address, data);
  }

  //00-3f,80-bf:3000-37ff
  if((address & 0x40f800) == 0x003000) {
    step(iram.conflict() ? 6 : 2);
    return iram.writeSA1(address, data);
  }

  //40-4f:0000-ffff
  if((address & 0xf00000) == 0x400000) {
    step(bwram.conflict() ? 8 : 4);
    return bwram.writeLinear(address, data);
  }

  //60-6f:0000-ffff
  if((address & 0xf00000) == 0x600000) {
    step(bwram.conflict() ? 8 : 4);
    return bwram.writeBitmap(address, data);
  }

  //unmapped region
  step(2);
  return;
}

//$230c (VDPL), $230d (VDPH) use this bus to read variable-length data.
//this is used both to keep VBR-reads from accessing MMIO registers, and
//to avoid syncing the S-CPU and SA-1*; as both chips are able to access
//these ports.
auto SA1::readVBR(uint24 address, uint8 data) -> uint8 {
  //00-3f,80-bf:8000-ffff
  if((address & 0x408000) == 0x008000) {
    return rom.readSA1(address, data);
  }

  //c0-ff:0000-ffff
  if((address & 0xc00000) == 0xc00000) {
    return rom.readSA1(address, data);
  }

  //00-3f,80-bf:6000-7fff
  if((address & 0x40e000) == 0x006000) {
    return bwram.read(address, data);
  }

  //40-4f:0000-ffff
  if((address & 0xf00000) == 0x400000) {
    return bwram.read(address, data);
  }

  //00-3f,80-bf:0000-07ff
  if((address & 0x40f800) == 0x000000) {
    return iram.read(address, data);
  }

  //00-3f,80-bf:3000-37ff
  if((address & 0x40f800) == 0x003000) {
    return iram.read(address, data);
  }

  return 0x00;
}
