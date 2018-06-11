#include <sfc/sfc.hpp>

namespace SuperFamicom {

Settings settings;

Interface::Interface() {
  information.manufacturer = "Nintendo";
  information.name         = "Super Famicom";
  information.overscan     = true;
  information.resettable   = true;

  media.append({ID::SuperFamicom, "Super Famicom", "sfc"});

  Port controllerPort1{ID::Port::Controller1, "Controller Port 1"};
  Port controllerPort2{ID::Port::Controller2, "Controller Port 2"};
  Port expansionPort{ID::Port::Expansion, "Expansion Port"};

  { Device device{ID::Device::None, "None"};
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
    expansionPort.devices.append(device);
  }

  { Device device{ID::Device::Gamepad, "Gamepad"};
    device.inputs.append({0, "Up"    });
    device.inputs.append({0, "Down"  });
    device.inputs.append({0, "Left"  });
    device.inputs.append({0, "Right" });
    device.inputs.append({0, "B"     });
    device.inputs.append({0, "A"     });
    device.inputs.append({0, "Y"     });
    device.inputs.append({0, "X"     });
    device.inputs.append({0, "L"     });
    device.inputs.append({0, "R"     });
    device.inputs.append({0, "Select"});
    device.inputs.append({0, "Start" });
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
  }

  { Device device{ID::Device::Mouse, "Mouse"};
    device.inputs.append({1, "X-axis"});
    device.inputs.append({1, "Y-axis"});
    device.inputs.append({0, "Left"  });
    device.inputs.append({0, "Right" });
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
  }

  { Device device{ID::Device::SuperMultitap, "Super Multitap"};
    for(uint p = 2; p <= 5; p++) {
      device.inputs.append({0, {"Port ", p, " - ", "Up"    }});
      device.inputs.append({0, {"Port ", p, " - ", "Down"  }});
      device.inputs.append({0, {"Port ", p, " - ", "Left"  }});
      device.inputs.append({0, {"Port ", p, " - ", "Right" }});
      device.inputs.append({0, {"Port ", p, " - ", "B"     }});
      device.inputs.append({0, {"Port ", p, " - ", "A"     }});
      device.inputs.append({0, {"Port ", p, " - ", "Y"     }});
      device.inputs.append({0, {"Port ", p, " - ", "X"     }});
      device.inputs.append({0, {"Port ", p, " - ", "L"     }});
      device.inputs.append({0, {"Port ", p, " - ", "R"     }});
      device.inputs.append({0, {"Port ", p, " - ", "Select"}});
      device.inputs.append({0, {"Port ", p, " - ", "Start" }});
    }
    controllerPort2.devices.append(device);
  }

  { Device device{ID::Device::SuperScope, "Super Scope"};
    device.inputs.append({1, "X-axis" });
    device.inputs.append({1, "Y-axis" });
    device.inputs.append({0, "Trigger"});
    device.inputs.append({0, "Cursor" });
    device.inputs.append({0, "Turbo"  });
    device.inputs.append({0, "Pause"  });
    controllerPort2.devices.append(device);
  }

  { Device device{ID::Device::Justifier, "Justifier"};
    device.inputs.append({1, "X-axis" });
    device.inputs.append({1, "Y-axis" });
    device.inputs.append({0, "Trigger"});
    device.inputs.append({0, "Start"  });
    controllerPort2.devices.append(device);
  }

  { Device device{ID::Device::Justifiers, "Justifiers"};
    device.inputs.append({1, "Port 1 - X-axis" });
    device.inputs.append({1, "Port 1 - Y-axis" });
    device.inputs.append({0, "Port 1 - Trigger"});
    device.inputs.append({0, "Port 1 - Start"  });
    device.inputs.append({1, "Port 2 - X-axis" });
    device.inputs.append({1, "Port 2 - Y-axis" });
    device.inputs.append({0, "Port 2 - Trigger"});
    device.inputs.append({0, "Port 2 - Start"  });
    controllerPort2.devices.append(device);
  }

  { Device device{ID::Device::Satellaview, "Satellaview"};
    expansionPort.devices.append(device);
  }

  { Device device{ID::Device::S21FX, "21fx"};
    expansionPort.devices.append(device);
  }

  ports.append(move(controllerPort1));
  ports.append(move(controllerPort2));
  ports.append(move(expansionPort));
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::videoInformation() -> VideoInformation {
  VideoInformation vi;
  vi.width  = 256;
  vi.height = 239;
  vi.internalWidth  = 512;
  vi.internalHeight = 478;
  vi.aspectCorrection = 8.0 / 7.0;
  if(Region::NTSC()) vi.refreshRate = system.cpuFrequency() / (262.0 * 1364.0);
  if(Region::PAL())  vi.refreshRate = system.cpuFrequency() / (312.0 * 1364.0);
  return vi;
}

auto Interface::videoColors() -> uint32 {
  return 1 << 19;
}

auto Interface::videoColor(uint32 color) -> uint64 {
  uint r = color.bits( 0, 4);
  uint g = color.bits( 5, 9);
  uint b = color.bits(10,14);
  uint l = color.bits(15,18);

  //luma=0 is not 100% black; but it's much darker than normal linear scaling
  //exact effect seems to be analog; requires > 24-bit color depth to represent accurately
  double L = (1.0 + l) / 16.0 * (l ? 1.0 : 0.25);
  uint64 R = L * image::normalize(r, 5, 16);
  uint64 G = L * image::normalize(g, 5, 16);
  uint64 B = L * image::normalize(b, 5, 16);

  if(settings.colorEmulation) {
    static const uint8 gammaRamp[32] = {
      0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
      0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
      0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
      0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
    };
    R = L * gammaRamp[r] * 0x0101;
    G = L * gammaRamp[g] * 0x0101;
    B = L * gammaRamp[b] * 0x0101;
  }

  return R << 32 | G << 16 | B << 0;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::sha256() -> string {
  return cartridge.sha256();
}

auto Interface::load(uint id) -> bool {
  if(id == ID::SuperFamicom) return system.load(this);
  if(id == ID::BSMemory) return cartridge.loadBSMemory();
  if(id == ID::SufamiTurboA) return cartridge.loadSufamiTurboA();
  if(id == ID::SufamiTurboB) return cartridge.loadSufamiTurboB();
  return false;
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(settings.controllerPort1 = device);
  if(port == ID::Port::Controller2) controllerPort2.connect(settings.controllerPort2 = device);
  if(port == ID::Port::Expansion) expansionPort.connect(settings.expansionPort = device);
}

auto Interface::power() -> void {
  system.power(/* reset = */ false);
}

auto Interface::reset() -> void {
  system.power(/* reset = */ true);
}

auto Interface::run() -> void {
  system.run();
}

auto Interface::rtc() -> bool {
  if(cartridge.has.EpsonRTC) return true;
  if(cartridge.has.SharpRTC) return true;
  return false;
}

auto Interface::rtcSynchronize() -> void {
  if(cartridge.has.EpsonRTC) epsonrtc.sync();
  if(cartridge.has.SharpRTC) sharprtc.sync();
}

auto Interface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto Interface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto Interface::cheatSet(const string_vector& list) -> void {
  cheat.reset();
  #if defined(SFC_SUPERGAMEBOY)
  if(cartridge.has.ICD) return GameBoy::cheat.assign(list);
  #endif
  cheat.assign(list);
}

auto Interface::cap(const string& name) -> bool {
  if(name == "Fast PPU") return true;
  if(name == "Fast DSP") return true;
  if(name == "Mode") return true;
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  if(name == "Scanline Emulation") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Mode") return string{
    system.fastPPU() && system.fastDSP() ? "[Fast PPU+DSP] "
  : system.fastPPU() ? "[Fast PPU] "
  : system.fastDSP() ? "[Fast DSP] "
  : ""
  };
  if(name == "Fast PPU") return settings.fastPPU;
  if(name == "Fast DSP") return settings.fastDSP;
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  if(name == "Scanline Emulation") return settings.scanlineEmulation;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  if(name == "Fast PPU" && value.is<bool>()) {
    settings.fastPPU = value.get<bool>();
    return true;
  }
  if(name == "Fast DSP" && value.is<bool>()) {
    settings.fastDSP = value.get<bool>();
    return true;
  }
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    return true;
  }
  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    Emulator::video.setPalette();
    return true;
  }
  if(name == "Scanline Emulation" && value.is<bool>()) return settings.scanlineEmulation = value.get<bool>(), true;
  return false;
}

}
