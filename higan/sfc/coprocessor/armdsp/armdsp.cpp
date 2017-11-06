#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "memory.cpp"
#include "serialization.cpp"
ArmDSP armdsp;

auto ArmDSP::Enter() -> void {
  armdsp.boot();
  while(true) scheduler.synchronize(), armdsp.main();
}

auto ArmDSP::boot() -> void {
  //reset hold delay
  while(bridge.reset) {
    step(1);
    continue;
  }

  //reset sequence delay
  if(bridge.ready == false) {
    step(65'536);
    bridge.ready = true;
  }
}

auto ArmDSP::main() -> void {
  processor.cpsr.t = 0;  //force ARM mode
  instruction();
}

auto ArmDSP::step(uint clocks) -> void {
  if(bridge.timer && --bridge.timer == 0);
  Thread::step(clocks);
  synchronize(cpu);
}

//MMIO: 00-3f,80-bf:3800-38ff
//3800-3807 mirrored throughout
//a0 ignored

auto ArmDSP::read(uint24 addr, uint8) -> uint8 {
  cpu.synchronize(*this);

  uint8 data = 0x00;
  addr &= 0xff06;

  if(addr == 0x3800) {
    if(bridge.armtocpu.ready) {
      bridge.armtocpu.ready = false;
      data = bridge.armtocpu.data;
    }
  }

  if(addr == 0x3802) {
    bridge.signal = false;
  }

  if(addr == 0x3804) {
    data = bridge.status();
  }

  return data;
}

auto ArmDSP::write(uint24 addr, uint8 data) -> void {
  cpu.synchronize(*this);

  addr &= 0xff06;

  if(addr == 0x3802) {
    bridge.cputoarm.ready = true;
    bridge.cputoarm.data = data;
  }

  if(addr == 0x3804) {
    data &= 1;
    if(!bridge.reset && data) reset();
    bridge.reset = data;
  }
}

auto ArmDSP::power() -> void {
  random.array((uint8*)programRAM, sizeof(programRAM));
  bridge.reset = false;
  reset();
}

auto ArmDSP::reset() -> void {
  ARM7TDMI::power();
  create(ArmDSP::Enter, 21'477'272);

  bridge.ready = false;
  bridge.signal = false;
  bridge.timer = 0;
  bridge.timerlatch = 0;
  bridge.cputoarm.ready = false;
  bridge.armtocpu.ready = false;
}

}
