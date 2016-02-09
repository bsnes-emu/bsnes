#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "memory.cpp"
#include "serialization.cpp"
ArmDSP armdsp;

ArmDSP::ArmDSP() {
  programROM = new uint8[128 * 1024];
  dataROM = new uint8[32 * 1024];
  programRAM = new uint8[16 * 1024];
}

ArmDSP::~ArmDSP() {
  delete[] programROM;
  delete[] dataROM;
  delete[] programRAM;
}

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
    step(65536);
    bridge.ready = true;
  }
}

auto ArmDSP::main() -> void {
  if(crash) {
    print(disassemble_arm_instruction(pipeline.execute.address), "\n");
    print(disassemble_registers(), "\n");
    print("Executed: ", instructions, "\n");
    while(true) step(frequency);
  }

  arm_step();
}

auto ArmDSP::step(uint clocks) -> void {
  if(bridge.timer && --bridge.timer == 0);
  Coprocessor::step(clocks);
  synchronizeCPU();
}

//MMIO: 00-3f,80-bf:3800-38ff
//3800-3807 mirrored throughout
//a0 ignored

auto ArmDSP::mmio_read(uint addr, uint8) -> uint8 {
  cpu.synchronizeCoprocessors();

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

auto ArmDSP::mmio_write(uint addr, uint8 data) -> void {
  cpu.synchronizeCoprocessors();

  addr &= 0xff06;

  if(addr == 0x3802) {
    bridge.cputoarm.ready = true;
    bridge.cputoarm.data = data;
  }

  if(addr == 0x3804) {
    data &= 1;
    if(!bridge.reset && data) resetARM();
    bridge.reset = data;
  }
}

auto ArmDSP::init() -> void {
}

auto ArmDSP::load() -> void {
}

auto ArmDSP::unload() -> void {
}

auto ArmDSP::power() -> void {
  for(auto n : range(16 * 1024)) programRAM[n] = random(0x00);
}

auto ArmDSP::reset() -> void {
  bridge.reset = false;
  resetARM();
}

auto ArmDSP::resetARM() -> void {
  create(ArmDSP::Enter, 21'477'272);
  ARM::power();

  bridge.ready = false;
  bridge.signal = false;
  bridge.timer = 0;
  bridge.timerlatch = 0;
  bridge.cputoarm.ready = false;
  bridge.armtocpu.ready = false;
}

}
