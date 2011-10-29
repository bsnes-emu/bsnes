#include "../base.hpp"
#include "palette.cpp"
#include "nes/nes.cpp"
#include "snes/snes.cpp"
#include "gameboy/gameboy.cpp"
Interface *interface = nullptr;

Filter filter;

void Filter::render(const uint32_t *input, unsigned inputPitch, unsigned inputWidth, unsigned inputHeight) {
  width = inputWidth, height = inputHeight;
  dl_size(width, height);
  dl_render(data, pitch, input, inputPitch, inputWidth, inputHeight);
}

Filter::Filter() {
  data = new uint32_t[2048 * 2048];
  pitch = 2048 * sizeof(uint32_t);
}

Filter::~Filter() {
  delete[] data;
}

void Interface::bindControllers() {
  switch(mode()) {
  case Mode::NES:
    nes.setController(0, config->nes.controllerPort1Device);
    nes.setController(1, config->nes.controllerPort2Device);
    break;

  case Mode::SNES:
    snes.setController(0, config->snes.controllerPort1Device);
    snes.setController(1, config->snes.controllerPort2Device);
    break;
  }
}

void Interface::setController(unsigned port, unsigned device) {
  switch(mode()) {
  case Mode::NES: return nes.setController(port, device);
  case Mode::SNES: return snes.setController(port, device);
  }
}

void Interface::updateDSP() {
  dspaudio.setVolume(config->audio.mute == false ? (double)config->audio.volume / 100.0 : 0.0);
  switch(mode()) {
  case Mode::NES:     return dspaudio.setFrequency(config->audio.frequencyNES);
  case Mode::SNES:    return dspaudio.setFrequency(config->audio.frequencySNES);
  case Mode::GameBoy: return dspaudio.setFrequency(config->audio.frequencyGameBoy);
  }
}

bool Interface::cartridgeLoaded() {
  switch(mode()) {
  case Mode::NES:     return nes.cartridgeLoaded();
  case Mode::SNES:    return snes.cartridgeLoaded();
  case Mode::GameBoy: return gameBoy.cartridgeLoaded();
  }
  return false;
}

void Interface::loadCartridge(Mode mode) {
  utility->setMode(this->mode = mode);
  switch(mode) {
  case Mode::NES: core = &nes; break;
  case Mode::SNES: core = &snes; break;
  case Mode::GameBoy: core = &gameBoy; break;
  default: core = nullptr; break;
  }

  bindControllers();
  cheatEditor->load({ baseName, ".cht" });
  stateManager->load({ baseName, ".bsa" }, 0u);
  dipSwitches->load();
  utility->showMessage({ "Loaded ", notdir(baseName) });
}

bool Interface::loadCartridge(const string &filename) {
  bool result = false;
  if(filename.endswith(".nes")) result = nes.loadCartridge(filename);
  if(filename.endswith(".sfc")) result = snes.loadCartridge(filename);
  if(filename.endswith(".gb" )) result = gameBoy.loadCartridge(GameBoy::System::Revision::GameBoy, filename);
  if(filename.endswith(".gbc")) result = gameBoy.loadCartridge(GameBoy::System::Revision::GameBoyColor, filename);
  return result;
}

void Interface::unloadCartridge() {
  if(cartridgeLoaded() == false) return;
  cheatDatabase->setVisible(false);
  cheatEditor->save({ baseName, ".cht" });
  stateManager->save({ baseName, ".bsa" }, 0u);
  setCheatCodes();

  switch(mode()) {
  case Mode::NES:     nes.unloadCartridge(); break;
  case Mode::SNES:    snes.unloadCartridge(); break;
  case Mode::GameBoy: gameBoy.unloadCartridge(); break;
  }

  interface->baseName = "";
  interface->slotName.reset();
  utility->setMode(mode = Mode::None);
}

void Interface::power() {
  if(core == nullptr) return;
  core->power();
  utility->showMessage("System power was cycled");
}

void Interface::reset() {
  if(core == nullptr) return;
  core->reset();
  utility->showMessage("System was reset");
}

void Interface::run() {
  if(core == nullptr) return;
  core->run();
}

serializer Interface::serialize() {
  if(core == nullptr) return serializer();
  return core->serialize();
}

bool Interface::unserialize(serializer &s) {
  if(core == nullptr) return false;
  return core->unserialize(s);
}

bool Interface::saveState(unsigned slot) {
  string filename = { baseName, "-", slot, ".bst" };
  serializer s = serialize();
  bool result = file::write(filename, s.data(), s.size());
  utility->showMessage(result == true ? string{ "Saved state ", slot } : "Failed to save state");
  return result;
}

bool Interface::loadState(unsigned slot) {
  string filename = { baseName, "-", slot, ".bst" };
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) {
    utility->showMessage(string{ "Slot ", slot, " save file not found" });
    return false;
  }
  serializer s(data, size);
  bool result = unserialize(s);
  utility->showMessage(result == true ? string{ "Loaded state ", slot } : "Failed to load state");
  return result;
}

void Interface::setCheatCodes(const lstring &list) {
  switch(mode()) {
  case Mode::NES:     return nes.setCheats(list);
  case Mode::SNES:    return snes.setCheats(list);
  case Mode::GameBoy: return gameBoy.setCheats(list);
  }
}

string Interface::sha256() {
  switch(mode()) {
  case Mode::NES:     return NES::cartridge.sha256();
  case Mode::SNES:    return SNES::cartridge.sha256();
  case Mode::GameBoy: return GameBoy::cartridge.sha256();
  }
  return "{None}";
}

Interface::Interface() : core(nullptr) {
  mode = Mode::None;
  palette.update();
  nes.initialize();
  snes.initialize();
  gameBoy.initialize();
}

//internal

bool Interface::loadFile(const string &filename, uint8_t *&data, unsigned &size) {
  if(file::read(filename, data, size) == false) return false;

  string patchname = { nall::basename(filename), ".bps" };
  if(file::exists(patchname) == false) return true;

  bpspatch bps;
  bps.modify(patchname);
  bps.source(data, size);
  unsigned targetSize = bps.size();
  uint8_t *targetData = new uint8_t[targetSize];
  bps.target(targetData, targetSize);
  if(bps.apply() != bpspatch::result::success) {
    delete[] targetData;
    return true;
  }

  delete[] data;
  data = targetData;
  size = targetSize;
  return true;
}

void Interface::videoRefresh(const uint32_t *input, unsigned inputPitch, unsigned width, unsigned height) {
  uint32_t *output;
  unsigned outputPitch;

  if(filter.opened()) {
    filter.render(input, inputPitch, width, height);
    input = filter.data;
    inputPitch = filter.pitch;
    width = filter.width;
    height = filter.height;
  }

  if(video.lock(output, outputPitch, width, height)) {
    inputPitch >>= 2, outputPitch >>= 2;

    for(unsigned y = 0; y < height; y++) {
      const uint32_t *sp = input + y * inputPitch;
      uint32_t *dp = output + y * outputPitch;
      for(unsigned x = 0; x < width; x++) {
        uint32_t color = *sp++;
        *dp++ = (palette[color >> 16] << 16) + (palette[color >> 8] << 8) + (palette[color >> 0] << 0);
      }
    }

    video.unlock();
    video.refresh();
  }

  static unsigned frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    previous = current;
    utility->setStatusText({ "FPS: ", frameCounter });
    frameCounter = 0;
  }
}
