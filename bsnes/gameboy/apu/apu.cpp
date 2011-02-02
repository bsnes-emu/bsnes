#include <gameboy/gameboy.hpp>

#include <nall/random.hpp>

#define APU_CPP
namespace GameBoy {

#include "mmio/mmio.cpp"
#include "square/square.cpp"
#include "wave/wave.cpp"
#include "noise/noise.cpp"
#include "master/master.cpp"
#include "serialization.cpp"
APU apu;

void APU::Main() {
  apu.main();
}

void APU::main() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    square1.run();
    square2.run();
    wave.run();
    noise.run();
    master.run();

    system.interface->audio_sample((uint16)prng() >> 3, (uint16)prng() >> 3);

    if(++clock >= 0) co_switch(scheduler.active_thread = cpu.thread);
  }
}

void APU::power() {
  create(Main, 4 * 1024 * 1024);

  for(unsigned n = 0xff10; n <= 0xff3f; n++) bus.mmio[n] = this;

  square1.has_sweep = true;
  square2.has_sweep = false;

  square1.power();
  square2.power();
  wave.power();
  noise.power();
  master.power();
}

}
