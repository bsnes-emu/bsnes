#include <gba/gba.hpp>

namespace GameBoyAdvance {

Interface* interface = nullptr;
Settings settings;

Interface::Interface() {
  interface = this;

  information.manufacturer = "Nintendo";
  information.name         = "Game Boy Advance";
  information.width        = 240;
  information.height       = 160;
  information.overscan     = false;
  information.aspectRatio  = 1.0;
  information.resettable   = false;

  information.capability.states = true;
  information.capability.cheats = false;

  media.append({ID::GameBoyAdvance, "Game Boy Advance", "gba", true});

  { Device device{0, ID::Device, "Controller"};
    device.inputs.append({ 0, 0, "Up"    });
    device.inputs.append({ 1, 0, "Down"  });
    device.inputs.append({ 2, 0, "Left"  });
    device.inputs.append({ 3, 0, "Right" });
    device.inputs.append({ 4, 0, "B"     });
    device.inputs.append({ 5, 0, "A"     });
    device.inputs.append({ 6, 0, "L"     });
    device.inputs.append({ 7, 0, "R"     });
    device.inputs.append({ 8, 0, "Select"});
    device.inputs.append({ 9, 0, "Start" });
    device.inputs.append({10, 2, "Rumble"});
    devices.append(device);
  }

  ports.append({0, "Device", {devices[0]}});
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::videoFrequency() -> double {
  return 16777216.0 / (228.0 * 1232.0);
}

auto Interface::videoColors() -> uint32 {
  return 1 << 15;
}

auto Interface::videoColor(uint32 color) -> uint64 {
  uint R = color.bits( 0, 4);
  uint G = color.bits( 5, 9);
  uint B = color.bits(10,14);

  uint64 r = image::normalize(R, 5, 16);
  uint64 g = image::normalize(G, 5, 16);
  uint64 b = image::normalize(B, 5, 16);

  if(settings.colorEmulation) {
    double lcdGamma = 4.0, outGamma = 2.2;
    double lb = pow(B / 31.0, lcdGamma);
    double lg = pow(G / 31.0, lcdGamma);
    double lr = pow(R / 31.0, lcdGamma);
    r = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    g = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    b = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
  }

  return r << 32 | g << 16 | b << 0;
}

auto Interface::audioFrequency() -> double {
  return 16777216.0 / 512.0;
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::group(uint id) -> uint {
  switch(id) {
  case ID::SystemManifest:
  case ID::BIOS:
    return ID::System;
  case ID::Manifest:
  case ID::MROM:
  case ID::SRAM:
  case ID::EEPROM:
  case ID::FLASH:
    return ID::GameBoyAdvance;
  }

  throw;
}

auto Interface::load(uint id) -> void {
  system.load();
}

auto Interface::save() -> void {
  for(auto& memory : cartridge.memory) {
    interface->saveRequest(memory.id, memory.name);
  }
}

auto Interface::load(uint id, const stream& stream) -> void {
  if(id == ID::SystemManifest) {
    system.information.manifest = stream.text();
  }

  if(id == ID::BIOS) {
    stream.read((uint8_t*)bios.data, min(bios.size, stream.size()));
  }

  if(id == ID::Manifest) {
    cartridge.information.markup = stream.text();
  }

  if(id == ID::MROM) {
    stream.read((uint8_t*)cartridge.mrom.data, min(cartridge.mrom.size, stream.size()));
  }

  if(id == ID::SRAM) {
    stream.read((uint8_t*)cartridge.sram.data, min(cartridge.sram.size, stream.size()));
  }

  if(id == ID::EEPROM) {
    stream.read((uint8_t*)cartridge.eeprom.data, min(cartridge.eeprom.size, stream.size()));
  }

  if(id == ID::FLASH) {
    stream.read((uint8_t*)cartridge.flash.data, min(cartridge.flash.size, stream.size()));
  }
}

auto Interface::save(uint id, const stream& stream) -> void {
  if(id == ID::SRAM) {
    stream.write((uint8_t*)cartridge.sram.data, cartridge.sram.size);
  }

  if(id == ID::EEPROM) {
    stream.write((uint8_t*)cartridge.eeprom.data, cartridge.eeprom.size);
  }

  if(id == ID::FLASH) {
    stream.write((uint8_t*)cartridge.flash.data, cartridge.flash.size);
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
  system.power();
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

auto Interface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto Interface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
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

}
