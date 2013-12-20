#include <gba/gba.hpp>

namespace GameBoyAdvance {

#include "bios.cpp"
#include "serialization.cpp"
BIOS bios;
System system;

void System::init() {
}

void System::term() {
}

void System::power() {
  bus.power();
  player.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power();
}

void System::load() {
  string manifest = string::read({interface->path(ID::System), "manifest.bml"});
  auto document = Markup::Document(manifest);

  interface->loadRequest(ID::BIOS, document["system/cpu/rom/name"].data);
  if(!file::exists({interface->path(ID::System), document["system/cpu/rom/name"].data})) {
    interface->notify("Error: required Game Boy Advance firmware bios.rom not found.\n");
  }

  serialize_init();
}

void System::run() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) break;
  }
  interface->videoRefresh(video.palette, ppu.output, 4 * 240, 240, 160);
}

void System::runtosave() {
  scheduler.sync = Scheduler::SynchronizeMode::CPU;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active = ppu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.active = apu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
      interface->videoRefresh(video.palette, ppu.output, 4 * 240, 240, 160);
    }
  }
}

}
