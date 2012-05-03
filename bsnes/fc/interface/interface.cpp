#include <fc/fc.hpp>

namespace Famicom {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == ID::ROM) {
    cartridge.load(markup, stream);
    system.power();
    input.connect(0, Input::Device::Joypad);
    input.connect(1, Input::Device::Joypad);
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ram_data(), min(stream.size(), cartridge.ram_size()));
  }
}

void Interface::save(unsigned id, const stream &stream) {
  if(id == ID::RAM) {
    stream.write(cartridge.ram_data(), cartridge.ram_size());
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

  information.name        = "Famicom";
  information.width       = 256;
  information.height      = 240;
  information.aspectRatio = 8.0 / 7.0;
  information.frequency   = 1789772;
  information.resettable  = true;

  information.media.append({"Famicom", "*.fc"});

  {
    Schema schema;
    schema.displayname = "Famicom";
    schema.name        = "program.rom";
    schema.filter      = "*.fc";
    schema.id          = ID::ROM;
    this->schema.append(schema);
  }

  {
    Port port;
    port.name = "Port 1";
    port.id   = 0;
    {
      Port::Device device;
      device.name = "Controller";
      device.id   = 0;
      device.input.append({"A",      0, 0});
      device.input.append({"B",      0, 1});
      device.input.append({"Select", 0, 2});
      device.input.append({"Start",  0, 3});
      device.input.append({"Up",     0, 4});
      device.input.append({"Down",   0, 5});
      device.input.append({"Left",   0, 6});
      device.input.append({"Right",  0, 7});
      device.displayinput = {4, 5, 6, 7, 1, 0, 2, 3};
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
      device.name = "Controller";
      device.id   = 0;
      device.input.append({"A",      0, 0});
      device.input.append({"B",      0, 1});
      device.input.append({"Select", 0, 2});
      device.input.append({"Start",  0, 3});
      device.input.append({"Up",     0, 4});
      device.input.append({"Down",   0, 5});
      device.input.append({"Left",   0, 6});
      device.input.append({"Right",  0, 7});
      device.displayinput = {4, 5, 6, 7, 1, 0, 2, 3};
      port.device.append(device);
    }
    this->port.append(port);
  }
}

}
