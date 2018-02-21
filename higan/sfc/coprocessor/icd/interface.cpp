auto ICD::lcdScanline() -> void {
  if(GameBoy::ppu.status.ly > 143) return;  //Vblank
  if((GameBoy::ppu.status.ly & 7) == 0) {
    writeBank = (writeBank + 1) & 3;
    writeAddress = 0;
  }
}

auto ICD::lcdOutput(uint2 color) -> void {
  uint y = writeAddress / 160;
  uint x = writeAddress % 160;
  uint addr = writeBank * 512 + y * 2 + x / 8 * 16;
  output[addr + 0] = (output[addr + 0] << 1) | (bool)(color & 1);
  output[addr + 1] = (output[addr + 1] << 1) | (bool)(color & 2);
  writeAddress = (writeAddress + 1) % 1280;
}

auto ICD::joypWrite(bool p15, bool p14) -> void {
  //joypad handling
  if(p15 == 1 && p14 == 1) {
    if(joyp15Lock == 0 && joyp14Lock == 0) {
      joyp15Lock = 1;
      joyp14Lock = 1;
      joypID = (joypID + 1) & 3;
    }
  }

  if(p15 == 0 && p14 == 1) joyp15Lock = 0;
  if(p15 == 1 && p14 == 0) joyp14Lock = 0;

  //packet handling
  if(p15 == 0 && p14 == 0) {  //pulse
    pulseLock = false;
    packetOffset = 0;
    bitOffset = 0;
    strobeLock = true;
    packetLock = false;
    return;
  }

  if(pulseLock) return;

  if(p15 == 1 && p14 == 1) {
    strobeLock = false;
    return;
  }

  if(strobeLock) {
    if(p15 == 1 || p14 == 1) {  //malformed packet
      packetLock = false;
      pulseLock = true;
      bitOffset = 0;
      packetOffset = 0;
    } else {
      return;
    }
  }

  //p15:1, p14:0 = 0
  //p15:0, p14:1 = 1
  bool bit = (p15 == 0);
  strobeLock = true;

  if(packetLock) {
    if(p15 == 1 && p14 == 0) {
      if((joypPacket[0] >> 3) == 0x11) {
        mltReq = joypPacket[1] & 3;
        if(mltReq == 2) mltReq = 3;
        joypID = 0;
      }

      if(packetSize < 64) packet[packetSize++] = joypPacket;
      packetLock = false;
      pulseLock = true;
    }
    return;
  }

  bitData = (bit << 7) | (bitData >> 1);
  if(++bitOffset < 8) return;

  bitOffset = 0;
  joypPacket[packetOffset] = bitData;
  if(++packetOffset < 16) return;
  packetLock = true;
}
