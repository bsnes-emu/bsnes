#include <ms/ms.hpp>

namespace MasterSystem {

CPU cpu;
#include "coleco.cpp"
#include "sega.cpp"
#include "serialization.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  if(state.nmiLine) {
    state.nmiLine = 0;  //edge-sensitive
    irq(0, 0x0066, 0xff);
  }

  if(state.intLine) {
    //level-sensitive
    irq(1, 0x0038, 0xff);
  }

  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vdp);
  synchronize(psg);
  for(auto peripheral : peripherals) synchronize(*peripheral);
}

auto CPU::synchronizing() const -> bool {
  return scheduler.synchronizing();
}

//called once per frame
auto CPU::pollPause() -> void {
  if(Model::SG1000() || Model::SC3000()) {
    static bool pause = 0;
    bool state = platform->inputPoll(ID::Port::Hardware, ID::Device::SG1000Controls, 0);
    if(!pause && state) setNMI(1);
    pause = state;
  }

  if(Model::MasterSystem()) {
    static bool pause = 0;
    bool state = platform->inputPoll(ID::Port::Hardware, ID::Device::MasterSystemControls, 1);
    if(!pause && state) setNMI(1);
    pause = state;
  }
}

auto CPU::setNMI(bool value) -> void {
  state.nmiLine = value;
}

auto CPU::setINT(bool value) -> void {
  state.intLine = value;
}

auto CPU::read(uint16 address) -> uint8 {
  return Model::ColecoVision() ? readColeco(address) : readSega(address);
}

auto CPU::write(uint16 address, uint8 data) -> void {
  return Model::ColecoVision() ? writeColeco(address, data) : writeSega(address, data);
}

auto CPU::in(uint8 address) -> uint8 {
  return Model::ColecoVision() ? inColeco(address) : inSega(address);
}

auto CPU::out(uint8 address, uint8 data) -> void {
  return Model::ColecoVision() ? outColeco(address, data) : outSega(address, data);
}

auto CPU::power() -> void {
  Z80::bus = this;
  Z80::power();
  create(CPU::Enter, system.colorburst());

  if(Model::ColecoVision()) ram.allocate(0x0400), expansion.allocate(0x1000);
  if(Model::SG1000())       ram.allocate(0x0400);
  if(Model::SC3000())       ram.allocate(0x0800);
  if(Model::MasterSystem()) ram.allocate(0x2000);
  if(Model::GameGear())     ram.allocate(0x2000);

  r.pc = 0x0000;  //reset vector address
  state = {};
  coleco = {};
}

}
