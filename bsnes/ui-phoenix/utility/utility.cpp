#include "../base.hpp"
Utility utility;

void Utility::setTitle(const char *text) {
  if(*text) {
    mainWindow.setTitle(string(text, " - ", SNES::Info::Name, " v", SNES::Info::Version));
  } else {
    mainWindow.setTitle(string(SNES::Info::Name, " v", SNES::Info::Version));
  }
}

void Utility::loadCartridgeNormal() {
  string filename = os.fileOpen(mainWindow, "SNES cartridges\t*.sfc\nAll files\t*", "/media/sdb1/root/snes_roms");
  if(filename != "") {
    cartridge.loadNormal(filename);
    SNES::system.power();
  }
}
