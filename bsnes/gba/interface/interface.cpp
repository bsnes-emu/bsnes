#include <gba/gba.hpp>

namespace GameBoyAdvance {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == 0) {
    memory.reset();
    cartridge.load(markup, stream);
    system.power();
  }
  if(id == 1) {
    stream.read(bios.data, min(bios.size, stream.size()));
  }
  if(id == 2) {
    stream.read(cartridge.ram.data, min(cartridge.ram.size, stream.size()));
  }
  if(id == 3) {
    stream.read(cartridge.eeprom.data, min(cartridge.eeprom.size, stream.size()));
  }
  if(id == 4) {
    stream.read(cartridge.flashrom.data, min(cartridge.flashrom.size, stream.size()));
  }
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

  information.name       = "Game Boy Advance";
  information.width      = 240;
  information.height     = 160;
  information.frequency  = 32768;
  information.ports      = 1;
  information.resettable = false;

  {
    Firmware firmware;
    firmware.displayname = "Game Boy Advance";
    firmware.name        = "Game Boy Advance.sys/bios.rom";
    firmware.id          = 1;
    this->firmware.append(firmware);
  }

  {
    Media media;
    media.displayname = "Game Boy Advance";
    media.name        = "program.rom";
    media.filter      = "*.gba";
    media.id          = 0;
    this->media.append(media);
  }

  {
    Port port;
    port.name = "Device";
    port.id   = 0;
    {
      Port::Device device;
      device.name = "Controller";
      device.id   = 0;
      device.input.append({"A",      0, 0});
      device.input.append({"B",      0, 1});
      device.input.append({"Select", 0, 2});
      device.input.append({"Start",  0, 3});
      device.input.append({"Right",  0, 4});
      device.input.append({"Left",   0, 5});
      device.input.append({"Up",     0, 6});
      device.input.append({"Down",   0, 7});
      device.input.append({"R",      0, 8});
      device.input.append({"L",      0, 9});
      device.displayinput = {6, 7, 5, 4, 1, 0, 9, 8, 2, 3};
      port.device.append(device);
    }
    this->port.append(port);
  }
}

}
