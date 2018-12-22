#include <ms/ms.hpp>

namespace MasterSystem {

System system;
Scheduler scheduler;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    cpu.pollPause();
    vdp.refresh();
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
}

auto System::load(Emulator::Interface* interface, Model model) -> bool {
  information = {};
  information.model = model;

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);

  if(MasterSystem::Model::ColecoVision()) {
    if(auto fp = platform->open(ID::System, "bios.rom", File::Read, File::Required)) {
      fp->read(bios, 0x2000);
    } else return false;
  }

  if(!cartridge.load()) return false;

  if(cartridge.region() == "NTSC") {
    information.region = Region::NTSC;
    information.colorburst = Emulator::Constants::Colorburst::NTSC;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.colorburst = Emulator::Constants::Colorburst::PAL * 4.0 / 5.0;
  }

  serializeInit();
  this->interface = interface;
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  if(!MasterSystem::Model::GameGear()) {
    cpu.peripherals.reset();
    controllerPort1.unload();
    controllerPort2.unload();
  }
  cartridge.unload();
}

auto System::power() -> void {
  Emulator::video.reset(interface);
  Emulator::video.setPalette();

  Emulator::audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  vdp.power();
  psg.power();
  scheduler.primary(cpu);

  if(!MasterSystem::Model::GameGear()) {
    controllerPort1.power(ID::Port::Controller1);
    controllerPort2.power(ID::Port::Controller2);

    controllerPort1.connect(settings.controllerPort1);
    controllerPort2.connect(settings.controllerPort2);
  }
}

}
