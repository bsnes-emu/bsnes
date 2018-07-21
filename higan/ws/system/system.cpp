#include <ws/ws.hpp>

namespace WonderSwan {

System system;
Scheduler scheduler;
Cheat cheat;
#include "io.cpp"
#include "serialization.cpp"

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::term() -> void {
}

auto System::load(Emulator::Interface* interface, Model model) -> bool {
  _model = model;

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);

  //note: IPLROM is currently undumped; otherwise we'd load it here ...

  if(auto node = document["system/eeprom"]) {
    eeprom.setSize(node["size"].natural() / sizeof(uint16));
    eeprom.erase();
    //initialize user-data section
    for(uint addr = 0x0030; addr <= 0x003a; addr++) eeprom[addr] = 0x0000;
    if(auto fp = platform->open(ID::System, node["name"].text(), File::Read)) {
      fp->read(eeprom.data(), eeprom.size());
    }
  }

  if(!cartridge.load()) return false;

  serializeInit();
  settings.rotateLeft = cartridge.information.orientation;
  this->interface = interface;
  return _loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;

  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;

  eeprom.setSize(0);

  cartridge.unload();
  _loaded = false;
}

auto System::power() -> void {
  Emulator::video.reset(interface);
  Emulator::video.setPalette();
  Emulator::video.setEffect(Emulator::Video::Effect::InterframeBlending, settings.blurEmulation);
  Emulator::video.setEffect(Emulator::Video::Effect::RotateLeft, settings.rotateLeft);

  Emulator::audio.reset(interface);

  scheduler.reset();
  bus.power();
  iram.power();
  eeprom.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.primary(cpu);

  bus.map(this, 0x0060);
  bus.map(this, 0x00ba, 0x00be);

  r.unknown = 0;
  r.format = 0;
  r.depth = 0;
  r.color = 0;
}

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
  pollKeypad();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
  scheduler.synchronize(cartridge);
}

auto System::pollKeypad() -> void {
  const uint landscape[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const uint portrait [] = {7, 4, 5, 6, 3, 0, 1, 2, 8, 9, 10};

  uint port = ID::Port::Hardware;
  uint device = ID::Device::Controls;
  auto id = !settings.rotateLeft ? landscape : portrait;

  keypad.y1 = platform->inputPoll(port, device, id[0]);
  keypad.y2 = platform->inputPoll(port, device, id[1]);
  keypad.y3 = platform->inputPoll(port, device, id[2]);
  keypad.y4 = platform->inputPoll(port, device, id[3]);
  keypad.x1 = platform->inputPoll(port, device, id[4]);
  keypad.x2 = platform->inputPoll(port, device, id[5]);
  keypad.x3 = platform->inputPoll(port, device, id[6]);
  keypad.x4 = platform->inputPoll(port, device, id[7]);
  keypad.b = platform->inputPoll(port, device, id[8]);
  keypad.a = platform->inputPoll(port, device, id[9]);
  keypad.start = platform->inputPoll(port, device, id[10]);

  if(keypad.y1 || keypad.y2 || keypad.y3 || keypad.y4
  || keypad.x1 || keypad.x2 || keypad.x3 || keypad.x4
  || keypad.b || keypad.a || keypad.start
  ) {
    cpu.raise(CPU::Interrupt::Input);
  }
}

}
