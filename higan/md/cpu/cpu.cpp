#include <md/md.hpp>

namespace MegaDrive {

CPU cpu;
#include "bus.cpp"
#include "serialization.cpp"

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

auto CPU::main() -> void {
  if(state.interruptPending) {
    if(state.interruptPending.bit((uint)Interrupt::Reset)) {
      state.interruptPending.bit((uint)Interrupt::Reset) = 0;
      r.a[7] = bus->readWord(0) << 16 | bus->readWord(2) << 0;
      r.pc   = bus->readWord(4) << 16 | bus->readWord(6) << 0;
    }

    if(state.interruptPending.bit((uint)Interrupt::HorizontalBlank)) {
      if(4 > r.i) {
        state.interruptPending.bit((uint)Interrupt::HorizontalBlank) = 0;
        return exception(Exception::Interrupt, Vector::HorizontalBlank, 4);
      }
    }

    if(state.interruptPending.bit((uint)Interrupt::VerticalBlank)) {
      if(6 > r.i) {
        state.interruptPending.bit((uint)Interrupt::VerticalBlank) = 0;
        return exception(Exception::Interrupt, Vector::VerticalBlank, 6);
      }
    }
  }

  instruction();
}

auto CPU::step(uint clocks) -> void {
  while(wait) {
    Thread::step(1);
    synchronize();
  }

  Thread::step(clocks);
  synchronize();
}

auto CPU::synchronize() -> void {
  synchronize(apu);
  synchronize(vdp);
  synchronize(psg);
  synchronize(ym2612);
  for(auto peripheral : peripherals) synchronize(*peripheral);
}

auto CPU::raise(Interrupt interrupt) -> void {
  if(!state.interruptLine.bit((uint)interrupt)) {
    state.interruptLine.bit((uint)interrupt) = 1;
    state.interruptPending.bit((uint)interrupt) = 1;
  }
}

auto CPU::lower(Interrupt interrupt) -> void {
  state.interruptLine.bit((uint)interrupt) = 0;
  state.interruptPending.bit((uint)interrupt) = 0;
}

auto CPU::load(Markup::Node node) -> bool {
  tmssEnable = false;
  if(node["cpu/version"].natural() == 1) {
    if(auto name = node["cpu/rom/name"].text()) {
      if(auto fp = platform->open(ID::System, name, File::Read, File::Required)) {
        fp->read(tmss, 2 * 1024);
        tmssEnable = true;
      }
    }
  }

  return true;
}

auto CPU::power(bool reset) -> void {
  M68K::bus = this;
  M68K::power();
  create(CPU::Enter, system.frequency() / 7.0);

  if(!reset) memory::fill(ram, sizeof(ram));

  io = {};
  io.version = tmssEnable;
  io.romEnable = !tmssEnable;
  io.vdpEnable[0] = !tmssEnable;
  io.vdpEnable[1] = !tmssEnable;

  state = {};
  state.interruptPending.bit((uint)Interrupt::Reset) = 1;
}

}
