#include <md/md.hpp>

namespace MegaDrive {

System system;
Scheduler scheduler;
Cheat cheat;
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

  if(cartridge.region() == "NTSC-J") {
    information.region = Region::NTSCJ;
    information.frequency = Emulator::Constants::Colorburst::NTSC * 15.0;
  }
  if(cartridge.region() == "NTSC-U") {
    information.region = Region::NTSCU;
    information.frequency = Emulator::Constants::Colorburst::NTSC * 15.0;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.frequency = Emulator::Constants::Colorburst::PAL * 12.0;
  }

  serializeInit();
  this->interface = interface;
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  cpu.peripherals.reset();
  controllerPort1.unload();
  controllerPort2.unload();
  extensionPort.unload();
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

  controllerPort1.power(ID::Port::Controller1);
  controllerPort2.power(ID::Port::Controller2);
  extensionPort.power(ID::Port::Extension);

  controllerPort1.connect(settings.controllerPort1);
  controllerPort2.connect(settings.controllerPort2);
  extensionPort.connect(settings.extensionPort);
}

}
