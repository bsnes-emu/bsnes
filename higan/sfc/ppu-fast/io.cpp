auto PPU::readIO(uint24 address, uint8 data) -> uint8 {
  cpu.synchronize(ppu);

  switch((uint16)address) {

  case 0x2104: case 0x2105: case 0x2106: case 0x2108:
  case 0x2109: case 0x210a: case 0x2114: case 0x2115:
  case 0x2116: case 0x2118: case 0x2119: case 0x211a:
  case 0x2124: case 0x2125: case 0x2126: case 0x2128:
  case 0x2129: case 0x212a: {
    return ppu1.mdr;
  }

  case 0x2134: {  //MPYL
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(0);
  }

  case 0x2135: {  //MPYM
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(1);
  }

  case 0x2136: {  //MPYH
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(2);
  }

  }

  return data;
}

auto PPU::writeIO(uint24 address, uint8 data) -> void {
  cpu.synchronize(ppu);

  switch((uint16)address) {

  case 0x211b: {  //M7A
    io.m7a = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211c: {  //M7B
    io.m7b = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211d: {  //M7C
    io.m7c = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211e: {  //M7D
    io.m7d = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x211f: {  //M7X
    io.m7x = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  case 0x2120: {  //M7Y
    io.m7y = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  }
}
