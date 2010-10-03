#include "../base.hpp"
Utility utility;

void Utility::setTitle(const char *text) {
  if(*text) {
    mainWindow.setTitle(string(text, " - ", SNES::Info::Name, " v", SNES::Info::Version));
  } else {
    mainWindow.setTitle(string(SNES::Info::Name, " v", SNES::Info::Version));
  }
}

void Utility::updateStatus() {
  time_t currentTime = time(0);
  string text;
  if((currentTime - statusTime) <= 3) {
    text = statusMessage;
  } else if(SNES::cartridge.loaded() == false) {
    text = "No cartridge loaded";
  } else {
    text = statusText;
  }
  if(text != statusCurrentText) {
    mainWindow.setStatusText(statusCurrentText = text);
  }
}

void Utility::setStatus(const char *text) {
  static char profile[] = { '[', SNES::Info::Profile[0], ']', ' ', 0 };
  statusText = string(profile, text);
}

void Utility::showMessage(const char *text) {
  statusMessage = text;
  statusTime = time(0);
}

void Utility::setControllers() {
  switch(config.controller.port1) {
    case 0: SNES::input.port_set_device(0, SNES::Input::Device::None); break;
    case 1: SNES::input.port_set_device(0, SNES::Input::Device::Joypad); break;
    case 2: SNES::input.port_set_device(0, SNES::Input::Device::Multitap); break;
    case 3: SNES::input.port_set_device(0, SNES::Input::Device::Mouse); break;
  }

  switch(config.controller.port2) {
    case 0: SNES::input.port_set_device(1, SNES::Input::Device::None); break;
    case 1: SNES::input.port_set_device(1, SNES::Input::Device::Joypad); break;
    case 2: SNES::input.port_set_device(1, SNES::Input::Device::Multitap); break;
    case 3: SNES::input.port_set_device(1, SNES::Input::Device::Mouse); break;
    case 4: SNES::input.port_set_device(1, SNES::Input::Device::SuperScope); break;
    case 5: SNES::input.port_set_device(1, SNES::Input::Device::Justifiers); break;
  }
}

void Utility::setScale(unsigned scale) {
  if(scale == 0) scale = config.video.scale;
  config.video.scale = scale;
  unsigned width, height;
  if(config.video.region == 0) {
    width = 256 * scale;
    height = 224 * scale;
    if(config.video.aspectRatioCorrection) width *= 54.0 / 47.0;
  } else {
    width = 256 * scale;
    height = 239 * scale;
    if(config.video.aspectRatioCorrection) width *= 32.0 / 23.0;
  }
  mainWindow.viewport.setGeometry(0, 0, width, height);
  mainWindow.setGeometry(128, 128, width, height);
}

void Utility::setShader() {
  string data;
  data.readfile(config.video.shader);
  video.set(Video::Shader, (const char*)data);
}

void Utility::cartridgeLoaded() {
  SNES::system.power();
  cheatEditor.load(cartridge.baseName);
  stateManager.load();
  mainWindow.synchronize();
  utility.setTitle(notdir(cartridge.baseName));
  utility.showMessage(string("Loaded ", notdir(cartridge.baseName)));
}

void Utility::cartridgeUnloaded() {
  SNES::cartridge.unload();
  cheatEditor.save(cartridge.baseName);
  stateManager.save();
  mainWindow.synchronize();
}

void Utility::loadCartridgeNormal() {
  if(config.settings.useNativeDialogs == false) {
    fileBrowser.fileOpen(FileBrowser::Mode::Cartridge, [](string filename) {
      cartridge.loadNormal(filename);
    });
  } else {
    string filename = OS::fileOpen(mainWindow, "SNES cartridges\t*.sfc\nAll files\t*", config.path.current);
    if(filename != "") {
      cartridge.loadNormal(filename);
      SNES::system.power();
    }
  }
}

void Utility::saveState(unsigned slot) {
  string filename = { cartridge.baseName, "-", slot, ".bst" };
  SNES::system.runtosave();
  serializer s = SNES::system.serialize();
  file fp;
  if(fp.open(filename, file::mode_write)) {
    fp.write(s.data(), s.size());
    fp.close();
    showMessage(string("Saved state ", slot));
  } else {
    showMessage(string("Failed to save state ", slot));
  }
}

void Utility::loadState(unsigned slot) {
  string filename = { cartridge.baseName, "-", slot, ".bst" };
  file fp;
  if(fp.open(filename, file::mode_read)) {
    unsigned size = fp.size();
    uint8_t *data = new uint8_t[size];
    fp.read(data, size);
    fp.close();
    serializer s(data, size);
    delete[] data;
    if(SNES::system.unserialize(s) == true) {
      showMessage(string("Loaded state ", slot));
    } else {
      showMessage(string("Failed to load state ", slot));
    }
  }
}

Utility::Utility() {
  statusTime = 0;
}
