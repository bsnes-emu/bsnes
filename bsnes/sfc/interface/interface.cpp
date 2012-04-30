#include <sfc/sfc.hpp>

namespace SuperFamicom {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == 0) {
    stream.read(smp.iplrom, min(64u, stream.size()));
  }

  if(id == 1) {
    cartridge.rom.copy(stream);
    cartridge.load(Cartridge::Mode::Normal, markup);
    system.power();
    input.connect(0, Input::Device::Joypad);
    input.connect(1, Input::Device::Joypad);
  }
}

void Interface::unload() {
  cartridge.unload();
}

void Interface::power() {
  system.power();
}

void Interface::reset() {
  system.reset();
}

void Interface::run() {
  system.run();
}

void Interface::updatePalette() {
  video.generate_palette();
}

Interface::Interface() {
  interface = this;

  information.name       = "Super Famicom";
  information.width      = 256;
  information.height     = 240;
  information.frequency  = 32040;
  information.ports      = 2;
  information.resettable = true;

  {
    Firmware firmware;
    firmware.displayname = "Super Famicom";
    firmware.name        = "Super Famicom.sys/spc700.rom";
    firmware.id          = 0;
    this->firmware.append(firmware);
  }

  {
    Media media;
    media.displayname = "Super Famicom";
    media.name        = "program.rom";
    media.filter      = "*.sfc";
    media.id          = 1;
    this->media.append(media);
  }

  {
    Port port;
    port.name = "Port 1";
    port.id   = 0;
    {
      Port::Device device;
      device.name = "None";
      device.id   = 0;
      port.device.append(device);
    }
    {
      Port::Device device;
      device.name = "Controller";
      device.id   = 1;
      device.input.append({"B",      0,  0});
      device.input.append({"Y",      0,  1});
      device.input.append({"Select", 0,  2});
      device.input.append({"Start",  0,  3});
      device.input.append({"Up",     0,  4});
      device.input.append({"Down",   0,  5});
      device.input.append({"Left",   0,  6});
      device.input.append({"Right",  0,  7});
      device.input.append({"A",      0,  8});
      device.input.append({"X",      0,  9});
      device.input.append({"L",      0, 10});
      device.input.append({"R",      0, 11});
      device.displayinput = {4, 5, 6, 7, 0, 8, 1, 9, 10, 11, 2, 3};
      port.device.append(device);
    }
    this->port.append(port);
  }

  {
    Port port;
    port.name = "Port 2";
    port.id   = 1;
    {
      Port::Device device;
      device.name = "None";
      device.id   = 0;
      port.device.append(device);
    }
    {
      Port::Device device;
      device.name = "Controller";
      device.id   = 1;
      device.input.append({"B",      0,  0});
      device.input.append({"Y",      0,  1});
      device.input.append({"Select", 0,  2});
      device.input.append({"Start",  0,  3});
      device.input.append({"Up",     0,  4});
      device.input.append({"Down",   0,  5});
      device.input.append({"Left",   0,  6});
      device.input.append({"Right",  0,  7});
      device.input.append({"A",      0,  8});
      device.input.append({"X",      0,  9});
      device.input.append({"L",      0, 10});
      device.input.append({"R",      0, 11});
      device.displayinput = {4, 5, 6, 7, 0, 8, 1, 9, 10, 11, 2, 3};
      port.device.append(device);
    }
    this->port.append(port);
  }
}

}
