auto VDP::vcounter() -> uint8 {
  switch(io.mode) {
  default:     return io.vcounter <= 218 ? io.vcounter : io.vcounter - 6;  //256x192
  case 0b1011: return io.vcounter <= 234 ? io.vcounter : io.vcounter - 6;  //256x224
  case 0b1110: return io.vcounter;  //256x240
  }
}

auto VDP::hcounter() -> uint8 {
  uint hcounter = io.hcounter >> 1;
  return hcounter <= 233 ? hcounter : hcounter - 86;
}

auto VDP::data() -> uint8 {
  io.controlLatch = 0;

  auto data = io.vramLatch;
  io.vramLatch = vram[io.address++];
  return data;
}

auto VDP::status() -> uint8 {
  io.controlLatch = 0;

  uint8 result = 0x00;
  result |= io.intFrame << 7;
  result |= io.spriteOverflow << 6;
  result |= io.spriteCollision << 5;
  result |= io.fifthSprite << 0;

  io.intLine = 0;
  io.intFrame = 0;
  io.spriteOverflow = 0;
  io.spriteCollision = 0;
  io.fifthSprite = 0;

  return result;
}

auto VDP::data(uint8 data) -> void {
  io.controlLatch = 0;

  if(io.code <= 2) {
    vram[io.address++] = data;
  } else {
    uint mask = 0;
    if(Model::MasterSystem()) cram[io.address++ & 0x1f] = data;
    if(Model::GameGear())     cram[io.address++ & 0x3f] = data;
  }
}

auto VDP::control(uint8 data) -> void {
  if(io.controlLatch == 0) {
    io.controlLatch = 1;
    io.address.bits(0,7) = data.bits(0,7);
    return;
  } else {
    io.controlLatch = 0;
    io.address.bits(8,13) = data.bits(0,5);
    io.code.bits(0,1) = data.bits(6,7);
  }

  if(io.code == 0) {
    io.vramLatch = vram[io.address++];
  }

  if(io.code == 2) {
    registerWrite(io.address.bits(11,8), io.address.bits(7,0));
  }
}

auto VDP::registerWrite(uint4 addr, uint8 data) -> void {
  switch(addr) {

  //mode control 1
  case 0x0: {
    io.externalSync = data.bit(0);
    io.mode.bit(1) = data.bit(1);
    io.mode.bit(3) = data.bit(2) & !Model::SG1000() & !Model::SC3000();
    io.spriteShift = data.bit(3);
    io.lineInterrupts = data.bit(4);
    io.leftClip = data.bit(5);
    io.horizontalScrollLock = data.bit(6);
    io.verticalScrollLock = data.bit(7);
    return;
  }

  //mode control 2
  case 0x1: {
    io.spriteDouble = data.bit(0);
    io.spriteTile = data.bit(1);
    io.mode.bit(2) = data.bit(3);
    io.mode.bit(0) = data.bit(4);
    io.frameInterrupts = data.bit(5);
    io.displayEnable = data.bit(6);
    return;
  }

  //name table base address
  case 0x2: {
    io.nameTableAddress = data.bits(0,3);
    return;
  }

  //color table base address
  case 0x3: {
    io.colorTableAddress = data.bits(0,7);
    return;
  }

  //pattern table base address
  case 0x4: {
    io.patternTableAddress = data.bits(0,2);
    return;
  }

  //sprite attribute table base address
  case 0x5: {
    io.spriteAttributeTableAddress = data.bits(0,6);
    return;
  }

  //sprite pattern table base address
  case 0x6: {
    io.spritePatternTableAddress = data.bits(0,2);
    return;
  }

  //backdrop color
  case 0x7: {
    io.backdropColor = data.bits(0,3);
    return;
  }

  //horizontal scroll offset
  case 0x8: {
    io.hscroll = data.bits(0,7);
    return;
  }

  //vertical scroll offset
  case 0x9: {
    io.vscroll = data.bits(0,7);
    return;
  }

  //line counter
  case 0xa: {
    io.lineCounter = data.bits(0,7);
    return;
  }

  //0xb - 0xf unmapped

  }
}
