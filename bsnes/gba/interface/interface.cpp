#include <gba/gba.hpp>

namespace GameBoyAdvance {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &memory, const string &markup) {
  if(id == 0) cartridge.load(markup, memory);
  if(id == 1) memory.read(bios.data, min(bios.size, memory.size()));
}

void Interface::unload() {
  cartridge.unload();
}

void Interface::power() {
  system.power();
}

void Interface::reset() {
  system.power();
}

void Interface::run() {
  system.run();
}

void Interface::updatePalette() {
  video.generate_palette();
}

Interface::Interface() {
  interface = this;

  information.name      = "Game Boy Advance";
  information.width     = 240;
  information.height    = 160;
  information.frequency = 32768;
  information.ports     = 1;

  {
  Media medium;
  medium.name   = "Game Boy Advance";
  medium.filter = "*.sfc";
  medium.id     = 0;
  media.append(medium);
  }

  {
  Controller controller;
  controller.name   = "Controller";
  controller.port   = 0;
  controller.device = 0;
  controller.inputs.append({"Up",     6});
  controller.inputs.append({"Down",   7});
  controller.inputs.append({"Left",   5});
  controller.inputs.append({"Right",  4});
  controller.inputs.append({"B",      1});
  controller.inputs.append({"A",      0});
  controller.inputs.append({"L",      9});
  controller.inputs.append({"R",      8});
  controller.inputs.append({"Select", 2});
  controller.inputs.append({"Start",  3});
  controllers.append(controller);
  }
}

}
