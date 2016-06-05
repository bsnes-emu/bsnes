auto ICD2::lcdScanline() -> void {
  if(GameBoy::ppu.status.ly > 143) return;  //Vblank
  if((GameBoy::ppu.status.ly & 7) == 0) {
    write_bank = (write_bank + 1) & 3;
    write_addr = 0;
  }
}

auto ICD2::lcdOutput(uint2 color) -> void {
  uint y = write_addr / 160;
  uint x = write_addr % 160;
  uint addr = write_bank * 512 + y * 2 + x / 8 * 16;
  output[addr + 0] = (output[addr + 0] << 1) | (bool)(color & 1);
  output[addr + 1] = (output[addr + 1] << 1) | (bool)(color & 2);
  write_addr = (write_addr + 1) % 1280;
}

auto ICD2::joypWrite(bool p15, bool p14) -> void {
  //joypad handling
  if(p15 == 1 && p14 == 1) {
    if(joyp15lock == 0 && joyp14lock == 0) {
      joyp15lock = 1;
      joyp14lock = 1;
      joyp_id = (joyp_id + 1) & 3;
    }
  }

  if(p15 == 0 && p14 == 1) joyp15lock = 0;
  if(p15 == 1 && p14 == 0) joyp14lock = 0;

  //packet handling
  if(p15 == 0 && p14 == 0) {  //pulse
    pulselock = false;
    packetoffset = 0;
    bitoffset = 0;
    strobelock = true;
    packetlock = false;
    return;
  }

  if(pulselock) return;

  if(p15 == 1 && p14 == 1) {
    strobelock = false;
    return;
  }

  if(strobelock) {
    if(p15 == 1 || p14 == 1) {  //malformed packet
      packetlock = false;
      pulselock = true;
      bitoffset = 0;
      packetoffset = 0;
    } else {
      return;
    }
  }

  //p15:1, p14:0 = 0
  //p15:0, p14:1 = 1
  bool bit = (p15 == 0);
  strobelock = true;

  if(packetlock) {
    if(p15 == 1 && p14 == 0) {
      if((joyp_packet[0] >> 3) == 0x11) {
        mlt_req = joyp_packet[1] & 3;
        if(mlt_req == 2) mlt_req = 3;
        joyp_id = 0;
      }

      if(packetsize < 64) packet[packetsize++] = joyp_packet;
      packetlock = false;
      pulselock = true;
    }
    return;
  }

  bitdata = (bit << 7) | (bitdata >> 1);
  if(++bitoffset < 8) return;

  bitoffset = 0;
  joyp_packet[packetoffset] = bitdata;
  if(++packetoffset < 16) return;
  packetlock = true;
}

auto ICD2::loadRequest(uint id, string name, string type, bool required) -> void {
}

auto ICD2::loadRequest(uint id, string name, bool required) -> void {
  if(id == GameBoy::ID::SystemManifest) {
    interface->loadRequest(ID::SuperGameBoyManifest, name, required);
  }

  if(id == GameBoy::ID::SuperGameBoyBootROM) {
    interface->loadRequest(ID::SuperGameBoyBootROM, name, required);
  }

  if(id == GameBoy::ID::Manifest) {
    interface->loadRequest(ID::GameBoyManifest, name, required);
  }

  if(id == GameBoy::ID::ROM) {
    interface->loadRequest(ID::GameBoyROM, name, required);
  }

  if(id == GameBoy::ID::RAM) {
    interface->loadRequest(ID::GameBoyRAM, name, required);
  }
}

auto ICD2::saveRequest(uint id, string name) -> void {
  if(id == GameBoy::ID::RAM) {
    interface->saveRequest(ID::GameBoyRAM, name);
  }
}

auto ICD2::videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void {
}

auto ICD2::audioSample(const double* samples, uint channels) -> void {
  stream->write(samples);
}

auto ICD2::inputPoll(uint port, uint device, uint id) -> int16 {
  GameBoy::cpu.status.mlt_req = joyp_id & mlt_req;

  uint data = 0x00;
  switch(joyp_id & mlt_req) {
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
