#include <ws/ws.hpp>

namespace WonderSwan {

System system;
#include "io.cpp"
#include "video.cpp"
#include "serialization.cpp"

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::term() -> void {
}

auto System::load(Model model) -> bool {
  _model = model;

  if(auto fp = interface->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);

  //note: IPLROM is currently undumped; otherwise we'd load it here ...

  if(auto node = document["system/eeprom"]) {
    eeprom.setName(node["name"].text());
    eeprom.setSize(node["size"].natural() / sizeof(uint16));
    eeprom.erase();
    //initialize user-data section
    for(uint addr = 0x0030; addr <= 0x003a; addr++) eeprom[addr] = 0x0000;
    if(auto fp = interface->open(ID::System, eeprom.name(), File::Read)) {
      fp->read(eeprom.data(), eeprom.size());
    }
  }

  if(!cartridge.load()) return false;
  serializeInit();
  _orientation = cartridge.information.orientation;
  return _loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;

  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;

  eeprom.setName("");
  eeprom.setSize(0);

  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  configureVideoPalette();
  configureVideoEffects();

  Emulator::audio.reset();
  Emulator::audio.setInterface(interface);

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

  r.unknown = 0;
  r.format = 0;
  r.depth = 0;
  r.color = 0;
}

auto System::run() -> void {
  scheduler.enter();
  pollKeypad();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu.thread);
  scheduler.synchronize(ppu.thread);
  scheduler.synchronize(apu.thread);
  scheduler.synchronize(cartridge.thread);
}

auto System::pollKeypad() -> void {
  uint port = ID::Port::Hardware;
  uint device = !_orientation ? ID::Device::HorizontalControls : ID::Device::VerticalControls;
  bool rotate = keypad.rotate;

  keypad.y1 = interface->inputPoll(port, device, 0);
  keypad.y2 = interface->inputPoll(port, device, 1);
  keypad.y3 = interface->inputPoll(port, device, 2);
  keypad.y4 = interface->inputPoll(port, device, 3);
  keypad.x1 = interface->inputPoll(port, device, 4);
  keypad.x2 = interface->inputPoll(port, device, 5);
  keypad.x3 = interface->inputPoll(port, device, 6);
  keypad.x4 = interface->inputPoll(port, device, 7);
  keypad.b = interface->inputPoll(port, device, 8);
  keypad.a = interface->inputPoll(port, device, 9);
  keypad.start = interface->inputPoll(port, device, 10);
  keypad.rotate = interface->inputPoll(port, device, 11);

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
