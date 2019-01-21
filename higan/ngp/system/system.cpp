#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

System system;
Scheduler scheduler;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    vpu.refresh();
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(vpu);
  scheduler.synchronize(psg);
}

auto System::load(Emulator::Interface* interface_, Model model_) -> bool {
  information.model = model_;

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  if(auto memory = document["system/bios"]) {
    bios.allocate(memory["size"].natural());
    if(auto fp = platform->open(ID::System, memory["name"].text(), File::Read, File::Required)) {
      bios.load(fp);
    } else return false;
  } else return false;

  if(model() == Model::NeoGeoPocket) {
  }

  if(model() == Model::NeoGeoPocketColor) {
  }

  if(!cartridge.load()) return false;
  serializeInit();
  interface = interface_;
  return information.loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  bios.reset();
  cartridge.unload();
  information.loaded = false;
}

auto System::power() -> void {
  Emulator::video.reset(interface);
  Emulator::video.setPalette();
  Emulator::audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  apu.power();
  vpu.power();
  psg.power();
  scheduler.primary(cpu);
}

}
