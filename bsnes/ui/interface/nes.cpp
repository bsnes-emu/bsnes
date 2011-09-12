bool InterfaceNES::loadCartridge(const string &filename) {
  filemap fp;
  if(fp.open(filename, filemap::mode::read) == false) return false;

  interface->baseName = nall::basename(filename);
  NES::cartridge.load("", fp.data(), fp.size());
  NES::system.power();

  fp.close();
  return true;
}

void InterfaceNES::unloadCartridge() {
  NES::cartridge.unload();
  interface->baseName = "";
}

//

void InterfaceNES::video_refresh(const uint32_t *data) {
  interface->video_refresh();

  uint32_t *output;
  unsigned outpitch;
  if(video.lock(output, outpitch, 256, 240)) {
    for(unsigned y = 0; y < 240; y++) {
      const uint32_t *sp = data + y * 256;
      uint32_t *dp = output + y * (outpitch >> 2);
      for(unsigned x = 0; x < 256; x++) {
        *dp++ = *sp++;
      }
    }

    video.unlock();
    video.refresh();
  }
}

void InterfaceNES::audio_sample(int16_t sample) {
  dspaudio.sample(sample, sample);
  while(dspaudio.pending()) {
    signed lsample, rsample;
    dspaudio.read(lsample, rsample);
    audio.sample(lsample, rsample);
  }
}

int16_t InterfaceNES::input_poll(bool port, unsigned device, unsigned id) {
  if(port == 0 && device == 0) {
    switch(id) {
    case 0: return interface->inputState[keyboard(0)[Keyboard::X]];
    case 1: return interface->inputState[keyboard(0)[Keyboard::Z]];
    case 2: return interface->inputState[keyboard(0)[Keyboard::Apostrophe]];
    case 3: return interface->inputState[keyboard(0)[Keyboard::Return]];
    case 4: return interface->inputState[keyboard(0)[Keyboard::Up]];
    case 5: return interface->inputState[keyboard(0)[Keyboard::Down]];
    case 6: return interface->inputState[keyboard(0)[Keyboard::Left]];
    case 7: return interface->inputState[keyboard(0)[Keyboard::Right]];
    }
  }

  return 0;
}
