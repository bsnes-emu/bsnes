#include <sfc/sfc.hpp>

namespace SuperFamicom {

Interface* interface = nullptr;
Settings settings;

Interface::Interface() {
  interface = this;
  system.init();

  information.manufacturer = "Nintendo";
  information.name         = "Super Famicom";
  information.width        = 256;
  information.height       = 240;
  information.overscan     = true;
  information.aspectRatio  = 8.0 / 7.0;
  information.resettable   = true;

  information.capability.states = true;
  information.capability.cheats = true;

  media.append({ID::SuperFamicom, "Super Famicom", "sfc", true });
  media.append({ID::SuperFamicom, "Game Boy",      "gb",  false});
  media.append({ID::SuperFamicom, "BS Memory",     "bs",  false});
  media.append({ID::SuperFamicom, "Sufami Turbo",  "st",  false});

  { Device device{0, ID::ControllerPort1 | ID::ControllerPort2 | ID::ExpansionPort, "None"};
    this->device.append(device);
  }

  { Device device{1, ID::ControllerPort1 | ID::ControllerPort2, "Gamepad"};
    device.input.append({ 0, 0, "B"     });
    device.input.append({ 1, 0, "Y"     });
    device.input.append({ 2, 0, "Select"});
    device.input.append({ 3, 0, "Start" });
    device.input.append({ 4, 0, "Up"    });
    device.input.append({ 5, 0, "Down"  });
    device.input.append({ 6, 0, "Left"  });
    device.input.append({ 7, 0, "Right" });
    device.input.append({ 8, 0, "A"     });
    device.input.append({ 9, 0, "X"     });
    device.input.append({10, 0, "L"     });
    device.input.append({11, 0, "R"     });
    device.order = {4, 5, 6, 7, 0, 8, 1, 9, 10, 11, 2, 3};
    this->device.append(device);
  }

  { Device device{2, ID::ControllerPort1 | ID::ControllerPort2, "Multitap"};
    for(uint p = 1, n = 0; p <= 4; p++, n += 12) {
      device.input.append({n +  0, 0, {"Port ", p, " - ", "B"     }});
      device.input.append({n +  1, 0, {"Port ", p, " - ", "Y"     }});
      device.input.append({n +  2, 0, {"Port ", p, " - ", "Select"}});
      device.input.append({n +  3, 0, {"Port ", p, " - ", "Start" }});
      device.input.append({n +  4, 0, {"Port ", p, " - ", "Up"    }});
      device.input.append({n +  5, 0, {"Port ", p, " - ", "Down"  }});
      device.input.append({n +  6, 0, {"Port ", p, " - ", "Left"  }});
      device.input.append({n +  7, 0, {"Port ", p, " - ", "Right" }});
      device.input.append({n +  8, 0, {"Port ", p, " - ", "A"     }});
      device.input.append({n +  9, 0, {"Port ", p, " - ", "X"     }});
      device.input.append({n + 10, 0, {"Port ", p, " - ", "L"     }});
      device.input.append({n + 11, 0, {"Port ", p, " - ", "R"     }});
      device.order.append(n + 4, n + 5, n +  6, n +  7, n + 0, n + 8);
      device.order.append(n + 1, n + 9, n + 10, n + 11, n + 2, n + 3);
    }
    this->device.append(device);
  }

  { Device device{3, ID::ControllerPort1 | ID::ControllerPort2, "Mouse"};
    device.input.append({0, 1, "X-axis"});
    device.input.append({1, 1, "Y-axis"});
    device.input.append({2, 0, "Left"  });
    device.input.append({3, 0, "Right" });
    device.order = {0, 1, 2, 3};
    this->device.append(device);
  }

  { Device device{4, ID::ControllerPort2, "Super Scope"};
    device.input.append({0, 1, "X-axis" });
    device.input.append({1, 1, "Y-axis" });
    device.input.append({2, 0, "Trigger"});
    device.input.append({3, 0, "Cursor" });
    device.input.append({4, 0, "Turbo"  });
    device.input.append({5, 0, "Pause"  });
    device.order = {0, 1, 2, 3, 4, 5};
    this->device.append(device);
  }

  { Device device{5, ID::ControllerPort2, "Justifier"};
    device.input.append({0, 1, "X-axis" });
    device.input.append({1, 1, "Y-axis" });
    device.input.append({2, 0, "Trigger"});
    device.input.append({3, 0, "Start"  });
    device.order = {0, 1, 2, 3};
    this->device.append(device);
  }

  { Device device{6, ID::ControllerPort2, "Justifiers"};
    device.input.append({0, 1, "Port 1 - X-axis" });
    device.input.append({1, 1, "Port 1 - Y-axis" });
    device.input.append({2, 0, "Port 1 - Trigger"});
    device.input.append({3, 0, "Port 1 - Start"  });
    device.order.append(0, 1, 2, 3);
    device.input.append({4, 1, "Port 2 - X-axis" });
    device.input.append({5, 1, "Port 2 - Y-axis" });
    device.input.append({6, 0, "Port 2 - Trigger"});
    device.input.append({7, 0, "Port 2 - Start"  });
    device.order.append(4, 5, 6, 7);
    this->device.append(device);
  }

  { Device device{7, ID::ControllerPort1, "Serial USART"};
    this->device.append(device);
  }

  { Device device{8, ID::ExpansionPort, "Satellaview"};
    this->device.append(device);
  }

  { Device device{9, ID::ExpansionPort, "eBoot"};
    this->device.append(device);
  }

  port.append({0, "Controller Port 1"});
  port.append({1, "Controller Port 2"});
  port.append({2, "Expansion Port"});

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
  switch(system.region()) { default:
  case System::Region::NTSC: return system.cpuFrequency() / (262.0 * 1364.0 - 4.0);
  case System::Region::PAL:  return system.cpuFrequency() / (312.0 * 1364.0);
  }
}

auto Interface::audioFrequency() -> double {
  return system.apuFrequency() / 768.0;
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
  case ID::IPLROM:
    return 0;
  case ID::Manifest:
  case ID::ROM:
  case ID::RAM:
  case ID::EventROM0:
  case ID::EventROM1:
  case ID::EventROM2:
  case ID::EventROM3:
  case ID::EventRAM:
  case ID::SA1ROM:
  case ID::SA1IRAM:
  case ID::SA1BWRAM:
  case ID::SuperFXROM:
  case ID::SuperFXRAM:
  case ID::ArmDSPPROM:
  case ID::ArmDSPDROM:
  case ID::ArmDSPRAM:
  case ID::HitachiDSPROM:
  case ID::HitachiDSPRAM:
  case ID::HitachiDSPDROM:
  case ID::HitachiDSPDRAM:
  case ID::Nec7725DSPPROM:
  case ID::Nec7725DSPDROM:
  case ID::Nec7725DSPRAM:
  case ID::Nec96050DSPPROM:
  case ID::Nec96050DSPDROM:
  case ID::Nec96050DSPRAM:
  case ID::EpsonRTC:
  case ID::SharpRTC:
  case ID::SPC7110PROM:
  case ID::SPC7110DROM:
  case ID::SPC7110RAM:
  case ID::SDD1ROM:
  case ID::SDD1RAM:
  case ID::OBC1RAM:
  case ID::SuperGameBoyManifest:
  case ID::SuperGameBoyBootROM:
  case ID::MCCROM:
  case ID::MCCRAM:
    return 1;
  case ID::GameBoy:
  case ID::GameBoyManifest:
  case ID::GameBoyROM:
  case ID::GameBoyRAM:
    return 2;
  case ID::BSMemory:
  case ID::BSMemoryManifest:
  case ID::BSMemoryROM:
    return 3;
  case ID::SufamiTurboSlotA:
  case ID::SufamiTurboSlotAManifest:
  case ID::SufamiTurboSlotAROM:
  case ID::SufamiTurboSlotARAM:
    return 4;
  case ID::SufamiTurboSlotB:
  case ID::SufamiTurboSlotBManifest:
  case ID::SufamiTurboSlotBROM:
  case ID::SufamiTurboSlotBRAM:
    return 5;
  }

  throw;
}

auto Interface::load(uint id) -> void {
  if(id == ID::SuperFamicom) system.load();
  if(id == ID::BSMemory) cartridge.loadBSMemory();
  if(id == ID::SufamiTurboSlotA) cartridge.loadSufamiTurboA();
  if(id == ID::SufamiTurboSlotB) cartridge.loadSufamiTurboB();
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

  if(id == ID::IPLROM) {
    stream.read(smp.iplrom, min(64u, stream.size()));
  }

  if(id == ID::Manifest) cartridge.information.markup.cartridge = stream.text();
  if(id == ID::ROM) cartridge.rom.read(stream);
  if(id == ID::RAM) cartridge.ram.read(stream);

  if(id == ID::EventROM0) event.rom[0].read(stream);
  if(id == ID::EventROM1) event.rom[1].read(stream);
  if(id == ID::EventROM2) event.rom[2].read(stream);
  if(id == ID::EventROM3) event.rom[3].read(stream);
  if(id == ID::EventRAM) event.ram.read(stream);

  if(id == ID::SA1ROM) sa1.rom.read(stream);
  if(id == ID::SA1IRAM) sa1.iram.read(stream);
  if(id == ID::SA1BWRAM) sa1.bwram.read(stream);

  if(id == ID::SuperFXROM) superfx.rom.read(stream);
  if(id == ID::SuperFXRAM) superfx.ram.read(stream);

  if(id == ID::ArmDSPPROM) {
    for(auto n : range(128 * 1024)) armdsp.programROM[n] = stream.read();
  }
  if(id == ID::ArmDSPDROM) {
    for(auto n : range( 32 * 1024)) armdsp.dataROM[n] = stream.read();
  }
  if(id == ID::ArmDSPRAM) {
    for(auto n : range( 16 * 1024)) armdsp.programRAM[n] = stream.read();
  }

  if(id == ID::HitachiDSPROM) hitachidsp.rom.read(stream);
  if(id == ID::HitachiDSPRAM) hitachidsp.ram.read(stream);
  if(id == ID::HitachiDSPDROM) {
    for(auto n : range(1024)) hitachidsp.dataROM[n] = stream.readl(3);
  }
  if(id == ID::HitachiDSPDRAM) {
    for(auto n : range(3072)) hitachidsp.dataRAM[n] = stream.readl(1);
  }

  if(id == ID::Nec7725DSPPROM) {
    for(auto n : range( 2048)) necdsp.programROM[n] = stream.readl(3);
  }
  if(id == ID::Nec7725DSPDROM) {
    for(auto n : range( 1024)) necdsp.dataROM[n]    = stream.readl(2);
  }
  if(id == ID::Nec7725DSPRAM) {
    for(auto n : range(  256)) necdsp.dataRAM[n]    = stream.readl(2);
  }
  if(id == ID::Nec96050DSPPROM) {
    for(auto n : range(16384)) necdsp.programROM[n] = stream.readl(3);
  }
  if(id == ID::Nec96050DSPDROM) {
    for(auto n : range( 2048)) necdsp.dataROM[n]    = stream.readl(2);
  }
  if(id == ID::Nec96050DSPRAM) {
    for(auto n : range( 2048)) necdsp.dataRAM[n]    = stream.readl(2);
  }

  if(id == ID::EpsonRTC) {
    uint8 data[16] = {0};
    stream.read(data, min(stream.size(), sizeof data));
    epsonrtc.load(data);
  }

  if(id == ID::SharpRTC) {
    uint8 data[16] = {0};
    stream.read(data, min(stream.size(), sizeof data));
    sharprtc.load(data);
  }

  if(id == ID::SPC7110PROM) spc7110.prom.read(stream);
  if(id == ID::SPC7110DROM) spc7110.drom.read(stream);
  if(id == ID::SPC7110RAM) spc7110.ram.read(stream);

  if(id == ID::SDD1ROM) sdd1.rom.read(stream);
  if(id == ID::SDD1RAM) sdd1.ram.read(stream);

  if(id == ID::OBC1RAM) obc1.ram.read(stream);

  if(id == ID::MCCROM) mcc.rom.read(stream);
  if(id == ID::MCCRAM) mcc.ram.read(stream);

  if(id == ID::SuperGameBoyManifest) {
    GameBoy::interface->load(GameBoy::ID::SystemManifest, stream);
  }

  if(id == ID::SuperGameBoyBootROM) {
    GameBoy::interface->load(GameBoy::ID::SuperGameBoyBootROM, stream);
  }

  if(id == ID::GameBoyManifest) {
    GameBoy::interface->load(GameBoy::ID::Manifest, stream);
  }

  if(id == ID::GameBoyROM) {
    GameBoy::interface->load(GameBoy::ID::ROM, stream);
  }

  if(id == ID::GameBoyRAM) {
    GameBoy::interface->load(GameBoy::ID::RAM, stream);
  }

  if(id == ID::BSMemoryManifest) cartridge.information.markup.bsMemory = stream.text();
  if(id == ID::BSMemoryROM) bsmemory.memory.read(stream);

  if(id == ID::SufamiTurboSlotAManifest) cartridge.information.markup.sufamiTurboA = stream.text();
  if(id == ID::SufamiTurboSlotAROM) sufamiturboA.rom.read(stream);
  if(id == ID::SufamiTurboSlotBROM) sufamiturboB.rom.read(stream);

  if(id == ID::SufamiTurboSlotBManifest) cartridge.information.markup.sufamiTurboB = stream.text();
  if(id == ID::SufamiTurboSlotARAM) sufamiturboA.ram.read(stream);
  if(id == ID::SufamiTurboSlotBRAM) sufamiturboB.ram.read(stream);
}

auto Interface::save(uint id, const stream& stream) -> void {
  if(id == ID::RAM) stream.write(cartridge.ram.data(), cartridge.ram.size());
  if(id == ID::EventRAM) stream.write(event.ram.data(), event.ram.size());
  if(id == ID::SA1IRAM) stream.write(sa1.iram.data(), sa1.iram.size());
  if(id == ID::SA1BWRAM) stream.write(sa1.bwram.data(), sa1.bwram.size());
  if(id == ID::SuperFXRAM) stream.write(superfx.ram.data(), superfx.ram.size());

  if(id == ID::ArmDSPRAM) {
    for(auto n : range(16 * 1024)) stream.write(armdsp.programRAM[n]);
  }

  if(id == ID::HitachiDSPRAM) stream.write(hitachidsp.ram.data(), hitachidsp.ram.size());
  if(id == ID::HitachiDSPDRAM) {
    for(auto n : range(3072)) stream.writel(hitachidsp.dataRAM[n], 1);
  }

  if(id == ID::Nec7725DSPRAM) {
    for(auto n : range( 256)) stream.writel(necdsp.dataRAM[n], 2);
  }
  if(id == ID::Nec96050DSPRAM) {
    for(auto n : range(2048)) stream.writel(necdsp.dataRAM[n], 2);
  }

  if(id == ID::EpsonRTC) {
    uint8 data[16] = {0};
    epsonrtc.save(data);
    stream.write(data, sizeof data);
  }

  if(id == ID::SharpRTC) {
    uint8 data[16] = {0};
    sharprtc.save(data);
    stream.write(data, sizeof data);
  }

  if(id == ID::SPC7110RAM) stream.write(spc7110.ram.data(), spc7110.ram.size());
  if(id == ID::SDD1RAM) stream.write(sdd1.ram.data(), sdd1.ram.size());
  if(id == ID::OBC1RAM) stream.write(obc1.ram.data(), obc1.ram.size());

  if(id == ID::GameBoyRAM) {
    GameBoy::interface->save(GameBoy::ID::RAM, stream);
  }

  if(id == ID::MCCRAM) stream.write(mcc.ram.data(), mcc.ram.size());

  if(id == ID::SufamiTurboSlotARAM) stream.write(sufamiturboA.ram.data(), sufamiturboA.ram.size());
  if(id == ID::SufamiTurboSlotBRAM) stream.write(sufamiturboB.ram.data(), sufamiturboB.ram.size());
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::connect(uint port, uint device) -> void {
  SuperFamicom::device.connect(port, (SuperFamicom::Device::ID)device);
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

auto Interface::rtc() -> bool {
  if(cartridge.hasEpsonRTC()) return true;
  if(cartridge.hasSharpRTC()) return true;
  return false;
}

auto Interface::rtcsync() -> void {
  if(cartridge.hasEpsonRTC()) epsonrtc.sync();
  if(cartridge.hasSharpRTC()) sharprtc.sync();
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

  //Super Game Boy
  if(cartridge.hasICD2()) {
    GameBoy::cheat.reset();
    for(auto& codeset : list) {
      lstring codes = codeset.split("+");
      for(auto& code : codes) {
        lstring part = code.split("/");
        if(part.size() == 2) GameBoy::cheat.append(hex(part[0]), hex(part[1]));
        if(part.size() == 3) GameBoy::cheat.append(hex(part[0]), hex(part[1]), hex(part[2]));
      }
    }
    return;
  }

  //Super Famicom, Broadcast Satellaview, Sufami Turbo
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
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  if(name == "Scanline Emulation") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  if(name == "Scanline Emulation") return settings.scanlineEmulation;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) return settings.blurEmulation = value.get<bool>(), true;
  if(name == "Color Emulation" && value.is<bool>()) return settings.colorEmulation = value.get<bool>(), true;
  if(name == "Scanline Emulation" && value.is<bool>()) return settings.scanlineEmulation = value.get<bool>(), true;
  return false;
}

}
