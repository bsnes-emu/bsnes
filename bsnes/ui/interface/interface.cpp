#include "../base.hpp"
#include "palette.cpp"
#include "nes.cpp"
#include "snes.cpp"
#include "gameboy.cpp"
Interface *interface = 0;

Filter filter;

void Filter::render(const uint16_t *input, unsigned inputPitch, unsigned inputWidth, unsigned inputHeight) {
  width = inputWidth, height = inputHeight;
  dl_size(width, height);
  dl_render(data, pitch, input, inputPitch, inputWidth, inputHeight);
}

Filter::Filter() {
  data = new uint16_t[1024 * 1024];
  pitch = 1024 * sizeof(uint16_t);
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
  dspaudio.setVolume((double)config->audio.volume / 100.0);
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
  if(filename.endswith(".gb" )) result = gameBoy.loadCartridge(filename);
  if(filename.endswith(".gbc")) result = gameBoy.loadCartridge(filename);
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
  switch(mode()) {
  case Mode::NES:     nes.power(); break;
  case Mode::SNES:    snes.power(); break;
  case Mode::GameBoy: gameBoy.power(); break;
  }
  utility->showMessage("System power was cycled");
}

void Interface::reset() {
  switch(mode()) {
  case Mode::NES:     nes.reset(); break;
  case Mode::SNES:    snes.reset(); break;
  case Mode::GameBoy: gameBoy.power(); break;  //Game Boy lacks reset button
  }
  utility->showMessage("System was reset");
}

void Interface::run() {
  switch(mode()) {
  case Mode::NES:     return nes.run();
  case Mode::SNES:    return snes.run();
  case Mode::GameBoy: return gameBoy.run();
  }
}

serializer Interface::serialize() {
  switch(mode()) {
  case Mode::SNES:    return snes.serialize();
  case Mode::GameBoy: return gameBoy.serialize();
  }
  return serializer();
}

bool Interface::unserialize(serializer &s) {
  switch(mode()) {
  case Mode::SNES:    return snes.unserialize(s);
  case Mode::GameBoy: return gameBoy.unserialize(s);
  }
  return false;
}

bool Interface::saveState(unsigned slot) {
  string filename = { baseName, "-", slot, ".bst" };
  bool result = false;
  switch(mode()) {
  case Mode::SNES:    result = snes.saveState(filename); break;
  case Mode::GameBoy: result = gameBoy.saveState(filename); break;
  }
  utility->showMessage(result == true ? string{ "Saved state ", slot } : "Failed to save state");
  return result;
}

bool Interface::loadState(unsigned slot) {
  string filename = { baseName, "-", slot, ".bst" };
  bool result = false;
  switch(mode()) {
  case Mode::SNES:    result = snes.loadState(filename); break;
  case Mode::GameBoy: result = gameBoy.loadState(filename); break;
  }
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

Interface::Interface() {
  mode = Mode::None;
  palette.update();
  nes.initialize(&nes);
  snes.initialize(&snes);
  gameBoy.initialize(&gameBoy);
}

//internal

//RGB555 input
void Interface::videoRefresh(const uint16_t *input, unsigned inputPitch, unsigned width, unsigned height) {
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
    inputPitch >>= 1, outputPitch >>= 2;

    for(unsigned y = 0; y < height; y++) {
      const uint16_t *sp = input + y * inputPitch;
      uint32_t *dp = output + y * outputPitch;
      for(unsigned x = 0; x < width; x++) {
        *dp++ = palette[*sp++];
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
