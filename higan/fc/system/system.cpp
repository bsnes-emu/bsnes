#include <fc/fc.hpp>

namespace Famicom {

#include "serialization.cpp"
System system;

auto System::run() -> void {
  scheduler.enter();
  if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
    interface->videoRefresh(video.palette, ppu.buffer, 4 * 256, 256, 240);
  }
}

auto System::runtosave() -> void {
  scheduler.sync = Scheduler::SynchronizeMode::PPU;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cpu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = apu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cartridge.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

auto System::runthreadtosave() -> void {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
      interface->videoRefresh(video.palette, ppu.buffer, 4 * 256, 256, 240);
    }
  }
}

auto System::load() -> void {
  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);

  serialize_init();
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
}

auto System::init() -> void {
  assert(interface != 0);
  input.connect(0, Input::Device::Joypad);
  input.connect(1, Input::Device::None);
}

auto System::term() -> void {
}

}
