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

auto System::load(Emulator::Interface* interface, Model model_, maybe<uint> systemID) -> bool {
  _model = model_;

  if(model() == Model::GameBoy) {
    if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
      information.manifest = fp->reads();
    } else return false;

    auto document = BML::unserialize(information.manifest);
    if(auto name = document["system/cpu/rom/name"].text()) {
      if(auto fp = platform->open(ID::System, name, File::Read, File::Required)) {
        fp->read(bootROM.dmg, 256);
      }
    }
  }

  if(model() == Model::GameBoyColor) {
    if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
      information.manifest = fp->reads();
    } else return false;

    auto document = BML::unserialize(information.manifest);
    if(auto name = document["system/cpu/rom/name"].text()) {
      if(auto fp = platform->open(ID::System, name, File::Read, File::Required)) {
        fp->read(bootROM.cgb, 2048);
      }
    }
  }

  if(model() == Model::SuperGameBoy) {
    if(auto fp = platform->open(systemID(), "manifest.bml", File::Read, File::Required)) {
      information.manifest = fp->reads();
    } else return false;

    auto document = BML::unserialize(information.manifest);
    if(auto name = document["board/icd2/rom/name"].text()) {
      if(auto fp = platform->open(systemID(), name, File::Read, File::Required)) {
        fp->read(bootROM.sgb, 256);
      }
    }
  }

  if(!cartridge.load()) return false;
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
  if(model() != Model::SuperGameBoy) {
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
