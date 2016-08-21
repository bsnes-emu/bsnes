#include <md/md.hpp>

namespace MegaDrive {

#include "peripherals.cpp"
System system;
Scheduler scheduler;

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vdp.refresh();
}

auto System::load() -> bool {
  information = Information();
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
  cartridge.power();
  cpu.power();
  apu.power();
  vdp.power();
  psg.power();
  ym2612.power();
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
  apu.reset();
  vdp.reset();
  psg.reset();
  ym2612.reset();
  scheduler.primary(cpu);

  peripherals.reset();
}

}
