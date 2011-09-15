#include "../base.hpp"
#include "nes.cpp"
#include "snes.cpp"
#include "gameboy.cpp"
Interface *interface = 0;

bool Interface::loaded() {
  switch(mode()) {
  case Mode::NES:     return nes.cartridgeLoaded();
  case Mode::SNES:    return snes.cartridgeLoaded();
  case Mode::GameBoy: return gameBoy.cartridgeLoaded();
  }
  return false;
}

bool Interface::loadCartridge(const string &filename) {
  bool result = false;
  setCheatCodes();
  unloadCartridge();
  if(filename.endswith(".nes")) result = loadCartridgeNES(filename);
  if(filename.endswith(".sfc")) result = loadCartridgeSNES(filename);
  if(filename.endswith(".gb" )) result = loadCartridgeGameBoy(filename);
  if(filename.endswith(".gbc")) result = loadCartridgeGameBoy(filename);
  if(result == true) {
    cheatEditor->load({ baseName, ".cht" });
    stateManager->load({ baseName, ".bsa" }, 0u);
  }
  return result;
}

bool Interface::loadCartridgeNES(const string &filename) {
  if(nes.loadCartridge(filename) == false) return false;
  utility->setMode(mode = Mode::NES);
  return true;
}

void Interface::unloadCartridge() {
  if(loaded() == false) return;
  cheatEditor->save({ baseName, ".cht" });
  stateManager->save({ baseName, ".bsa" }, 0u);

  switch(mode()) {
  case Mode::NES:     nes.unloadCartridge(); break;
  case Mode::SNES:    snes.unloadCartridge(); break;
  case Mode::GameBoy: gameBoy.unloadCartridge(); break;
  }

  utility->setMode(mode = Mode::None);
}

void Interface::unloadCartridgeNES() {
  nes.unloadCartridge();
  utility->setMode(mode = Mode::None);
}

bool Interface::loadCartridgeSNES(const string &filename) {
  if(snes.loadCartridge(filename) == false) return false;
  utility->setMode(mode = Mode::SNES);
  return true;
}

void Interface::unloadCartridgeSNES() {
  snes.unloadCartridge();
  utility->setMode(mode = Mode::None);
}

bool Interface::loadCartridgeGameBoy(const string &filename) {
  if(gameBoy.loadCartridge(filename) == false) return false;
  utility->setMode(mode = Mode::GameBoy);
  return true;
}

void Interface::unloadCartridgeGameBoy() {
  gameBoy.unloadCartridge();
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
  switch(mode()) {
  case Mode::SNES:    return snes.saveState(filename);
  case Mode::GameBoy: return gameBoy.saveState(filename);
  }
  return false;
}

bool Interface::loadState(const string &filename) {
  switch(mode()) {
  case Mode::SNES:    return snes.loadState(filename);
  case Mode::GameBoy: return gameBoy.loadState(filename);
  }
  return false;
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

void Interface::input_poll() {
  bool fullScreen = inputState[keyboard(0)[Keyboard::F11]];

  input.poll(inputState);

  if(!fullScreen && inputState[keyboard(0)[Keyboard::F11]]) {
    utility->toggleFullScreen();
  }
}

void Interface::video_refresh() {
  static unsigned frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    previous = current;
    mainWindow->setStatusText({ "FPS: ", frameCounter });
    frameCounter = 0;
  }
}
