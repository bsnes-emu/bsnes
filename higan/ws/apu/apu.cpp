#include <ws/ws.hpp>

namespace WonderSwan {

APU apu;

auto APU::Enter() -> void {
  apu.main();
}

auto APU::main() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    step(128);
    interface->audioSample(0, 0);
  }
}

auto APU::step(uint clocks) -> void {
  clock += clocks;
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}

auto APU::power() -> void {
  create(APU::Enter, 3072000);
}

}
