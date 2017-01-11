#include <pce/pce.hpp>

namespace PCEngine {

System system;
Scheduler scheduler;
#include "peripherals.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vdc.refresh();
}

auto System::load() -> bool {
  information = {};

  if(auto fp = interface->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;

  information.colorburst = Emulator::Constants::Colorburst::NTSC;
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  peripherals.unload();
  cartridge.unload();
}

auto System::power() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  Emulator::video.setPalette();

  Emulator::audio.reset();
  Emulator::audio.setInterface(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  vdc.power();
  psg.power();
  scheduler.primary(cpu);

  peripherals.reset();
}

}
