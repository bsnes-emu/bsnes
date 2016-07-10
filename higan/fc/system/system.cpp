#include <fc/fc.hpp>

namespace Famicom {

#include "peripherals.cpp"
#include "video.cpp"
#include "serialization.cpp"
System system;
Scheduler scheduler;
Cheat cheat;

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
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

auto System::load() -> bool {
  information = Information();
  if(auto fp = interface->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else {
    return false;
  }
  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;
  information.colorburst = Emulator::Constants::Colorburst::NTSC;
  serializeInit();
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  peripherals.unload();
  cartridge.unload();
  information.loaded = false;
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
  scheduler.reset(cpu.thread);
  peripherals.reset();
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::term() -> void {
}

}
