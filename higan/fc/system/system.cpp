#include <fc/fc.hpp>

namespace Famicom {

#include "serialization.cpp"
System system;

auto System::loaded() const -> bool { return _loaded; }

auto System::run() -> void {
  scheduler.enter();
  if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
    video.refresh();
  }
}

auto System::runToSave() -> void {
  scheduler.sync = Scheduler::SynchronizeMode::PPU;
  runThreadToSave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cpu.thread;
  runThreadToSave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = apu.thread;
  runThreadToSave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cartridge.thread;
  runThreadToSave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

auto System::runThreadToSave() -> void {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
      video.refresh();
    }
  }
}

auto System::load() -> void {
  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);
  cartridge.load();
  serializeInit();
  _loaded = true;
}

auto System::unload() -> void {
  if(!loaded()) return;
  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
  input.reset();
  scheduler.power();
  reset();
}

auto System::reset() -> void {
  cartridge.reset();
  cpu.reset();
  apu.reset();
  ppu.reset();
  input.reset();
  scheduler.reset();
  video.reset();
}

auto System::init() -> void {
  assert(interface != 0);
  input.connect(0, Input::Device::Joypad);
  input.connect(1, Input::Device::None);
}

auto System::term() -> void {
}

}
