bool InterfaceSNES::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;

  interface->baseName = nall::basename(filename);
  string xml = SNESCartridge(data, size).xmlMemoryMap;
  SNES::cartridge.rom.copy(data, size);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, { xml });
  SNES::system.power();

  delete[] data;
  return true;
}

void InterfaceSNES::unloadCartridge() {
  SNES::cartridge.unload();
  interface->baseName = "";
}

//

void InterfaceSNES::video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan) {
  interface->video_refresh();

  unsigned width = hires ? 512 : 256;
  unsigned height = 0 ? 224 : 239;
  if(interlace) height <<= 1;
  unsigned inpitch = interlace ? 1024 : 2048;

  if(0) {  //NTSC
    if(overscan == false) data +=  9 * 1024;  // 0 + 224 +  0
    if(overscan == true ) data += 16 * 1024;  //-7 + 224 + -7
  }

  if(1) {  //PAL
    if(overscan == false) data +=  1 * 1024;  // 8 + 224 +  7
    if(overscan == true ) data +=  9 * 1024;  // 0 + 239 +  0
  }

  uint32_t *output;
  unsigned outpitch;
  if(video.lock(output, outpitch, width, height)) {
    for(unsigned y = 0; y < height; y++) {
      const uint16_t *sp = data + y * (inpitch >> 1);
      uint32_t *dp = output + y * (outpitch >> 2);
      for(unsigned x = 0; x < width; x++) {
        uint32_t color = *sp++;
        color = ((color & 0x7c00) << 9) | ((color & 0x03e0) << 6) | ((color & 0x001f) << 3);
        *dp++ = color | ((color >> 3) & 0x070707);
      }
    }

    video.unlock();
    video.refresh();
  }
}

void InterfaceSNES::audio_sample(int16_t lsample, int16_t rsample) {
  dspaudio.sample(lsample, rsample);
  while(dspaudio.pending()) {
    signed lsample, rsample;
    dspaudio.read(lsample, rsample);
    audio.sample(lsample, rsample);
  }
}

int16_t InterfaceSNES::input_poll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(port == 0 && device == SNES::Input::Device::Joypad) {
    switch((SNES::Input::JoypadID)id) {
    case SNES::Input::JoypadID::Up:     return interface->inputState[keyboard(0)[Keyboard::Up]];
    case SNES::Input::JoypadID::Down:   return interface->inputState[keyboard(0)[Keyboard::Down]];
    case SNES::Input::JoypadID::Left:   return interface->inputState[keyboard(0)[Keyboard::Left]];
    case SNES::Input::JoypadID::Right:  return interface->inputState[keyboard(0)[Keyboard::Right]];
    case SNES::Input::JoypadID::B:      return interface->inputState[keyboard(0)[Keyboard::Z]];
    case SNES::Input::JoypadID::A:      return interface->inputState[keyboard(0)[Keyboard::X]];
    case SNES::Input::JoypadID::Y:      return interface->inputState[keyboard(0)[Keyboard::A]];
    case SNES::Input::JoypadID::X:      return interface->inputState[keyboard(0)[Keyboard::S]];
    case SNES::Input::JoypadID::L:      return interface->inputState[keyboard(0)[Keyboard::D]];
    case SNES::Input::JoypadID::R:      return interface->inputState[keyboard(0)[Keyboard::C]];
    case SNES::Input::JoypadID::Select: return interface->inputState[keyboard(0)[Keyboard::Apostrophe]];
    case SNES::Input::JoypadID::Start:  return interface->inputState[keyboard(0)[Keyboard::Return]];
    }
  }

  return 0;
}

string InterfaceSNES::path(SNES::Cartridge::Slot slot, const string &hint) {
  return "/home/byuu/Desktop/test";
}
