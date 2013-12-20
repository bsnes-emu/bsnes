#include <gb/gb.hpp>

#define SYSTEM_CPP
namespace GameBoy {

#include "serialization.cpp"
System system;

void System::run() {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
    interface->videoRefresh(video.palette, ppu.screen, 4 * 160, 160, 144);
  }
}

void System::runtosave() {
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

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
      interface->videoRefresh(video.palette, ppu.screen, 4 * 160, 160, 144);
    }
  }
}

void System::init() {
  assert(interface != nullptr);
}

void System::load(Revision revision) {
  this->revision = revision;
  serialize_init();
  if(revision == Revision::SuperGameBoy) return;  //Super Famicom core loads boot ROM for SGB

  string manifest = string::read({interface->path(ID::System), "manifest.bml"});
  auto document = Markup::Document(manifest);

  interface->loadRequest(
    revision == Revision::GameBoy ? ID::GameBoyBootROM : ID::GameBoyColorBootROM,
    document["system/cpu/rom/name"].data
  );
  if(!file::exists({interface->path(ID::System), document["system/cpu/rom/name"].data})) {
    interface->notify("Error: required Game Boy firmware boot.rom not found.\n");
  }
}

void System::power() {
  bus.power();
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.init();

  clocks_executed = 0;
}

System::System() {
  for(auto& byte : bootROM.dmg) byte = 0;
  for(auto& byte : bootROM.sgb) byte = 0;
  for(auto& byte : bootROM.cgb) byte = 0;
}

}
