#include <fc/fc.hpp>

namespace Famicom {

#include "video.cpp"
#include "serialization.cpp"
System system;

auto System::loaded() const -> bool { return _loaded; }

auto System::run() -> void {
  scheduler.enter();
}

auto System::runToSave() -> void {
  scheduler.synchronize(ppu.thread);
  scheduler.synchronize(cpu.thread);
  scheduler.synchronize(apu.thread);
  scheduler.synchronize(cartridge.thread);
}

auto System::load() -> void {
  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);
  cartridge.load();
  serializeInit();
  _loaded = true;
}

auto System::unload() -> void {
  if(!loaded()) return;
  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
  input.reset();
  reset();
}

auto System::reset() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  configureVideoPalette();
  configureVideoEffects();

  Emulator::audio.reset();
  Emulator::audio.setInterface(interface);

  cartridge.reset();
  cpu.reset();
  apu.reset();
  ppu.reset();
  input.reset();
  scheduler.reset();
}

auto System::init() -> void {
  assert(interface != nullptr);
  input.connect(0, Input::Device::Joypad);
  input.connect(1, Input::Device::None);
}

auto System::term() -> void {
}

}
