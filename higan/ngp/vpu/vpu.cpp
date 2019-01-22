#include <ngp/ngp.hpp>

namespace NeoGeoPocket {

VPU vpu;
#include "serialization.cpp"

auto VPU::Enter() -> void {
  while(true) scheduler.synchronize(), vpu.main();
}

auto VPU::main() -> void {
  cpu.setInterruptHblank(0);
  for(uint hclock : range(480)) {
    io.hcounter++;
    step(1);
  }
  if(io.vcounter <= 150) {
    if(ram[0x0000].bit(6)) cpu.setInterruptHblank(1);
  }
  for(uint hclock : range(35)) {
    io.hcounter++;
    step(1);
  }
  cpu.setInterruptHblank(0);
  io.hcounter = 0;
  io.vcounter++;
  if(io.vcounter == 152) {
    ram[0x0010].bit(6) = 1;
    if(ram[0x0000].bit(7)) cpu.setInterruptVblank(1);
    scheduler.exit(Scheduler::Event::Frame);
  }
  if(io.vcounter == 198) {
    if(ram[0x0000].bit(6)) cpu.setInterruptHblank(1);
  }
  if(io.vcounter == 199) {
    ram[0x0010].bit(6) = 0;
    cpu.setInterruptVblank(0);
    io.vcounter = 0;
  }
}

auto VPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VPU::refresh() -> void {
  for(uint address : range(0x4000)) buffer[address] = ram[address];
  for(uint address : range(0x1f00)) buffer[address + 0x4000] = cpu.ram[address + 0x3000 - 0x1f00];
  Emulator::video.refresh(buffer, 160 * sizeof(uint32), 160, 152);
}

auto VPU::power() -> void {
  create(VPU::Enter, system.frequency());
  ram.allocate(0x4000);
  io = {};
}

}
