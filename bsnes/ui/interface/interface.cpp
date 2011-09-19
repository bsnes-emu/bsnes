#include "../base.hpp"
#include "nes.cpp"
#include "snes.cpp"
#include "gameboy.cpp"
Interface *interface = 0;

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
  cheatEditor->save({ baseName, ".cht" });
  stateManager->save({ baseName, ".bsa" }, 0u);
  setCheatCodes();

  switch(mode()) {
  case Mode::NES:     nes.unloadCartridge(); break;
  case Mode::SNES:    snes.unloadCartridge(); break;
  case Mode::GameBoy: gameBoy.unloadCartridge(); break;
  }

  utility->setMode(mode = Mode::None);
}

void Interface::power() {
  switch(mode()) {
  case Mode::NES:     return nes.power();
  case Mode::SNES:    return snes.power();
  case Mode::GameBoy: return gameBoy.power();
  }
}

void Interface::reset() {
  switch(mode()) {
  case Mode::NES:     return nes.reset();
  case Mode::SNES:    return snes.reset();
  case Mode::GameBoy: return gameBoy.power();  //Game Boy lacks reset button
  }
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

bool Interface::saveState(const string &filename) {
  bool result = false;
  switch(mode()) {
  case Mode::SNES:    result = snes.saveState(filename); break;
  case Mode::GameBoy: result = gameBoy.saveState(filename); break;
  }
  utility->showMessage(result == true ? "Saved state" : "Failed to save state");
  return result;
}

bool Interface::loadState(const string &filename) {
  bool result = false;
  switch(mode()) {
  case Mode::SNES:    result = snes.loadState(filename); break;
  case Mode::GameBoy: result = gameBoy.loadState(filename); break;
  }
  utility->showMessage(result == true ? "Loaded state" : "Failed to load state");
  return result;
}

void Interface::setCheatCodes(const lstring &list) {
  switch(mode()) {
  case Mode::NES:     return nes.setCheats(list);
  case Mode::SNES:    return snes.setCheats(list);
  case Mode::GameBoy: return gameBoy.setCheats(list);
  }
}

Interface::Interface() {
  mode = Mode::None;
  nes.initialize(&nes);
  snes.initialize(&snes);
  gameBoy.initialize(&gameBoy);
}

//internal

void Interface::videoRefresh() {
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
