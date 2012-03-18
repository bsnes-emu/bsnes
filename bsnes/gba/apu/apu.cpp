#include <gba/gba.hpp>

#define APU_CPP
namespace GBA {

APU apu;

void APU::Enter() { apu.enter(); }

void APU::enter() {
  while(true) {
  //scheduler.synchronize_all();

    interface->audioSample(0, 0);
    step(1);
  }
}

void APU::step(unsigned clocks) {
  clock += clocks << 9;  //16777216hz / 32768hz = 512hz (1 << 9)
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) {
    co_switch(cpu.thread);
  }
}

void APU::power() {
  create(APU::Enter, 16777216);
}

}
