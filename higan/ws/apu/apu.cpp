#include <ws/ws.hpp>

namespace WonderSwan {

APU apu;

auto APU::Enter() -> void {
  while(true) scheduler.synchronize(), apu.main();
}

auto APU::main() -> void {
  step(128);
  interface->audioSample(0, 0);
}

auto APU::step(uint clocks) -> void {
  clock += clocks;
  if(clock >= 0 && !scheduler.synchronizing()) co_switch(cpu.thread);
}

auto APU::power() -> void {
  create(APU::Enter, 3'072'000);
}

}
