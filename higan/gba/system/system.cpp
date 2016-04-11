#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "bios.cpp"
#include "video.cpp"
#include "serialization.cpp"
BIOS bios;
System system;

auto System::loaded() const -> bool { return _loaded; }

auto System::init() -> void {
}

auto System::term() -> void {
}

auto System::power() -> void {
  bus.power();
  player.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power();
}

auto System::load() -> void {
  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);

  if(auto bios = document["system/cpu/rom/name"].text()) {
    interface->loadRequest(ID::BIOS, bios, true);
  }

  cartridge.load();
  serializeInit();
  configureVideo();
  _loaded = true;
}

auto System::unload() -> void {
  if(!loaded()) return;
  cartridge.unload();
  _loaded = false;
}

auto System::run() -> void {
  while(scheduler.enter() != Scheduler::Event::Frame);
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu.thread);
  scheduler.synchronize(ppu.thread);
  scheduler.synchronize(apu.thread);
}

}
