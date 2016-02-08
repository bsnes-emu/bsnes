#include <gb/gb.hpp>

namespace GameBoy {

#include "serialization.cpp"
System system;

auto System::loaded() const -> bool { return _loaded; }
auto System::revision() const -> Revision { return _revision; }
auto System::clocksExecuted() const -> uint { return _clocksExecuted; }

System::System() {
  for(auto& byte : bootROM.dmg) byte = 0;
  for(auto& byte : bootROM.sgb) byte = 0;
  for(auto& byte : bootROM.cgb) byte = 0;
}

auto System::run() -> void {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason == Scheduler::ExitReason::FrameEvent) {
    video.refresh();
  }
}

auto System::runToSave() -> void {
  scheduler.sync = Scheduler::SynchronizeMode::CPU;
  runThreadToSave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active_thread = ppu.thread;
  runThreadToSave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active_thread = apu.thread;
  runThreadToSave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

auto System::runThreadToSave() -> void {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason == Scheduler::ExitReason::FrameEvent) {
      video.refresh();
    }
  }
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::load(Revision revision) -> void {
  _revision = revision;

  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);
  string path = "system/cpu/rom/name";
  if(revision == Revision::SuperGameBoy) path = "board/icd2/rom/name";

  if(auto bootROM = document[path].text()) {
    interface->loadRequest(
      revision == Revision::GameBoy ? ID::GameBoyBootROM
    : revision == Revision::SuperGameBoy ? ID::SuperGameBoyBootROM
    : revision == Revision::GameBoyColor ? ID::GameBoyColorBootROM
    : ID::GameBoyBootROM,
      bootROM, true
    );
  }

  cartridge.load(revision);
  serializeInit();
  _loaded = true;
}

auto System::unload() -> void {
  if(!loaded()) return;
  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  bus.power();
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  video.power();
  scheduler.init();

  _clocksExecuted = 0;
}

}
