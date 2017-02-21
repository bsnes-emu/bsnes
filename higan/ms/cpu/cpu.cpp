#include <ms/ms.hpp>

namespace MasterSystem {

CPU cpu;

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

//called once per frame
auto CPU::pollPause() -> void {
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

auto CPU::power() -> void {
  Z80::bus = &MasterSystem::bus;
  Z80::power();
  create(CPU::Enter, system.colorburst());

  r.pc = 0x0000;  //reset vector address

  memory::fill(&state, sizeof(State));
}

}
