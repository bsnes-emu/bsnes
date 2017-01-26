#include <ms/ms.hpp>

namespace MasterSystem {

Settings settings;
#include "master-system.cpp"
#include "game-gear.cpp"

Interface::Interface() {
  information.capability.states = false;
  information.capability.cheats = false;
}

auto Interface::manifest() -> string {
  return cartridge.manifest();
}

auto Interface::title() -> string {
  return cartridge.title();
}

auto Interface::audioFrequency() -> double {
  return 44'100.0;  //todo: not correct
}

auto Interface::loaded() -> bool {
  return system.loaded();
}

auto Interface::save() -> void {
  system.save();
}

auto Interface::unload() -> void {
  save();
  system.unload();
}

auto Interface::connect(uint port, uint device) -> void {
  peripherals.connect(port, device);
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
