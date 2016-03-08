#include <ws/ws.hpp>

namespace WonderSwan {

System system;
#include "io.cpp"

auto System::loaded() const -> bool { return _loaded; }
auto System::model() const -> Model { return _model; }
auto System::color() const -> bool { return r.color; }
auto System::planar() const -> bool { return r.format == 0; }
auto System::packed() const -> bool { return r.format == 1; }

auto System::init() -> void {
}

auto System::term() -> void {
}

auto System::load(Model model) -> void {
  _model = model;

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
  iram.power();
  eeprom.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power();

  iomap[0x0060] = this;
  for(uint n = 0x00ba; n <= 0x00be; n++) iomap[n] = this;

  r.depth = 0;
  r.color = 0;
  r.format = 0;
  r.unknown = 0;
}

auto System::run() -> void {
  while(scheduler.enter() != Scheduler::Event::Frame);
}

}
