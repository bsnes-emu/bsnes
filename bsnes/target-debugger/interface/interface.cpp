#include "../base.hpp"
Interface *interface = nullptr;

bool Interface::loadCartridge(const string &foldername) {
  auto memory = file::read({foldername, "program.rom"});
  if(memory.empty()) return false;

  if(SFC::cartridge.loaded()) {
    saveMemory();
    SFC::cartridge.unload();
    debugger->print("Cartridge unloaded\n");
  }

  pathName = foldername;
  mkdir(string(pathName, "debug/"), 0755);

  string markup;
  markup.readfile({pathName, "manifest.xml"});
  if(markup.empty()) markup = SuperFamicomCartridge(memory.data(), memory.size()).markup;

  SFC::cartridge.rom.copy(vectorstream{memory});
  SFC::cartridge.load(SFC::Cartridge::Mode::Normal, markup);
  SFC::system.power();

  string name = pathName;
  name.rtrim<1>("/");
  name = notdir(name);

  videoWindow->setTitle(name);
  SFC::video.generate_palette();
  debugger->print("Loaded ", pathName, "program.rom\n");
  loadMemory();
  debugger->print(markup, "\n");
  debugger->suspend();
  return true;
}

void Interface::loadMemory() {
  for(auto &memory : SFC::cartridge.nvram) {
    if(memory.size == 0) continue;
    string filename = {pathName, memory.id};
    if(auto content = file::read(filename)) {
      debugger->print("Loaded ", filename, "\n");
      memcpy(memory.data, content.data(), min(memory.size, content.size()));
    }
  }

  debugger->loadUsage();
}

void Interface::saveMemory() {
  for(auto &memory : SFC::cartridge.nvram) {
    if(memory.size == 0) continue;
    string filename = { pathName, memory.id };
    if(file::write(filename, memory.data, memory.size)) {
      debugger->print("Saved ", filename, "\n");
    }
  }

  debugger->saveUsage();
}

bool Interface::loadState(unsigned slot) {
  string filename = {pathName, "state-", slot, ".bst"};
  auto memory = file::read(filename);
  if(memory.empty()) return false;
  serializer s(memory.data(), memory.size());
  bool result = SFC::system.unserialize(s);
  if(result) debugger->print("Loaded state from ", filename, "\n");
  return result;
}

bool Interface::saveState(unsigned slot) {
  SFC::system.runtosave();
  serializer s = SFC::system.serialize();
  string filename = {pathName, "state-", slot, ".bst"};
  bool result = file::write(filename, s.data(), s.size());
  if(result) debugger->print("Saved state to ", filename, "\n");
  return result;
}

//hires is always true for accuracy core
//overscan is ignored for the debugger port
void Interface::videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan) {
  data += 8 * 1024;  //skip NTSC overscan compensation
  uint32_t *output = videoWindow->canvas.data();

  if(interlace == false) {
    for(unsigned y = 0; y < 240; y++) {
      const uint32_t *sp = data + y * 1024;
      uint32_t *dp0 = output + y * 1024, *dp1 = dp0 + 512;
      for(unsigned x = 0; x < 512; x++) {
        *dp0++ = SFC::video.palette[*sp];
        *dp1++ = SFC::video.palette[*sp++];
      }
    }
  } else {
    for(unsigned y = 0; y < 480; y++) {
      const uint32_t *sp = data + y * 512;
      uint32_t *dp = output + y * 512;
      for(unsigned x = 0; x < 512; x++) {
        *dp++ = SFC::video.palette[*sp++];
      }
    }
  }

  videoWindow->canvas.update();
}

void Interface::audioSample(int16_t lsample, int16_t rsample) {
  if(consoleWindow->menuEmulationMuteAudio.checked()) lsample = rsample = 0;
  audio.sample(lsample, rsample);
}

int16_t Interface::inputPoll(bool port, SFC::Input::Device device, unsigned index, unsigned id) {
  if(videoWindow->focused() == false) return 0;
  auto keyboardState = phoenix::Keyboard::state();

  if(port == 0) {
    if(device == SFC::Input::Device::Joypad) {
      switch((SFC::Input::JoypadID)id) {
      case SFC::Input::JoypadID::Up:     return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Up];
      case SFC::Input::JoypadID::Down:   return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Down];
      case SFC::Input::JoypadID::Left:   return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Left];
      case SFC::Input::JoypadID::Right:  return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Right];
      case SFC::Input::JoypadID::B:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Z];
      case SFC::Input::JoypadID::A:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::X];
      case SFC::Input::JoypadID::Y:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::A];
      case SFC::Input::JoypadID::X:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::S];
      case SFC::Input::JoypadID::L:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::D];
      case SFC::Input::JoypadID::R:      return keyboardState[(unsigned)phoenix::Keyboard::Scancode::C];
      case SFC::Input::JoypadID::Select: return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Apostrophe];
      case SFC::Input::JoypadID::Start:  return keyboardState[(unsigned)phoenix::Keyboard::Scancode::Return];
      }
    }
  }

  return 0;
}

string Interface::path(SFC::Cartridge::Slot slot, const string &hint) {
  return {pathName, hint};
}

void Interface::message(const string &text) {
  debugger->print(text, "\n");
}

Interface::Interface() {
  SFC::interface = this;
  SFC::system.init();

  filestream fs{{application->userpath, "Super Famicom.sys/spc700.rom"}};
  fs.read(SFC::smp.iplrom, min(64u, fs.size()));
}
