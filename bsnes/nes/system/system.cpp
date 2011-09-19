#include <nes/nes.hpp>

namespace NES {

System system;

void System::run() {
  scheduler.enter();
}

void System::power() {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
  input.reset();
  scheduler.power();
}

void System::reset() {
  cartridge.reset();
  cpu.reset();
  apu.reset();
  ppu.reset();
  input.reset();
  scheduler.reset();
}

void System::init() {
  assert(interface != 0);
  input.connect(0, Input::Device::Joypad);
  input.connect(1, Input::Device::None);
}

void System::term() {
}

}
