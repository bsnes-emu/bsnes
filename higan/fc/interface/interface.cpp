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

  { Device device{0, ID::ControllerPort1 | ID::ControllerPort2, "None"};
    devices.append(device);
  }

  { Device device{1, ID::ControllerPort1 | ID::ControllerPort2, "Gamepad"};
    device.inputs.append({0, 0, "Up"    });
    device.inputs.append({1, 0, "Down"  });
    device.inputs.append({2, 0, "Left"  });
    device.inputs.append({3, 0, "Right" });
    device.inputs.append({4, 0, "B"     });
    device.inputs.append({5, 0, "A"     });
    device.inputs.append({6, 0, "Select"});
    device.inputs.append({7, 0, "Start" });
    devices.append(device);
  }

  ports.append({0, "Port 1"});
  ports.append({1, "Port 2"});

  for(auto& device : devices) {
    for(auto& port : ports) {
      if(device.portmask & (1 << port.id)) {
        port.devices.append(device);
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

auto Interface::videoColors() -> uint32 {
  return 1 << 9;
}

auto Interface::videoColor(uint32 n) -> uint64 {
  double saturation = 2.0;
  double hue = 0.0;
  double contrast = 1.0;
  double brightness = 1.0;
  double gamma = settings.colorEmulation ? 1.8 : 2.2;

  int color = (n & 0x0f), level = color < 0xe ? (n >> 4) & 3 : 1;

  static const double black = 0.518, white = 1.962, attenuation = 0.746;
  static const double levels[8] = {
    0.350, 0.518, 0.962, 1.550,
    1.094, 1.506, 1.962, 1.962,
  };

  double lo_and_hi[2] = {
    levels[level + 4 * (color == 0x0)],
    levels[level + 4 * (color <  0xd)],
  };

  double y = 0.0, i = 0.0, q = 0.0;
  auto wave = [](int p, int color) { return (color + p + 8) % 12 < 6; };
  for(int p : range(12)) {
    double spot = lo_and_hi[wave(p, color)];

    if(((n & 0x040) && wave(p, 12))
    || ((n & 0x080) && wave(p,  4))
    || ((n & 0x100) && wave(p,  8))
    ) spot *= attenuation;

    double v = (spot - black) / (white - black);

    v = (v - 0.5) * contrast + 0.5;
    v *= brightness / 12.0;

    y += v;
    i += v * cos((Math::Pi / 6.0) * (p + hue));
    q += v * sin((Math::Pi / 6.0) * (p + hue));
  }

  i *= saturation;
  q *= saturation;

  auto gammaAdjust = [=](double f) { return f < 0.0 ? 0.0 : pow(f, 2.2 / gamma); };
  uint64 r = uclamp<16>(65535.0 * gammaAdjust(y +  0.946882 * i +  0.623557 * q));
  uint64 g = uclamp<16>(65535.0 * gammaAdjust(y + -0.274788 * i + -0.635691 * q));
  uint64 b = uclamp<16>(65535.0 * gammaAdjust(y + -1.108545 * i +  1.709007 * q));

  return r << 32 | g << 16 | b << 0;
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

auto Interface::connect(uint port, uint device) -> void {
  Famicom::peripherals.connect(port, device);
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
  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    system.configureVideoPalette();
    return true;
  }
  if(name == "Scanline Emulation" && value.is<bool>()) return settings.scanlineEmulation = value.get<bool>(), true;
  return false;
}

}
