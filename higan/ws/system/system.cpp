#include <ws/ws.hpp>

namespace WonderSwan {

System system;
#include "io.cpp"

auto System::loaded() const -> bool { return _loaded; }
auto System::model() const -> Model { return _model; }
auto System::orientation() const -> bool { return _orientation; }
auto System::color() const -> bool { return r.color; }
auto System::planar() const -> bool { return r.format == 0; }
auto System::packed() const -> bool { return r.format == 1; }
auto System::depth() const -> bool { return r.depth == 1; }

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
  _orientation = cartridge.information.orientation;
}

auto System::unload() -> void {
  if(!loaded()) return;

  eeprom.setName("");
  eeprom.setSize(0);

  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  bus.power();
  iram.power();
  eeprom.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power();

  bus.map(this, 0x0060);
  bus.map(this, 0x00ba, 0x00be);

  r.depth = 0;
  r.color = 0;
  r.format = 0;
  r.unknown = 0;
}

auto System::run() -> void {
  while(scheduler.enter() != Scheduler::Event::Frame);

  bool rotate = keypad.rotate;
  keypad.y1 = interface->inputPoll(_orientation, 0, 0);
  keypad.y2 = interface->inputPoll(_orientation, 0, 1);
  keypad.y3 = interface->inputPoll(_orientation, 0, 2);
  keypad.y4 = interface->inputPoll(_orientation, 0, 3);
  keypad.x1 = interface->inputPoll(_orientation, 0, 4);
  keypad.x2 = interface->inputPoll(_orientation, 0, 5);
  keypad.x3 = interface->inputPoll(_orientation, 0, 6);
  keypad.x4 = interface->inputPoll(_orientation, 0, 7);
  keypad.b = interface->inputPoll(_orientation, 0, 8);
  keypad.a = interface->inputPoll(_orientation, 0, 9);
  keypad.start = interface->inputPoll(_orientation, 0, 10);
  keypad.rotate = interface->inputPoll(_orientation, 0, 11);

  if(keypad.y1 || keypad.y2 || keypad.y3 || keypad.y4
  || keypad.x1 || keypad.x2 || keypad.x3 || keypad.x4
  || keypad.b || keypad.a || keypad.start
  ) {
    cpu.raise(CPU::Interrupt::Input);
  }

  if(!rotate && keypad.rotate) {
    _orientation = !_orientation;
  }
}

}
