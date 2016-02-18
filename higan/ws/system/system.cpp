#include <ws/ws.hpp>

namespace WonderSwan {

System system;
#include "io.cpp"

auto System::loaded() const -> bool { return _loaded; }
auto System::revision() const -> Revision { return _revision; }

auto System::init() -> void {
}

auto System::term() -> void {
}

auto System::load(Revision revision) -> void {
  _revision = revision;

  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);

  //note: IPLROM is currently undumped; otherwise we'd load it here ...

  if(auto node = document["system/eeprom"]) {
    eeprom.setName(node["name"].text());
    eeprom.setSize(node["size"].natural() / sizeof(uint16));
    eeprom.erase();
    interface->loadRequest(ID::SystemEEPROM, eeprom.name(), false);
  }

  cartridge.load();
  _loaded = true;
}

auto System::unload() -> void {
  if(!loaded()) return;

  eeprom.setName("");
  eeprom.setSize(0);

  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  IO::power();
  eeprom.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power();

  for(uint n = 0x00ba; n <= 0x00be; n++) iomap[n] = this;
}

auto System::run() -> void {
  while(scheduler.enter() != Scheduler::Event::Frame);
}

}
