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
    device.input.append({ 0, 0, "X1"});
    device.input.append({ 1, 0, "X2"});
    device.input.append({ 2, 0, "X3"});
    device.input.append({ 3, 0, "X4"});
    device.input.append({ 4, 0, "Y1"});
    device.input.append({ 5, 0, "Y2"});
    device.input.append({ 6, 0, "Y3"});
    device.input.append({ 7, 0, "Y4"});
    device.input.append({ 8, 0, "B"});
    device.input.append({ 9, 0, "A"});
    device.input.append({10, 0, "Start"});
    device.order = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    this->device.append(device);
  }

  { Device device{1, ID::DeviceVertical, "Controller"};
    device.input.append({ 0, 0, "X1"});
    device.input.append({ 1, 0, "X2"});
    device.input.append({ 2, 0, "X3"});
    device.input.append({ 3, 0, "X4"});
    device.input.append({ 4, 0, "Y1"});
    device.input.append({ 5, 0, "Y2"});
    device.input.append({ 6, 0, "Y3"});
    device.input.append({ 7, 0, "Y4"});
    device.input.append({ 8, 0, "B"});
    device.input.append({ 9, 0, "A"});
    device.input.append({10, 0, "Start"});
    device.order = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
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
    return 0;
  case ID::Manifest:
  case ID::ROM:
  case ID::RAM:
    switch(system.revision()) {
    case System::Revision::WonderSwan:
      return ID::WonderSwan;
    case System::Revision::WonderSwanColor:
    case System::Revision::SwanCrystal:
      return ID::WonderSwanColor;
    }
  }
  throw;
}

auto Interface::load(uint id) -> void {
  if(id == ID::WonderSwan) system.load(System::Revision::WonderSwan);
  if(id == ID::WonderSwanColor) system.load(System::Revision::WonderSwanColor);
}

auto Interface::save() -> void {
  if(cartridge.ram.name) interface->saveRequest(ID::RAM, cartridge.ram.name);
}

auto Interface::load(uint id, const stream& stream) -> void {
  if(id == ID::SystemManifest) {
    system.information.manifest = stream.text();
  }

  if(id == ID::Manifest) {
    cartridge.information.manifest = stream.text();
  }

  if(id == ID::ROM) {
    stream.read(cartridge.rom.data, min(cartridge.rom.size, stream.size()));
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ram.data, min(cartridge.ram.size, stream.size()));
  }
}

auto Interface::save(uint id, const stream& stream) -> void {
  if(id == ID::RAM) {
    stream.write(cartridge.ram.data, cartridge.ram.size);
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
