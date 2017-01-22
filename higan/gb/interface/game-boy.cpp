GameBoyInterface::GameBoyInterface() {
  information.manufacturer = "Nintendo";
  information.name         = "Game Boy";
  information.overscan     = false;

  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::GameBoy, "Game Boy", "gb"});

  Port hardwarePort{ID::Port::Hardware, "Hardware"};

  { Device device{ID::Device::Controls, "Controls"};
    device.inputs.append({0, "Up"    });
    device.inputs.append({0, "Down"  });
    device.inputs.append({0, "Left"  });
    device.inputs.append({0, "Right" });
    device.inputs.append({0, "B"     });
    device.inputs.append({0, "A"     });
    device.inputs.append({0, "Select"});
    device.inputs.append({0, "Start" });
    hardwarePort.devices.append(device);
  }

  ports.append(move(hardwarePort));
}

auto GameBoyInterface::manifest() -> string {
  return cartridge.manifest();
}

auto GameBoyInterface::title() -> string {
  return cartridge.title();
}

auto GameBoyInterface::videoSize() -> VideoSize {
  return {160, 144};
}

auto GameBoyInterface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  uint w = 160;
  uint h = 144;
  uint m = min(width / w, height / h);
  return {w * m, h * m};
}

auto GameBoyInterface::videoFrequency() -> double {
  return 4194304.0 / (154.0 * 456.0);
}

auto GameBoyInterface::videoColors() -> uint32 {
  return 1 << 2;
}

auto GameBoyInterface::videoColor(uint32 color) -> uint64 {
  if(!settings.colorEmulation) {
    uint64 L = image::normalize(3 - color, 2, 16);
    return L << 32 | L << 16 | L << 0;
  } else {
    #define DMG_PALETTE_GREEN
  //#define DMG_PALETTE_YELLOW
  //#define DMG_PALETTE_WHITE

    const uint16 monochrome[4][3] = {
      #if defined(DMG_PALETTE_GREEN)
      {0xaeae, 0xd9d9, 0x2727},
      {0x5858, 0xa0a0, 0x2828},
      {0x2020, 0x6262, 0x2929},
      {0x1a1a, 0x4545, 0x2a2a},
      #elif defined(DMG_PALETTE_YELLOW)
      {0xffff, 0xf7f7, 0x7b7b},
      {0xb5b5, 0xaeae, 0x4a4a},
      {0x6b6b, 0x6969, 0x3131},
      {0x2121, 0x2020, 0x1010},
      #elif defined(DMG_PALETTE_WHITE)
      {0xffff, 0xffff, 0xffff},
      {0xaaaa, 0xaaaa, 0xaaaa},
      {0x5555, 0x5555, 0x5555},
      {0x0000, 0x0000, 0x0000},
      #endif
    };

    uint64 R = monochrome[color][0];
    uint64 G = monochrome[color][1];
    uint64 B = monochrome[color][2];

    return R << 32 | G << 16 | B << 0;
  }
}

auto GameBoyInterface::audioFrequency() -> double {
  return 4194304.0 / 2.0;
}

auto GameBoyInterface::loaded() -> bool {
  return system.loaded();
}

auto GameBoyInterface::sha256() -> string {
  return cartridge.sha256();
}

auto GameBoyInterface::load(uint id) -> bool {
  if(id == ID::GameBoy) return system.load(this, System::Revision::GameBoy);
  return false;
}

auto GameBoyInterface::save() -> void {
  system.save();
}

auto GameBoyInterface::unload() -> void {
  save();
  system.unload();
}

auto GameBoyInterface::power() -> void {
  system.power();
}

auto GameBoyInterface::run() -> void {
  system.run();
}

auto GameBoyInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto GameBoyInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto GameBoyInterface::cheatSet(const string_vector& list) -> void {
  cheat.assign(list);
}

auto GameBoyInterface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto GameBoyInterface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto GameBoyInterface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    system.configureVideoEffects();
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    system.configureVideoPalette();
    return true;
  }

  return false;
}
