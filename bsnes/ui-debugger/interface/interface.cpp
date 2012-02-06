#include "../base.hpp"
Interface *interface = nullptr;

bool Interface::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;

  if(SNES::cartridge.loaded()) {
    saveMemory();
    SNES::cartridge.unload();
    consoleWindow->print("Cartridge unloaded\n");
  }

  fileName = filename;
  baseName = nall::basename(fileName);

  string markup;
  markup.readfile({ baseName, ".xml" });
  if(markup.empty()) markup = SnesCartridge(data, size).markup;

  SNES::cartridge.rom.copy(data, size);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, markup);
  SNES::system.power();

  delete[] data;
  videoWindow->setTitle(notdir(baseName));
  SNES::video.generate(SNES::Video::Format::RGB24);
  consoleWindow->print({"Loaded ", fileName, "\n", markup, "\n"});
  loadMemory();
  return true;
}

void Interface::loadMemory() {
  for(auto &memory : SNES::cartridge.nvram) {
    if(memory.size == 0) continue;
    string filename = { baseName, memory.id };
    uint8_t *data;
    unsigned size;
    if(file::read(filename, data, size)) {
      consoleWindow->print({"Loaded ", filename, "\n"});
      memcpy(memory.data, data, min(memory.size, size));
      delete[] data;
    }
  }
}

void Interface::saveMemory() {
  for(auto &memory : SNES::cartridge.nvram) {
    if(memory.size == 0) continue;
    string filename = { baseName, memory.id };
    if(file::write(filename, memory.data, memory.size)) {
      consoleWindow->print({"Saved ", filename, "\n"});
    }
  }
}

//hires is always true for accuracy core
//overscan is ignored for the debugger port
void Interface::videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan) {
  uint32_t *output = videoWindow->canvas.data();

  if(interlace == false) {
    for(unsigned y = 0; y < 240; y++) {
      const uint32_t *sp = data + y * 1024;
      uint32_t *dp0 = output + y * 1024, *dp1 = dp0 + 512;
      for(unsigned x = 0; x < 512; x++) {
        *dp0++ = SNES::video.palette[*sp];
        *dp1++ = SNES::video.palette[*sp++];
      }
    }
  } else {
    for(unsigned y = 0; y < 480; y++) {
      const uint32_t *sp = data + y * 512;
      uint32_t *dp = output + y * 512;
      for(unsigned x = 0; x < 512; x++) {
        *dp++ = SNES::video.palette[*sp++];
      }
    }
  }

  videoWindow->canvas.update();
}

void Interface::audioSample(int16_t lsample, int16_t rsample) {
  if(consoleWindow->menuEmulationMuteAudio.checked()) lsample = rsample = 0;
  audio.sample(lsample, rsample);
}

int16_t Interface::inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(videoWindow->focused() == false) return 0;
  auto keyboardState = phoenix::Keyboard::state();

  if(port == 0) {
    if(device == SNES::Input::Device::Joypad) {
      switch((SNES::Input::JoypadID)id) {
      case SNES::Input::JoypadID::Up:     return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Up];
      case SNES::Input::JoypadID::Down:   return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Down];
      case SNES::Input::JoypadID::Left:   return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Left];
      case SNES::Input::JoypadID::Right:  return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Right];
      case SNES::Input::JoypadID::B:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::B];
      case SNES::Input::JoypadID::A:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::A];
      case SNES::Input::JoypadID::Y:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Y];
      case SNES::Input::JoypadID::X:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::X];
      case SNES::Input::JoypadID::L:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::L];
      case SNES::Input::JoypadID::R:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::R];
      case SNES::Input::JoypadID::Select: return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Apostrophe];
      case SNES::Input::JoypadID::Start:  return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Return];
      }
    }
  }

  return 0;
}

string Interface::path(SNES::Cartridge::Slot slot, const string &hint) {
  return { baseName, hint };
}

void Interface::message(const string &text) {
  consoleWindow->print({text, "\n"});
}

Interface::Interface() {
  SNES::interface = this;
  SNES::system.init();
}
