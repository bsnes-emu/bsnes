auto VDP::status() -> uint8 {
  io.controlLatch = 0;
  uint8 data = 0x00;
  data.bits(0,4) = io.spriteOverflowIndex;
  data.bit(5) = io.spriteCollision;
  data.bit(6) = io.spriteOverflow;
  data.bit(7) = io.irqLine;
  io.irqLine = 0;
  return data;
}

auto VDP::data() -> uint8 {
  io.controlLatch = 0;
  uint8 data = io.vramLatch;
  uint14 address = io.controlValue.bits(0,13)++;
  io.vramLatch = vram.read(address);
  return data;
}

auto VDP::data(uint8 data) -> void {
  io.controlLatch = 0;
  uint14 address = io.controlValue.bits(0,13)++;
  vram.write(address, data);
}

auto VDP::control(uint8 data) -> void {
  if(io.controlLatch == 0) {
    io.controlLatch = 1;
    io.controlValue.byte(0) = data;
    return;
  } else {
    io.controlLatch = 0;
    io.controlValue.byte(1) = data;
  }

  if(!io.controlValue.bit(15)) {
    if(!io.controlValue.bit(14)) {
      uint14 address = io.controlValue.bits(0,13)++;
      io.vramLatch = vram.read(address);
    }
    return;
  }

  data = io.controlValue.bits(0,7);
  switch(io.controlValue.bits(8,10)) {
  case 0:
    io.externalInput = data.bit(0);
    io.videoMode.bit(1) = data.bit(1);
    break;
  case 1:
    io.spriteZoom = data.bit(0);
    io.spriteSize = data.bit(1);
    io.videoMode.bit(2) = data.bit(3);
    io.videoMode.bit(0) = data.bit(4);
    io.irqEnable = data.bit(5);
    io.displayEnable = data.bit(6);
    io.ramMode = data.bit(7);
    break;
  case 2:
    io.nameTableAddress = data.bits(0,3);
    break;
  case 3:
    io.colorTableAddress = data.bits(0,7);
    break;
  case 4:
    io.patternTableAddress = data.bits(0,2);
    break;
  case 5:
    io.spriteAttributeTableAddress = data.bits(0,6);
    break;
  case 6:
    io.spritePatternTableAddress = data.bits(0,2);
    break;
  case 7:
    io.colorBackground = data.bits(0,3);
    io.colorForeground = data.bits(4,7);
    break;
  }
}
