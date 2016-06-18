#include <fc/fc.hpp>

namespace Famicom {

#include "peripherals.cpp"
#include "video.cpp"
#include "serialization.cpp"
System system;

auto System::run() -> void {
  scheduler.enter();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu.thread);
  scheduler.synchronize(apu.thread);
  scheduler.synchronize(ppu.thread);
  scheduler.synchronize(cartridge.thread);
  for(auto peripheral : cpu.peripherals) {
    scheduler.synchronize(peripheral->thread);
  }
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
  peripherals.unload();
  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
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
  scheduler.reset();
  peripherals.reset();
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::term() -> void {
}

}
