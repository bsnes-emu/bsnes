#include <ws/ws.hpp>

namespace WonderSwan {

Interface* interface = nullptr;
Settings settings;

Interface::Interface() {
  interface = this;

  information.manufacturer = "Bandai";
  information.name         = "WonderSwan";
  information.width        = 224;  //note: technically 224x144; but screen can be rotated
  information.height       = 224;  //by using a square size; this can be done in the core
  information.overscan     = false;
  information.aspectRatio  = 1.0;
  information.resettable   = false;

  information.capability.states = false;
  information.capability.cheats = false;

  media.append({ID::WonderSwan, "WonderSwan", "ws", true});
  media.append({ID::WonderSwanColor, "WonderSwan Color", "wsc", true});

  { Device device{0, ID::DeviceHorizontal, "Controller"};
    device.input.append({ 0, 0, "Y1"});
    device.input.append({ 1, 0, "Y2"});
    device.input.append({ 2, 0, "Y3"});
    device.input.append({ 3, 0, "Y4"});
    device.input.append({ 4, 0, "X1"});
    device.input.append({ 5, 0, "X2"});
    device.input.append({ 6, 0, "X3"});
    device.input.append({ 7, 0, "X4"});
    device.input.append({ 8, 0, "B"});
    device.input.append({ 9, 0, "A"});
    device.input.append({10, 0, "Start"});
    device.input.append({11, 0, "Rotate"});
    device.order = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    this->device.append(device);
  }

  { Device device{1, ID::DeviceVertical, "Controller"};
    device.input.append({ 0, 0, "Y1"});
    device.input.append({ 1, 0, "Y2"});
    device.input.append({ 2, 0, "Y3"});
    device.input.append({ 3, 0, "Y4"});
    device.input.append({ 4, 0, "X1"});
    device.input.append({ 5, 0, "X2"});
    device.input.append({ 6, 0, "X3"});
    device.input.append({ 7, 0, "X4"});
    device.input.append({ 8, 0, "B"});
    device.input.append({ 9, 0, "A"});
    device.input.append({10, 0, "Start"});
    device.input.append({11, 0, "Rotate"});
    device.order = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    this->device.append(device);
  }

  port.append({0, "Horizontal Orientation", {device[0]}});
  port.append({1, "Vertical Orientation", {device[1]}});
}

auto Interface::manifest() -> string {
  return cartridge.information.manifest;
}

auto Interface::title() -> string {
  return cartridge.information.title;
}

auto Interface::videoFrequency() -> double {
  return 3072000.0 / (159.0 * 256.0);  //~75.47hz
}

auto Interface::audioFrequency() -> double {
  return 3072000.0 / 128.0;  //24Khz
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::sha256() -> string {
  return cartridge.information.sha256;
}

auto Interface::group(uint id) -> uint {
  switch(id) {
  case ID::SystemManifest:
  case ID::SystemIPLROM:
  case ID::SystemEEPROM:
    return 0;
  case ID::Manifest:
  case ID::ROM:
  case ID::RAM:
  case ID::EEPROM:
    switch(system.model()) {
    case Model::WonderSwan:
      return ID::WonderSwan;
    case Model::WonderSwanColor:
    case Model::SwanCrystal:
      return ID::WonderSwanColor;
    }
  }
  throw;
}

auto Interface::load(uint id) -> void {
  if(id == ID::WonderSwan) system.load(Model::WonderSwan);
  if(id == ID::WonderSwanColor) system.load(Model::WonderSwanColor);
}

auto Interface::save() -> void {
  if(auto name = system.eeprom.name()) interface->saveRequest(ID::SystemEEPROM, name);
  if(auto name = cartridge.ram.name) interface->saveRequest(ID::RAM, name);
  if(auto name = cartridge.eeprom.name()) interface->saveRequest(ID::EEPROM, name);
}

auto Interface::load(uint id, const stream& stream) -> void {
  if(id == ID::SystemManifest) {
    system.information.manifest = stream.text();
  }

  if(id == ID::SystemEEPROM) {
    stream.read((uint8_t*)system.eeprom.data(), min(system.eeprom.size() * sizeof(uint16), stream.size()));
  }

  if(id == ID::Manifest) {
    cartridge.information.manifest = stream.text();
  }

  if(id == ID::ROM) {
    stream.read((uint8_t*)cartridge.rom.data, min(cartridge.rom.size, stream.size()));
  }

  if(id == ID::RAM) {
    stream.read((uint8_t*)cartridge.ram.data, min(cartridge.ram.size, stream.size()));
  }

  if(id == ID::EEPROM) {
    stream.read((uint8_t*)cartridge.eeprom.data(), min(cartridge.eeprom.size() * sizeof(uint16), stream.size()));
  }
}

auto Interface::save(uint id, const stream& stream) -> void {
  if(id == ID::SystemEEPROM) {
    stream.write((uint8_t*)system.eeprom.data(), system.eeprom.size() * sizeof(uint16));
  }

  if(id == ID::RAM) {
    stream.write((uint8_t*)cartridge.ram.data, cartridge.ram.size);
  }

  if(id == ID::EEPROM) {
    stream.write((uint8_t*)cartridge.eeprom.data(), cartridge.eeprom.size() * sizeof(uint16));
  }
}

auto Interface::unload() -> void {
  save();
}

auto Interface::power() -> void {
  system.power();
}

auto Interface::run() -> void {
  system.run();
}

auto Interface::serialize() -> serializer {
  return {};
}

auto Interface::unserialize(serializer& s) -> bool {
  return false;
}

auto Interface::cap(const string& name) -> bool {
  return false;
}

auto Interface::get(const string& name) -> any {
  return {};
}

auto Interface::set(const string& name, const any& value) -> bool {
  return false;
}

}
