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

serializer Interface::serialize() {
  system.runtosave();
  return system.serialize();
}

bool Interface::unserialize(serializer &s) {
  return system.unserialize(s);
}

void Interface::updatePalette() {
  video.generate_palette();
}

Interface::Interface() {
  interface = this;

  information.name        = "Famicom";
  information.width       = 256;
  information.height      = 240;
  information.overscan    = true;
  information.aspectRatio = 8.0 / 7.0;
  information.frequency   = 1789772;
  information.resettable  = true;

  information.media.append({"Famicom", "fc"});

  schema.append(Media{ID::ROM, "Famicom", "fc", "program.rom"});

  {
    Port port{0, "Port 1"};
    port.device.append(controller());
    this->port.append(port);
  }

  {
    Port port{1, "Port 2"};
    port.device.append(controller());
    this->port.append(port);
  }
}

Emulator::Interface::Port::Device Interface::controller() {
  Port::Device device{0, "Controller"};
  device.input.append({0, 0, "A"     });
  device.input.append({1, 0, "B"     });
  device.input.append({2, 0, "Select"});
  device.input.append({3, 0, "Start" });
  device.input.append({4, 0, "Up"    });
  device.input.append({5, 0, "Down"  });
  device.input.append({6, 0, "Left"  });
  device.input.append({7, 0, "Right" });
  device.order = {4, 5, 6, 7, 1, 0, 2, 3};
  return device;
}

}
