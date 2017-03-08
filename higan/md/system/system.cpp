#include <md/md.hpp>

namespace MegaDrive {

System system;
Scheduler scheduler;
Cheat cheat;
#include "peripherals.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vdp.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
  scheduler.synchronize(ym2612);
}

auto System::load(Emulator::Interface* interface, maybe<Region> region) -> bool {
  information = {};

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  if(!cartridge.load()) return false;

  serializeInit();
  information.region = Region::NTSCU;
  information.colorburst = Emulator::Constants::Colorburst::NTSC;
  this->interface = interface;
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
  apu.power();
  vdp.power();
  psg.power();
  ym2612.power();
  scheduler.primary(cpu);

  peripherals.reset();
}

}
