#include <gba/gba.hpp>

namespace GameBoyAdvance {

System system;
Scheduler scheduler;
#include "bios.cpp"
#include "serialization.cpp"

auto System::init() -> void {
}

auto System::term() -> void {
}

auto System::power() -> void {
  Emulator::video.reset(interface);
  Emulator::video.setPalette();
  Emulator::video.setEffect(Emulator::Video::Effect::InterframeBlending, settings.blurEmulation);
  Emulator::video.setEffect(Emulator::Video::Effect::RotateLeft, settings.rotateLeft);

  Emulator::audio.reset(interface);

  scheduler.reset();
  bus.power();
  player.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.primary(cpu);
}

auto System::load(Emulator::Interface* interface) -> bool {
  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);

  if(auto name = document["system/cpu/rom/name"].text()) {
    if(auto fp = platform->open(ID::System, name, File::Read, File::Required)) {
      fp->read(bios.data, bios.size);
    } else return false;
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

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
  scheduler.synchronize(player);
}

}
