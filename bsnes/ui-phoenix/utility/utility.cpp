#include "../base.hpp"
Utility utility;

void Utility::setTitle(const string &text) {
  if(*text) {
    mainWindow.setTitle({ text, " - ", SNES::Info::Name, " v", SNES::Info::Version });
  } else {
    mainWindow.setTitle({ SNES::Info::Name, " v", SNES::Info::Version });
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

void Utility::setStatus(const string &text) {
  static char profile[] = { '[', SNES::Info::Profile[0], ']', ' ', 0 };
  statusText = { profile, text };
}

void Utility::showMessage(const string &text) {
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
    case 5: SNES::input.port_set_device(1, SNES::Input::Device::Justifier); break;
    case 6: SNES::input.port_set_device(1, SNES::Input::Device::Justifiers); break;
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
  Geometry geom = mainWindow.geometry();
  mainWindow.setGeometry(geom.x, geom.y, width, height);
}

void Utility::setShader() {
  string data;
  data.readfile(config.video.shader);
  video.set(Video::Shader, (const char*)data);
}

void Utility::cartridgeLoaded() {
  SNES::cartridge.basename = cartridge.baseName;
  SNES::system.power();
  cheatEditor.load(cartridge.baseName);
  stateManager.load();
  mainWindow.synchronize();
  utility.setTitle(notdir(cartridge.baseName));
  utility.showMessage({
    "Loaded ", notdir(cartridge.baseName),
    cartridge.patchApplied ? ", and applied UPS patch" : ""
  });
}

void Utility::cartridgeUnloaded() {
  SNES::cartridge.unload();
  cheatEditor.save(cartridge.baseName);
  stateManager.save();
  mainWindow.synchronize();
}

void Utility::saveState(unsigned slot) {
  string filename = { cartridge.baseName, "-", slot, ".bst" };
  SNES::system.runtosave();
  serializer s = SNES::system.serialize();
  file fp;
  if(fp.open(filename, file::mode::write)) {
    fp.write(s.data(), s.size());
    fp.close();
    showMessage({ "Saved state ", slot });
  } else {
    showMessage({ "Failed to save state ", slot });
  }
}

void Utility::loadState(unsigned slot) {
  string filename = { cartridge.baseName, "-", slot, ".bst" };
  file fp;
  if(fp.open(filename, file::mode::read)) {
    unsigned size = fp.size();
    uint8_t *data = new uint8_t[size];
    fp.read(data, size);
    fp.close();
    serializer s(data, size);
    delete[] data;
    if(SNES::system.unserialize(s) == true) {
      showMessage({ "Loaded state ", slot });
    } else {
      showMessage({ "Failed to load state ", slot });
    }
  }
}

Utility::Utility() {
  statusTime = 0;
}
