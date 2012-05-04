#include <sfc/sfc.hpp>

namespace SuperFamicom {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == ID::IPLROM) {
    stream.read(smp.iplrom, min(64u, stream.size()));
  }

  if(id == ID::ROM) {
    cartridge.load(markup, stream);
    system.power();
    input.connect(0, Input::Device::Joypad);
    input.connect(1, Input::Device::Joypad);
  }

  if(id == ID::SuperGameBoyROM) {
    GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy, markup, stream);
  }

  if(id == ID::BsxFlashROM) {
    bsxflash.memory.copy(stream);
  }

  if(id == ID::SufamiTurboSlotAROM) {
    sufamiturbo.slotA.rom.copy(stream);
  }

  if(id == ID::SufamiTurboSlotBROM) {
    sufamiturbo.slotB.rom.copy(stream);
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ram.data(), min(cartridge.ram.size(), stream.size()));
  }

  if(id == ID::RTC) {
    stream.read(srtc.rtc, min(stream.size(), sizeof srtc.rtc));
  }

  if(id == ID::SPC7110RTC) {
    stream.read(spc7110.rtc, min(stream.size(), sizeof srtc.rtc));
  }

  if(id == ID::BsxRAM) {
    stream.read(bsxcartridge.sram.data(), min(stream.size(), bsxcartridge.sram.size()));
  }

  if(id == ID::BsxPSRAM) {
    stream.read(bsxcartridge.psram.data(), min(stream.size(), bsxcartridge.psram.size()));
  }
}

void Interface::save(unsigned id, const stream &stream) {
  if(id == ID::RAM) {
    stream.write(cartridge.ram.data(), cartridge.ram.size());
  }

  if(id == ID::RTC) {
    stream.write(srtc.rtc, sizeof srtc.rtc);
  }

  if(id == ID::SPC7110RTC) {
    stream.write(spc7110.rtc, sizeof srtc.rtc);
  }

  if(id == ID::BsxRAM) {
    stream.write(bsxcartridge.sram.data(), bsxcartridge.sram.size());
  }

  if(id == ID::BsxPSRAM) {
    stream.write(bsxcartridge.psram.data(), bsxcartridge.psram.size());
  }
}

void Interface::unload() {
  cartridge.unload();
}

void Interface::connect(unsigned port, unsigned device) {
  input.connect(port, (Input::Device)device);
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

  information.name        = "Super Famicom";
  information.width       = 256;
  information.height      = 240;
  information.overscan    = true;
  information.aspectRatio = 8.0 / 7.0;
  information.frequency   = 32040;
  information.resettable  = true;

  information.media.append({"Super Famicom",    "sfc"});
  information.media.append({"BS-X Satellaview", "bs" });
  information.media.append({"Sufami Turbo",     "st" });
  information.media.append({"Super Game Boy",   "gb" });

  firmware.append({ID::IPLROM, "Super Famicom", "sys", "spc700.rom"});

  {
    Schema schema(Media{ID::ROM, "Super Famicom", "sfc", "program.rom"});
    this->schema.append(schema);
  }

  {
    Schema schema(Media{ID::ROM, "Super Game Boy", "sfc", "program.rom"});
    schema.slot.append({ID::SuperGameBoyROM, "Game Boy", "gb", "program.rom"});
    this->schema.append(schema);
  }

  {
    Schema schema(Media{ID::ROM, "BS-X Satellaview", "sfc", "program.rom"});
    schema.slot.append({ID::BsxFlashROM, "BS-X Satellaview", "bs", "program.rom"});
    this->schema.append(schema);
  }

  {
    Schema schema(Media{ID::ROM, "Sufami Turbo", "sfc", "program.rom"});
    schema.slot.append({ID::SufamiTurboSlotAROM, "Sufami Turbo - Slot A", "st", "program.rom"});
    schema.slot.append({ID::SufamiTurboSlotBROM, "Sufami Turbo - Slot B", "st", "program.rom"});
    this->schema.append(schema);
  }

  {
    Port port{0, "Port 1"};
    port.device.append(none());
    port.device.append(controller());
    port.device.append(multitap());
    port.device.append(mouse());
    port.device.append(usart());
    this->port.append(port);
  }

  {
    Port port{1, "Port 2"};
    port.device.append(none());
    port.device.append(controller());
    port.device.append(multitap());
    port.device.append(mouse());
    port.device.append(superScope());
    port.device.append(justifier());
    port.device.append(justifiers());
    this->port.append(port);
  }
}

Emulator::Interface::Port::Device Interface::none() {
  Port::Device device{0, "None"};
  return device;
}

Emulator::Interface::Port::Device Interface::controller() {
  Port::Device device{1, "Controller"};
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
  return device;
}

Emulator::Interface::Port::Device Interface::multitap() {
  Port::Device device{2, "Multitap"};
  for(unsigned p = 1, n = 0; p <= 4; p++, n += 12) {
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
  return device;
}

Emulator::Interface::Port::Device Interface::mouse() {
  Port::Device device{3, "Mouse"};
  device.input.append({0, 1, "X-axis"});
  device.input.append({1, 1, "Y-axis"});
  device.input.append({2, 0, "Left"  });
  device.input.append({3, 0, "Right" });
  device.order = {0, 1, 2, 3};
  return device;
}

Emulator::Interface::Port::Device Interface::superScope() {
  Port::Device device{4, "Super Scope"};
  device.input.append({0, 1, "X-axis" });
  device.input.append({1, 1, "Y-axis" });
  device.input.append({2, 0, "Trigger"});
  device.input.append({3, 0, "Cursor" });
  device.input.append({4, 0, "Turbo"  });
  device.input.append({5, 0, "Pause"  });
  device.order = {0, 1, 2, 3, 4, 5};
  return device;
}

Emulator::Interface::Port::Device Interface::justifier() {
  Port::Device device{5, "Justifier"};
  device.input.append({0, 1, "X-axis" });
  device.input.append({1, 1, "Y-axis" });
  device.input.append({2, 0, "Trigger"});
  device.input.append({3, 0, "Start"  });
  device.order = {0, 1, 2, 3};
  return device;
}

Emulator::Interface::Port::Device Interface::justifiers() {
  Port::Device device{6, "Justifiers"};
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
  return device;
}

Emulator::Interface::Port::Device Interface::usart() {
  Port::Device device{7, "Serial USART"};
  return device;
}

}
