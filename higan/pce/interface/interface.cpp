#include <pce/pce.hpp>

namespace PCEngine {

Interface* interface = nullptr;
Settings settings;

Interface::Interface() {
  interface = this;

  information.manufacturer = "NEC";
  information.name         = "PC Engine";
  information.overscan     = true;
  information.resettable   = false;

  information.capability.states = false;
  information.capability.cheats = false;

  media.append({ID::PCEngine, "PC Engine", "pce"});

  Port controllerPort{ID::Port::Controller, "Controller Port"};

  { Device device{ID::Device::None, "None"};
    controllerPort.devices.append(device);
  }

  { Device device{ID::Device::Gamepad, "Gamepad"};
    device.inputs.append({0, "Up"});
    device.inputs.append({0, "Down"});
    device.inputs.append({0, "Left"});
    device.inputs.append({0, "Right"});
    device.inputs.append({0, "II"});
    device.inputs.append({0, "I"});
    device.inputs.append({0, "Select"});
    device.inputs.append({0, "Run"});
    controllerPort.devices.append(device);
  }

  ports.append(move(controllerPort));
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::videoSize() -> VideoSize {
  return {512, 484};
}

auto Interface::videoSize(uint width, uint height, bool arc) -> VideoSize {
  auto a = arc ? 8.0 / 7.0 : 1.0;
  uint w = 256;
  uint h = 242;
  uint m = min(width / (w * a), height / h);
  return {uint(w * a * m), uint(h * m)};
}

auto Interface::videoFrequency() -> double {
  return 60.0;
}

auto Interface::videoColors() -> uint32 {
  return 1 << 9;
}

auto Interface::videoColor(uint32 color) -> uint64 {
  return 0;
}

auto Interface::audioFrequency() -> double {
  return 44'100.0;  //todo: not accurate
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::load(uint id) -> bool {
  if(id == ID::PCEngine) return system.load();
  return false;
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  system.unload();
}

auto Interface::connect(uint port, uint device) -> void {
  PCEngine::peripherals.connect(port, device);
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
