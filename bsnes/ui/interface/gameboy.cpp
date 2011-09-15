bool InterfaceGameBoy::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;

  interface->baseName = nall::basename(filename);
  GameBoyCartridge info(data, size);
  GameBoy::cartridge.load(info.xml, data, size);
  GameBoy::system.power();

  delete[] data;
  return true;
}

void InterfaceGameBoy::unloadCartridge() {
  GameBoy::cartridge.unload();
  interface->baseName = "";
}

bool InterfaceGameBoy::saveState(const string &filename) {
  GameBoy::system.runtosave();
  serializer s = GameBoy::system.serialize();
  return file::write(filename, s.data(), s.size());
}

bool InterfaceGameBoy::loadState(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;
  serializer s(data, size);
  delete[] data;
  return GameBoy::system.unserialize(s);
}

void InterfaceGameBoy::setCheatCodes(const lstring &list) {
  GameBoy::cheat.reset();
  for(unsigned n = 0; n < list.size(); n++) {
    GameBoy::cheat[n] = list[n];
    GameBoy::cheat[n].enable = true;
  }
  GameBoy::cheat.synchronize();
}

//

void InterfaceGameBoy::video_refresh(const uint8_t *data) {
  interface->video_refresh();

  uint32_t *output;
  unsigned outpitch;
  if(video.lock(output, outpitch, 160, 144)) {
    for(unsigned y = 0; y < 144; y++) {
      const uint8_t *sp = data + y * 160;
      uint32_t *dp = output + y * (outpitch >> 2);
      for(unsigned x = 0; x < 160; x++) {
        uint32_t color = *sp++;
        *dp++ = (color << 16) | (color << 8) | (color << 0);
      }
    }

    video.unlock();
    video.refresh();
  }
}

void InterfaceGameBoy::audio_sample(int16_t csample, int16_t lsample, int16_t rsample) {
  dspaudio.sample(lsample, rsample);
  while(dspaudio.pending()) {
    signed lsample, rsample;
    dspaudio.read(lsample, rsample);
    audio.sample(lsample, rsample);
  }
}

bool InterfaceGameBoy::input_poll(unsigned id) {
  switch((GameBoy::Input)id) {
  case GameBoy::Input::Up:     return interface->inputState[keyboard(0)[Keyboard::Up]];
  case GameBoy::Input::Down:   return interface->inputState[keyboard(0)[Keyboard::Down]];
  case GameBoy::Input::Left:   return interface->inputState[keyboard(0)[Keyboard::Left]];
  case GameBoy::Input::Right:  return interface->inputState[keyboard(0)[Keyboard::Right]];
  case GameBoy::Input::B:      return interface->inputState[keyboard(0)[Keyboard::Z]];
  case GameBoy::Input::A:      return interface->inputState[keyboard(0)[Keyboard::X]];
  case GameBoy::Input::Select: return interface->inputState[keyboard(0)[Keyboard::Apostrophe]];
  case GameBoy::Input::Start:  return interface->inputState[keyboard(0)[Keyboard::Return]];
  }

  return false;
}
