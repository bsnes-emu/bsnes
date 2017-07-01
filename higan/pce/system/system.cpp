#include <pce/pce.hpp>

namespace PCEngine {

System system;
Scheduler scheduler;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vce.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vce);
  scheduler.synchronize(vdc0);
  scheduler.synchronize(vdc1);
  scheduler.synchronize(psg);
}

auto System::load(Emulator::Interface* interface, Model model) -> bool {
  information = {};
  information.model = model;

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;

  cpu.load();
  serializeInit();
  this->interface = interface;
  information.colorburst = Emulator::Constants::Colorburst::NTSC;
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
  cpu.save();
}

auto System::unload() -> void {
  cpu.peripherals.reset();
  controllerPort.unload();
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
  vce.power();
  vpc.power();
  vdc0.power();
  vdc1.power();
  psg.power();
  scheduler.primary(cpu);

  controllerPort.power();
  controllerPort.connect(settings.controllerPort);
}

}
