auto ICD2::lcdScanline() -> void {
  if(GameBoy::ppu.status.ly > 143) return;  //Vblank
  if((GameBoy::ppu.status.ly & 7) == 0) {
    writeBank = (writeBank + 1) & 3;
    writeAddress = 0;
  }
}

auto ICD2::lcdOutput(uint2 color) -> void {
  uint y = writeAddress / 160;
  uint x = writeAddress % 160;
  uint addr = writeBank * 512 + y * 2 + x / 8 * 16;
  output[addr + 0] = (output[addr + 0] << 1) | (bool)(color & 1);
  output[addr + 1] = (output[addr + 1] << 1) | (bool)(color & 2);
  writeAddress = (writeAddress + 1) % 1280;
}

auto ICD2::joypWrite(bool p15, bool p14) -> void {
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

auto ICD2::open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file {
  //redirect system folder to cartridge folder:
  //expects "GameBoy.sys"; but this would be "Super Famicom.sys"; redirect to "Super Game Boy.sfc/"
  if(id == ID::System) id = cartridge.pathID();
  return interface->open(id, name, mode, required);
}

auto ICD2::load(uint id, string name, string type) -> maybe<uint> {
  return interface->load(id, name, type);
}

auto ICD2::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
}

auto ICD2::audioSample(const double* samples, uint channels) -> void {
  stream->write(samples);
}

auto ICD2::inputPoll(uint port, uint device, uint id) -> int16 {
  GameBoy::cpu.status.mltReq = joypID & mltReq;

  uint data = 0x00;
  switch(joypID & mltReq) {
  case 0: data = ~r6004; break;
  case 1: data = ~r6005; break;
  case 2: data = ~r6006; break;
  case 3: data = ~r6007; break;
  }

  switch((GameBoy::Input)id) {
  case GameBoy::Input::Start:  return (bool)(data & 0x80);
  case GameBoy::Input::Select: return (bool)(data & 0x40);
  case GameBoy::Input::B:      return (bool)(data & 0x20);
  case GameBoy::Input::A:      return (bool)(data & 0x10);
  case GameBoy::Input::Down:   return (bool)(data & 0x08);
  case GameBoy::Input::Up:     return (bool)(data & 0x04);
  case GameBoy::Input::Left:   return (bool)(data & 0x02);
  case GameBoy::Input::Right:  return (bool)(data & 0x01);
  }

  return 0;
}
