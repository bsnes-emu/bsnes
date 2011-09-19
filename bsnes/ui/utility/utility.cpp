#include "../base.hpp"
Utility *utility = 0;

void Utility::setMode(Interface::Mode mode) {
  video.clear();
  audio.clear();

  mainWindow->nesMenu.setVisible(false);
  mainWindow->snesMenu.setVisible(false);
  mainWindow->gameBoyMenu.setVisible(false);

  if(mode == Interface::Mode::None) {
    mainWindow->setTitle(application->title);
    mainWindow->setStatusText("No cartridge loaded");
    cheatEditor->reset();
  }

  else if(mode == Interface::Mode::NES) {
    mainWindow->setTitle({ notdir(interface->baseName), " - ", NES::Info::Name, " v", NES::Info::Version });
    mainWindow->nesMenu.setVisible(true);
    dspaudio.setChannels(1);
    dspaudio.setFrequency(315.0 / 88.0 * 6000000.0 / 12.0);
  }

  else if(mode == Interface::Mode::SNES) {
    mainWindow->setTitle({ notdir(interface->baseName), " - ", SNES::Info::Name, " v", SNES::Info::Version });
    mainWindow->snesMenu.setVisible(true);
    dspaudio.setChannels(2);
    dspaudio.setFrequency(32040.0);
  }

  else if(mode == Interface::Mode::GameBoy) {
    mainWindow->setTitle({ notdir(interface->baseName), " - ", GameBoy::Info::Name, " v", GameBoy::Info::Version });
    mainWindow->gameBoyMenu.setVisible(true);
    dspaudio.setChannels(2);
    dspaudio.setFrequency(4194304.0);
  }

  mainWindow->synchronize();
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

  width  = width  * maxM;
  height = height * maxM;

  if(shrink == false) {
    if(geometry.width  < width ) width  = geometry.width;
    if(geometry.height < height) height = geometry.height;

    mainWindow->viewport.setGeometry({
      (geometry.width - width) / 2, (geometry.height - height) / 2,
      width, height
    });
  } else {
    mainWindow->setGeometry({ geometry.x, geometry.y, width, height });
    mainWindow->viewport.setGeometry({ 0, 0, width, height });
  }
}

void Utility::toggleFullScreen() {
  static Geometry geometry;

  if(mainWindow->fullScreen() == false) {
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

  resizeMainWindow();
}

void Utility::bindVideoShader() {
  string data;
  data.readfile(config->video.shader);
  video.set(Video::Shader, (const char*)data);
}

void Utility::updateStatus() {
  time_t currentTime = time(0);
  string text;
  if((currentTime - statusTime) <= 2) {
    text = statusMessage;
  } else if(interface->cartridgeLoaded() == false) {
    text = "No cartridge loaded";
  } else if(application->pause || application->autopause) {
    text = "Paused";
  } else {
    text = statusText;
  }
  if(text != mainWindow->statusText()) {
    mainWindow->setStatusText(text);
  }
}

void Utility::setStatusText(const string &text) {
  statusText = text;
}

void Utility::showMessage(const string &message) {
  statusTime = time(0);
  statusMessage = message;
}

Utility::Utility() {
  statusTime = 0;
}
