#include <ms/ms.hpp>

namespace MasterSystem {

System system;
Scheduler scheduler;

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vdp.refresh();
}

auto System::load(Model model) -> bool {
  information = {};
  information.model = model;

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
  cartridge.unload();
}

auto System::power() -> void {
  cartridge.power();
  cpu.power();
  vdp.power();
  psg.power();
  reset();
}

auto System::reset() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  Emulator::video.setPalette();

  Emulator::audio.reset();
  Emulator::audio.setInterface(interface);

  scheduler.reset();
  cartridge.reset();
  cpu.reset();
  vdp.reset();
  psg.reset();
  scheduler.primary(cpu);
}

}
