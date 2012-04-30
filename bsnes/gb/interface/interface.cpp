#include <gb/gb.hpp>

namespace GameBoy {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == 0) stream.read(system.bootROM.dmg, min( 256u, stream.size()));
  if(id == 1) stream.read(system.bootROM.sgb, min( 256u, stream.size()));
  if(id == 2) stream.read(system.bootROM.cgb, min(2048u, stream.size()));
  if(id == 3) {
    cartridge.load(System::Revision::GameBoy, markup, stream);
    system.power();
  }
  if(id == 4) {
    cartridge.load(System::Revision::SuperGameBoy, markup, stream);
    system.power();
  }
  if(id == 5) {
    cartridge.load(System::Revision::GameBoyColor, markup, stream);
    system.power();
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

  information.name       = "Game Boy";
  information.width      = 160;
  information.height     = 144;
  information.frequency  = 4194304;
  information.ports      = 1;
  information.resettable = false;

  {
    Firmware firmware;
    firmware.displayname = "Game Boy";
    firmware.name        = "Game Boy.sys/boot.rom";
    firmware.id          = 0;
    this->firmware.append(firmware);
  }

  {
    Firmware firmware;
    firmware.displayname = "Super Game Boy";
    firmware.name        = "Super Game Boy.sfc/boot.rom";
    firmware.id          = 1;
    this->firmware.append(firmware);
  }

  {
    Firmware firmware;
    firmware.displayname = "Game Boy Color";
    firmware.name        = "Game Boy Color.sys/boot.rom";
    firmware.id          = 2;
    this->firmware.append(firmware);
  }

  {
    Media media;
    media.displayname = "Game Boy";
    media.name        = "program.rom";
    media.filter      = "*.gb";
    media.id          = 3;
    this->media.append(media);
  }

  {
    Media media;
    media.displayname = "Game Boy Color";
    media.name        = "program.rom";
    media.filter      = "*.gbc";
    media.id          = 5;
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
      device.input.append({"Up",     0, 0});
      device.input.append({"Down",   0, 1});
      device.input.append({"Left",   0, 2});
      device.input.append({"Right",  0, 3});
      device.input.append({"B",      0, 4});
      device.input.append({"A",      0, 5});
      device.input.append({"Select", 0, 6});
      device.input.append({"Start",  0, 7});
      device.displayinput = {0, 1, 2, 3, 4, 5, 6, 7};
      port.device.append(device);
    }
    this->port.append(port);
  }
}

}
