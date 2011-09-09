#include "../base.hpp"
Utility *utility = 0;

void Utility::setMode(Interface::Mode mode) {
  video.clear();
  audio.clear();

  mainWindow->cartridgeMenu.setVisible(false);
  mainWindow->nesMenu.setVisible(false);
  mainWindow->snesMenu.setVisible(false);
  mainWindow->gameBoyMenu.setVisible(false);

  if(mode == Interface::Mode::None) {
    mainWindow->setTitle(application->title);
    mainWindow->cartridgeMenu.setVisible(true);
    mainWindow->setStatusText("No cartridge loaded");
  }

  else if(mode == Interface::Mode::NES) {
    mainWindow->setTitle({ notdir(interface->baseName), " - ", NES::Info::Name, " v", NES::Info::Version });
    mainWindow->nesMenu.setVisible(true);
    dspaudio.setFrequency(315.0 / 88.8 * 6000000.0 / 12.0);
  }

  else if(mode == Interface::Mode::SNES) {
    mainWindow->setTitle({ notdir(interface->baseName), " - ", SNES::Info::Name, " v", SNES::Info::Version });
    mainWindow->snesMenu.setVisible(true);
    dspaudio.setFrequency(32040.0);
  }

  else if(mode == Interface::Mode::GameBoy) {
    mainWindow->setTitle({ notdir(interface->baseName), " - ", GameBoy::Info::Name, " v", GameBoy::Info::Version });
    mainWindow->gameBoyMenu.setVisible(true);
    dspaudio.setFrequency(4194304.0);
  }

  resizeMainWindow();
}

void Utility::resizeMainWindow(bool shrink) {
  Geometry geometry = mainWindow->geometry();
  unsigned width = geometry.width, height = geometry.height;

  switch(interface->mode()) {
  case Interface::Mode::NES:     width = 256, height = 240; break;
  case Interface::Mode::SNES:    width = 256, height = 239; break;
  case Interface::Mode::GameBoy: width = 160, height = 144; break;
  }

  unsigned maxW = geometry.width / width;
  unsigned maxH = geometry.height / height;
  unsigned maxM = max(1u, min(maxW, maxH));

  width = width * maxM;
  height = height * maxM;

  if(shrink == false) {
    mainWindow->viewport.setGeometry({
      (geometry.width - width) / 2, (geometry.height - height) / 2,
      width, height
    });
  } else {
    mainWindow->setGeometry({ geometry.x, geometry.y, width, height });
    mainWindow->viewport.setGeometry({ 0, 0, width, height });
  }
}

void Utility::shrinkMainWindow() {
}

void Utility::toggleFullScreen() {
  static bool fullScreen = false;
  static Geometry geometry;

  if(fullScreen == false) {
    geometry = mainWindow->geometry();
    mainWindow->setMenuVisible(false);
    mainWindow->setStatusVisible(false);
    mainWindow->setFullScreen(true);
    input.acquire();
  } else {
    input.unacquire();
    mainWindow->setMenuVisible(true);
    mainWindow->setStatusVisible(true);
    mainWindow->setFullScreen(false);
    mainWindow->setGeometry(geometry);
  }

  fullScreen ^= 1;
  resizeMainWindow();
}
