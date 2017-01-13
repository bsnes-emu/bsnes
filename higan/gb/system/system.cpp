#include <gb/gb.hpp>

namespace GameBoy {

#include "video.cpp"
#include "serialization.cpp"
System system;
Scheduler scheduler;
Cheat cheat;

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::load(Emulator::Interface* interface, Revision revision) -> bool {
  _revision = revision;

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  string path = "system/cpu/rom/name";
  if(revision == Revision::SuperGameBoy) path = "board/icd2/rom/name";

  if(auto name = document[path].text()) {
    if(auto fp = platform->open(ID::System, name, File::Read, File::Required)) {
      if(revision == Revision::GameBoy) fp->read(bootROM.dmg, 256);
      if(revision == Revision::SuperGameBoy) fp->read(bootROM.sgb, 256);
      if(revision == Revision::GameBoyColor) fp->read(bootROM.cgb, 2048);
    }
  }

  if(!cartridge.load(revision)) return false;
  serializeInit();
  this->interface = interface;
  return _loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  if(!system.sgb()) {
    Emulator::video.reset();
    Emulator::video.setInterface(interface);
    configureVideoPalette();
    configureVideoEffects();

    Emulator::audio.reset();
    Emulator::audio.setInterface(interface);
  }

  scheduler.reset();
  bus.power();
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.primary(cpu);

  _clocksExecuted = 0;
}

}
