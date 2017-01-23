#include <pce/pce.hpp>

namespace PCEngine {

uint Model::id;
System system;
Scheduler scheduler;
#include "peripherals.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vce.refresh();
}

auto System::load(Emulator::Interface* interface, uint id) -> bool {
  Model::id = id;
  information = {};

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;

  this->interface = interface;
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
  vpc.power();
  vce.power();
  vdc0.power();
  vdc1.power();
  psg.power();
  scheduler.primary(cpu);

  peripherals.reset();
}

}
