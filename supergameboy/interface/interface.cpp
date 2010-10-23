SuperGameBoy supergameboy;

//====================
//SuperGameBoy::Packet
//====================

const char SuperGameBoy::command_name[32][64] = {
  "PAL01",    "PAL23",    "PAL03",    "PAL12",
  "ATTR_BLK", "ATTR_LIN", "ATTR_DIV", "ATTR_CHR",
  "SOUND",    "SOU_TRN",  "PAL_SET",  "PAL_TRN",
  "ATRC_EN",  "TEST_EN",  "ICON_EN",  "DATA_SND",
  "DATA_TRN", "MLT_REQ",  "JUMP",     "CHR_TRN",
  "PCT_TRN",  "ATTR_TRN", "ATTR_SET", "MASK_EN",
  "OBJ_TRN",  "19_???",   "1A_???",   "1B_???",
  "1C_???",   "1D_???",   "1E_ROM",   "1F_???",
};

void SuperGameBoy::joyp_write(bool p15, bool p14) {
  //===============
  //joypad handling
  //===============

  if(p15 == 1 && p14 == 1) {
    if(joyp15lock == 0 && joyp14lock == 0) {
      joyp15lock = 1;
      joyp14lock = 1;
      joyp_id = (joyp_id + 1) & 3;
    }
  }

  if(p15 == 0 && p14 == 1) joyp15lock = 0;
  if(p15 == 1 && p14 == 0) joyp14lock = 0;

  //===============
  //packet handling
  //===============

  if(p15 == 0 && p14 == 0) {
    //pulse
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
    if(p15 == 1 || p14 == 1) {
      //malformed packet
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
        mmio.mlt_req = joyp_packet[1] & 3;
        if(mmio.mlt_req == 2) mmio.mlt_req = 3;
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

//==================
//SuperGameBoy::Core
//==================

static uint8_t null_rom[32768];

bool SuperGameBoy::init(bool version_) {
  if(!romdata) { romdata = null_rom; romsize = 32768; }
  version = version_;

  gambatte = new Gambatte::GB;
  gambatte->setVideoBlitter(this);
  gambatte->setInputStateGetter(this);

  return true;
}

void SuperGameBoy::term() {
  if(gambatte) {
    delete gambatte;
    gambatte = 0;
  }
}

unsigned SuperGameBoy::run(uint32_t *samplebuffer, unsigned samples) {
  if((mmio.r6003 & 0x80) == 0) {
    //Gameboy is inactive
    samplebuffer[0] = 0;
    return 1;
  }

  return gambatte->runFor(samplebuffer, samples);
}

void SuperGameBoy::save() {
  gambatte->saveSavedata();
}

void SuperGameBoy::serialize(nall::serializer &s) {
  s.integer(vram_row);
  s.array(vram);

  s.integer(mmio.r6000);
  s.integer(mmio.r6003);
  s.integer(mmio.r6004);
  s.integer(mmio.r6005);
  s.integer(mmio.r6006);
  s.integer(mmio.r6007);
  s.array(mmio.r7000);
  s.integer(mmio.r7800);
  s.integer(mmio.mlt_req);

  for(unsigned i = 0; i < 64; i++) s.array(packet[i].data);
  s.integer(packetsize);

  s.integer(joyp_id);
  s.integer(joyp15lock);
  s.integer(joyp14lock);
  s.integer(pulselock);
  s.integer(strobelock);
  s.integer(packetlock);
  s.array(joyp_packet.data);
  s.integer(packetoffset);
  s.integer(bitdata);
  s.integer(bitoffset);

  uint8_t *savestate = new uint8_t[256 * 1024];
  if(s.mode() == serializer::Load) {
    s.array(savestate, 256 * 1024);

    file fp;
    if(fp.open("supergameboy-state.tmp", file::mode::write)) {
      fp.write(savestate, 256 * 1024);
      fp.close();

      gambatte->loadState("supergameboy-state.tmp");
      unlink("supergameboy-state.tmp");
    }
  } else if(s.mode() == serializer::Save) {
    gambatte->saveState("supergameboy-state.tmp");

    file fp;
    if(fp.open("supergameboy-state.tmp", file::mode::read)) {
      fp.read(savestate, fp.size() < 256 * 1024 ? fp.size() : 256 * 1024);
      fp.close();
    }

    unlink("supergameboy-state.tmp");
    s.array(savestate, 256 * 1024);
  } else if(s.mode() == serializer::Size) {
    s.array(savestate, 256 * 1024);
  }
  delete[] savestate;
}

void SuperGameBoy::power() {
  gambatte->load(true);
  mmio_reset();
}

void SuperGameBoy::reset() {
  gambatte->reset();
  mmio_reset();
}

void SuperGameBoy::row(unsigned row) {
  mmio.r7800 = 0;
  vram_row = row;
  render(vram_row);
}

uint8_t SuperGameBoy::read(uint16_t addr) {
  //LY counter
  if(addr == 0x6000) {
    return gambatte->lyCounter();
  }

  //command ready port
  if(addr == 0x6002) {
    bool data = packetsize > 0;
    if(data) {
      for(unsigned i = 0; i < 16; i++) mmio.r7000[i] = packet[0][i];
      packetsize--;
      for(unsigned i = 0; i < packetsize; i++) packet[i] = packet[i + 1];
    }
    return data;
  }

  //command port
  if((addr & 0xfff0) == 0x7000) {
    return mmio.r7000[addr & 15];
  }

  if(addr == 0x7800) {
    uint8_t data = vram[mmio.r7800];
    mmio.r7800 = (mmio.r7800 + 1) % 320;
    return data;
  }

  return 0x00;
}

void SuperGameBoy::write(uint16_t addr, uint8_t data) {
  //control port
  //d7 = /RESET line (0 = stop, 1 = run)
  if(addr == 0x6003) {
    if((mmio.r6003 & 0x80) == 0x00 && (data & 0x80) == 0x80) {
      reset();
      command_1e();
    }

    mmio.r6003 = data;
    return;
  }

  if(addr == 0x6004) { mmio.r6004 = data; return; }  //joypad 1 state
  if(addr == 0x6005) { mmio.r6005 = data; return; }  //joypad 2 state
  if(addr == 0x6006) { mmio.r6006 = data; return; }  //joypad 3 state
  if(addr == 0x6007) { mmio.r6007 = data; return; }  //joypad 4 state
}

void SuperGameBoy::mmio_reset() {
  mmio.r6000 = 0x00;
  mmio.r6003 = 0x00;
  mmio.r6004 = 0xff;
  mmio.r6005 = 0xff;
  mmio.r6006 = 0xff;
  mmio.r6007 = 0xff;
  for(unsigned n = 0; n < 16; n++) mmio.r7000[n] = 0;
  mmio.r7800 = 0;
  mmio.mlt_req = 0;

  packetsize = 0;

  vram_row = 0;
  memset(vram, 0, 320);

  joyp_id = 3;
  joyp15lock = 0;
  joyp14lock = 0;
  pulselock = true;
}

//simulate 256-byte internal SGB BIOS on /RESET
void SuperGameBoy::command_1e() {
  for(unsigned i = 0; i < 6; i++) {
    Packet p;
    p[0] = 0xf1 + (i << 1);
    p[1] = 0;
    for(unsigned n = 2; n < 16; n++) {
      uint8_t data = romdata[0x0104 + (i * 14) + (n - 2)];
      p[1] += data;
      p[n] = data;
    }
    if(packetsize < 64) packet[packetsize++] = p;
  }
}

void SuperGameBoy::render(unsigned row) {
  gambatte->updateVideo();

  uint32_t *source = buffer + row * 160 * 8;
  memset(vram, 0x00, 320);

  for(unsigned y = row * 8; y < row * 8 + 8; y++) {
    for(unsigned x = 0; x < 160; x++) {
      unsigned pixel = *source++ / 0x555555;
      pixel ^= 3;

      unsigned addr = (x / 8 * 16) + ((y & 7) * 2);
      vram[addr + 0] |= ((pixel & 1) >> 0) << (7 - (x & 7));
      vram[addr + 1] |= ((pixel & 2) >> 1) << (7 - (x & 7));
    }
  }
}

//======================
//Gambatte::VideoBlitter
//======================

//should always be 160x144, as no filters are used
void SuperGameBoy::setBufferDimensions(unsigned width, unsigned height) {
  if(buffer) delete[] buffer;
  buffer = new uint32_t[width * height];
  bufferWidth = width;
  bufferHeight = height;
}

const Gambatte::PixelBuffer SuperGameBoy::inBuffer() {
  Gambatte::PixelBuffer pixelBuffer;
  pixelBuffer.pixels = (void*)buffer;
  pixelBuffer.format = Gambatte::PixelBuffer::RGB32;
  pixelBuffer.pitch = bufferWidth;
  return pixelBuffer;
}

void SuperGameBoy::blit() {
}

//==========================
//Gambatte::InputStateGetter
//==========================

const Gambatte::InputState& SuperGameBoy::operator()() {
  inputState.joypadId = 0x0f - (joyp_id & mmio.mlt_req);

  unsigned data = 0x00;
  switch(joyp_id & mmio.mlt_req) {
    case 0: data = mmio.r6004; break;
    case 1: data = mmio.r6005; break;
    case 2: data = mmio.r6006; break;
    case 3: data = mmio.r6007; break;
  }

  inputState.startButton  = !(data & 0x80);
  inputState.selectButton = !(data & 0x40);
  inputState.bButton      = !(data & 0x20);
  inputState.aButton      = !(data & 0x10);
  inputState.dpadDown     = !(data & 0x08);
  inputState.dpadUp       = !(data & 0x04);
  inputState.dpadLeft     = !(data & 0x02);
  inputState.dpadRight    = !(data & 0x01);

  return inputState;
}

//==========================
//SuperGameBoy::Construction
//==========================

SuperGameBoy::SuperGameBoy() : gambatte(0), buffer(0) {
  romdata = ramdata = rtcdata = 0;
  romsize = ramsize = rtcsize = 0;
}

SuperGameBoy::~SuperGameBoy() {
  if(buffer) delete[] buffer;
}
