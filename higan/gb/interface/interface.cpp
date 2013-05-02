#include <gb/gb.hpp>

namespace GameBoy {

Interface* interface = nullptr;

void Interface::lcdScanline() {
  if(hook) hook->lcdScanline();
}

void Interface::joypWrite(bool p15, bool p14) {
  if(hook) hook->joypWrite(p15, p14);
}

string Interface::title() {
  return cartridge.title();
}

double Interface::videoFrequency() {
  return 4194304.0 / (154.0 * 456.0);
}

double Interface::audioFrequency() {
  return 4194304.0;
}

bool Interface::loaded() {
  return cartridge.loaded();
}

string Interface::sha256() {
  return cartridge.sha256();
}

unsigned Interface::group(unsigned id) {
  switch(id) {
  case ID::GameBoyBootROM:
  case ID::SuperGameBoyBootROM:
  case ID::GameBoyColorBootROM:
    return 0;
  case ID::Manifest:
  case ID::ROM:
  case ID::RAM:
    if(system.revision() == System::Revision::GameBoy) return ID::GameBoy;
    if(system.revision() == System::Revision::SuperGameBoy) return ID::SuperGameBoy;
    if(system.revision() == System::Revision::GameBoyColor) return ID::GameBoyColor;
    throw;
  }

  throw;
}

void Interface::load(unsigned id) {
  if(id == ID::GameBoy) cartridge.load(System::Revision::GameBoy);
  if(id == ID::SuperGameBoy) cartridge.load(System::Revision::SuperGameBoy);
  if(id == ID::GameBoyColor) cartridge.load(System::Revision::GameBoyColor);
}

void Interface::save() {
  for(auto &memory : cartridge.memory) {
    interface->saveRequest(memory.id, memory.name);
  }
}

void Interface::load(unsigned id, const stream& stream) {
  if(id == ID::GameBoyBootROM) {
    stream.read(system.bootROM.dmg, min( 256u, stream.size()));
  }

  if(id == ID::SuperGameBoyBootROM) {
    stream.read(system.bootROM.sgb, min( 256u, stream.size()));
  }

  if(id == ID::GameBoyColorBootROM) {
    stream.read(system.bootROM.cgb, min(2048u, stream.size()));
  }

  if(id == ID::Manifest) cartridge.information.markup = stream.text();

  if(id == ID::ROM) {
    stream.read(cartridge.romdata, min(cartridge.romsize, stream.size()));
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ramdata, min(stream.size(), cartridge.ramsize));
  }
}

void Interface::save(unsigned id, const stream& stream) {
  if(id == ID::RAM) {
    stream.write(cartridge.ramdata, cartridge.ramsize);
  }
}

void Interface::unload() {
  save();
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

bool Interface::unserialize(serializer& s) {
  return system.unserialize(s);
}

void Interface::cheatSet(const lstring& list) {
  cheat.reset();
  for(auto& code : list) {
    lstring codelist = code.split("+");
    for(auto& part : codelist) {
      unsigned addr, data, comp;
      if(Cheat::decode(part, addr, data, comp)) cheat.append({addr, data, comp});
    }
  }
  cheat.synchronize();
}

void Interface::paletteUpdate() {
  video.generate_palette();
}

Interface::Interface() {
  interface = this;
  hook = nullptr;

  information.name        = "Game Boy";
  information.width       = 160;
  information.height      = 144;
  information.overscan    = false;
  information.aspectRatio = 1.0;
  information.resettable  = false;
  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::GameBoy,      "Game Boy",       "gb" , true});
  media.append({ID::GameBoyColor, "Game Boy Color", "gbc", true});

  {
    Device device{0, ID::Device, "Controller"};
    device.input.append({0, 0, "Up"    });
    device.input.append({1, 0, "Down"  });
    device.input.append({2, 0, "Left"  });
    device.input.append({3, 0, "Right" });
    device.input.append({4, 0, "B"     });
    device.input.append({5, 0, "A"     });
    device.input.append({6, 0, "Select"});
    device.input.append({7, 0, "Start" });
    device.order = {0, 1, 2, 3, 4, 5, 6, 7};
    this->device.append(device);
  }

  port.append({0, "Device", {device[0]}});
}

}
