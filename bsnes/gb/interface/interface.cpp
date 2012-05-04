#include <gb/gb.hpp>

namespace GameBoy {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == ID::GameBoyBootROM) {
    stream.read(system.bootROM.dmg, min( 256u, stream.size()));
  }

  if(id == ID::SuperGameBoyBootROM) {
    stream.read(system.bootROM.sgb, min( 256u, stream.size()));
  }

  if(id == ID::GameBoyColorBootROM) {
    stream.read(system.bootROM.cgb, min(2048u, stream.size()));
  }

  if(id == ID::GameBoyROM) {
    cartridge.load(System::Revision::GameBoy, markup, stream);
    system.power();
  }

  if(id == ID::GameBoyColorROM) {
    cartridge.load(System::Revision::GameBoyColor, markup, stream);
    system.power();
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ramdata, min(stream.size(), cartridge.ramsize));
  }
}

void Interface::save(unsigned id, const stream &stream) {
  if(id == ID::RAM) {
    stream.write(cartridge.ramdata, cartridge.ramsize);
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

  information.name        = "Game Boy";
  information.width       = 160;
  information.height      = 144;
  information.overscan    = false;
  information.aspectRatio = 1.0;
  information.frequency   = 4194304;
  information.resettable  = false;

  information.media.append({"Game Boy",       "gb" });
  information.media.append({"Game Boy Color", "gbc"});

  firmware.append({ID::GameBoyBootROM,      "Game Boy",       "sys", "boot.rom"});
  firmware.append({ID::SuperGameBoyBootROM, "Super Game Boy", "sfc", "boot.rom"});
  firmware.append({ID::GameBoyColorBootROM, "Game Boy Color", "sys", "boot.rom"});

  schema.append(Media{ID::GameBoyROM,      "Game Boy",       "gb",  "program.rom"});
  schema.append(Media{ID::GameBoyColorROM, "Game Boy Color", "gbc", "program.rom"});

  {
    Port port{0, "Device"};
    {
      Port::Device device{0, "Controller"};
      device.input.append({0, 0, "Up"    });
      device.input.append({1, 0, "Down"  });
      device.input.append({2, 0, "Left"  });
      device.input.append({3, 0, "Right" });
      device.input.append({4, 0, "B"     });
      device.input.append({5, 0, "A"     });
      device.input.append({6, 0, "Select"});
      device.input.append({7, 0, "Start" });
      device.order = {0, 1, 2, 3, 4, 5, 6, 7};
      port.device.append(device);
    }
    this->port.append(port);
  }
}

}
