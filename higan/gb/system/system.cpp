#include <gb/gb.hpp>

namespace GameBoy {

#include "serialization.cpp"
System system;

System::System() {
  for(auto& byte : bootROM.dmg) byte = 0;
  for(auto& byte : bootROM.sgb) byte = 0;
  for(auto& byte : bootROM.cgb) byte = 0;
}

auto System::run() -> void {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason == Scheduler::ExitReason::FrameEvent) {
    interface->videoRefresh(video.palette, ppu.screen, 4 * 160, 160, 144);
  }
}

auto System::runtosave() -> void {
  scheduler.sync = Scheduler::SynchronizeMode::CPU;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active_thread = ppu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active_thread = apu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

auto System::runthreadtosave() -> void {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason == Scheduler::ExitReason::FrameEvent) {
      interface->videoRefresh(video.palette, ppu.screen, 4 * 160, 160, 144);
    }
  }
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::load(Revision revision) -> void {
  this->revision = revision;
  serialize_init();
  if(revision == Revision::SuperGameBoy) return;  //Super Famicom core loads boot ROM for SGB

  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);

  if(auto bootROM = document["system/cpu/rom/name"].text()) {
    interface->loadRequest(
      revision == Revision::GameBoy ? ID::GameBoyBootROM : ID::GameBoyColorBootROM,
      bootROM, true
    );
  }
}

auto System::power() -> void {
  bus.power();
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.init();

  clocks_executed = 0;
}

}
