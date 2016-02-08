#include <fc/fc.hpp>

namespace Famicom {

Interface* interface = nullptr;
Settings settings;

Interface::Interface() {
  interface = this;

  information.manufacturer = "Nintendo";
  information.name         = "Famicom";
  information.width        = 256;
  information.height       = 240;
  information.overscan     = true;
  information.aspectRatio  = 8.0 / 7.0;
  information.resettable   = true;

  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::Famicom, "Famicom", "fc", true});

  { Device device{0, ID::Port1 | ID::Port2, "Controller"};
    device.input.append({0, 0, "A"     });
    device.input.append({1, 0, "B"     });
    device.input.append({2, 0, "Select"});
    device.input.append({3, 0, "Start" });
    device.input.append({4, 0, "Up"    });
    device.input.append({5, 0, "Down"  });
    device.input.append({6, 0, "Left"  });
    device.input.append({7, 0, "Right" });
    device.order = {4, 5, 6, 7, 1, 0, 2, 3};
    this->device.append(device);
  }

  port.append({0, "Port 1"});
  port.append({1, "Port 2"});

  for(auto& device : this->device) {
    for(auto& port : this->port) {
      if(device.portmask & (1 << port.id)) {
        port.device.append(device);
      }
    }
  }
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::videoFrequency() -> double {
  return 21477272.0 / (262.0 * 1364.0 - 4.0);
}

auto Interface::audioFrequency() -> double {
  return 21477272.0 / 12.0;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::sha256() -> string {
  return cartridge.sha256();
}

auto Interface::group(uint id) -> uint {
  switch(id) {
  case ID::SystemManifest:
    return 0;
  case ID::Manifest:
  case ID::ProgramROM:
  case ID::ProgramRAM:
  case ID::CharacterROM:
  case ID::CharacterRAM:
    return 1;
  }

  throw;
}

auto Interface::load(uint id) -> void {
  system.load();
}

auto Interface::save() -> void {
  for(auto& memory : cartridge.memory) {
    saveRequest(memory.id, memory.name);
  }
}

auto Interface::load(uint id, const stream& stream) -> void {
  if(id == ID::SystemManifest) {
    system.information.manifest = stream.text();
  }

  if(id == ID::Manifest) {
    cartridge.information.markup = stream.text();
  }

  if(id == ID::ProgramROM) {
    stream.read(cartridge.board->prgrom.data, min(cartridge.board->prgrom.size, stream.size()));
  }

  if(id == ID::ProgramRAM) {
    stream.read(cartridge.board->prgram.data, min(cartridge.board->prgram.size, stream.size()));
  }

  if(id == ID::CharacterROM) {
    stream.read(cartridge.board->chrrom.data, min(cartridge.board->chrrom.size, stream.size()));
  }

  if(id == ID::CharacterRAM) {
    stream.read(cartridge.board->chrram.data, min(cartridge.board->chrram.size, stream.size()));
  }
}

auto Interface::save(uint id, const stream& stream) -> void {
  if(id == ID::ProgramRAM) {
    stream.write(cartridge.board->prgram.data, cartridge.board->prgram.size);
  }

  if(id == ID::CharacterRAM) {
    stream.write(cartridge.board->chrram.data, cartridge.board->chrram.size);
  }
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::power() -> void {
  system.power();
}

auto Interface::reset() -> void {
  system.reset();
}

auto Interface::run() -> void {
  system.run();
}

auto Interface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto Interface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto Interface::cheatSet(const lstring& list) -> void {
  cheat.reset();
  for(auto& codeset : list) {
    lstring codes = codeset.split("+");
    for(auto& code : codes) {
      lstring part = code.split("/");
      if(part.size() == 2) cheat.append(hex(part[0]), hex(part[1]));
      if(part.size() == 3) cheat.append(hex(part[0]), hex(part[1]), hex(part[2]));
    }
  }
}

auto Interface::cap(const string& name) -> bool {
  if(name == "Color Emulation") return true;
  if(name == "Scanline Emulation") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Color Emulation") return settings.colorEmulation;
  if(name == "Scanline Emulation") return settings.scanlineEmulation;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  if(name == "Color Emulation" && value.is<bool>()) return settings.colorEmulation = value.get<bool>(), true;
  if(name == "Scanline Emulation" && value.is<bool>()) return settings.scanlineEmulation = value.get<bool>(), true;
  return false;
}

}
