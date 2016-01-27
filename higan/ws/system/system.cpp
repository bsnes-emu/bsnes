#include <ws/ws.hpp>

namespace WonderSwan {

System system;

auto System::revision() const -> Revision {
  return _revision;
}

auto System::monochrome() const -> bool {
  return revision() == Revision::WonderSwan;
}

auto System::color() const -> bool {
  return revision() != Revision::WonderSwan;
}

auto System::init() -> void {
}

auto System::term() -> void {
}

auto System::load(Revision revision) -> void {
  _revision = revision;

  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);

  cartridge.load();
}

auto System::power() -> void {
  IO::power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power();
}

auto System::run() -> void {
  while(true) {
    scheduler.enter();
    if(scheduler.exitReason == Scheduler::ExitReason::FrameEvent) break;
  }
}

}
