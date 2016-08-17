#include <ms/ms.hpp>

namespace MasterSystem {

Interface* interface = nullptr;
Settings settings;

Interface::Interface() {
  interface = this;

  information.manufacturer = "Sega";
  information.name         = "Master System";
  information.overscan     = true;
  information.resettable   = true;

  information.capability.states = false;
  information.capability.cheats = false;

  media.append({ID::MasterSystem, "Master System", "ms"});
  media.append({ID::GameGear,     "Game Gear",     "gg"});

  Port controllerPort1{ID::Port::Controller1, "Controller Port 1"};
  Port controllerPort2{ID::Port::Controller2, "Controller Port 2"};

  { Device device{ID::Device::Gamepad, "Gamepad"};
    device.inputs.append({0, "Up"});
    device.inputs.append({0, "Down"});
    device.inputs.append({0, "Left"});
    device.inputs.append({0, "Right"});
    device.inputs.append({0, "1"});
    device.inputs.append({0, "2"});
    controllerPort1.devices.append(device);
    controllerPort2.devices.append(device);
  }
}

auto Interface::manifest() -> string {
  return "";
}

auto Interface::title() -> string {
  return "";
}

auto Interface::videoSize() -> VideoSize {
  return {256, 192};
}

auto Interface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  uint w = 256;
  uint h = 192;
  uint m = min(width / w, height / h);
  return {w * m, h * m};
}

auto Interface::videoFrequency() -> double {
  return 60.0;
}

auto Interface::videoColors() -> uint32 {
  return 1 << 6;
}

auto Interface::videoColor(uint32 color) -> uint64 {
  return 0;
}

auto Interface::audioFrequency() -> double {
  return 44'100.0;
}

auto Interface::loaded() -> bool {
  return false;
}

auto Interface::load(uint id) -> bool {
  return false;
}

auto Interface::save() -> void {
}

auto Interface::unload() -> void {
}

auto Interface::power() -> void {
}

auto Interface::reset() -> void {
}

auto Interface::run() -> void {
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
