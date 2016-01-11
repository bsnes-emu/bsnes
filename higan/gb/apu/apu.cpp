#include <gb/gb.hpp>

namespace GameBoy {

#include "square1/square1.cpp"
#include "square2/square2.cpp"
#include "wave/wave.cpp"
#include "noise/noise.cpp"
#include "master/master.cpp"
#include "serialization.cpp"
APU apu;

auto APU::Main() -> void {
  apu.main();
}

auto APU::main() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(sequencer_base == 0) {  //512hz
      if(sequencer_step == 0 || sequencer_step == 2 || sequencer_step == 4 || sequencer_step == 6) {  //256hz
        square1.clock_length();
        square2.clock_length();
        wave.clock_length();
        noise.clock_length();
      }
      if(sequencer_step == 2 || sequencer_step == 6) {  //128hz
        square1.clock_sweep();
      }
      if(sequencer_step == 7) {  //64hz
        square1.clock_envelope();
        square2.clock_envelope();
        noise.clock_envelope();
      }
      sequencer_step++;
    }
    sequencer_base++;

    square1.run();
    square2.run();
    wave.run();
    noise.run();
    master.run();

    hipass(master.center, master.center_bias);
    hipass(master.left, master.left_bias);
    hipass(master.right, master.right_bias);

    interface->audioSample(master.left, master.right);

    clock += cpu.frequency;
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(scheduler.active_thread = cpu.thread);
  }
}

auto APU::hipass(int16& sample, int64& bias) -> void {
  bias += ((((int64)sample << 16) - (bias >> 16)) * 57593) >> 16;
  sample = sclamp<16>(sample - (bias >> 32));
}

auto APU::power() -> void {
  create(Main, 2 * 1024 * 1024);
  for(uint n = 0xff10; n <= 0xff3f; n++) bus.mmio[n] = this;

  sequencer_base = 0;
  sequencer_step = 0;

  square1.power();
  square2.power();
  wave.power();
  noise.power();
  master.power();

  LinearFeedbackShiftRegisterGenerator r;
  for(auto& n : wave.pattern) n = r();
}

auto APU::mmio_read(uint16 addr) -> uint8 {
//if(!master.enable && addr != 0xff26) return 0xff;
  if(addr >= 0xff10 && addr <= 0xff14) return square1.read(addr);
  if(addr >= 0xff15 && addr <= 0xff19) return square2.read(addr);
  if(addr >= 0xff1a && addr <= 0xff1e) return wave.read(addr);
  if(addr >= 0xff1f && addr <= 0xff23) return noise.read(addr);
  if(addr >= 0xff24 && addr <= 0xff26) return master.read(addr);
  if(addr >= 0xff30 && addr <= 0xff3f) return wave.read(addr);
  return 0xff;
}

auto APU::mmio_write(uint16 addr, uint8 data) -> void {
  if(!master.enable && addr != 0xff26) return;
  if(addr >= 0xff10 && addr <= 0xff14) return square1.write(addr, data);
  if(addr >= 0xff15 && addr <= 0xff19) return square2.write(addr, data);
  if(addr >= 0xff1a && addr <= 0xff1e) return wave.write(addr, data);
  if(addr >= 0xff1f && addr <= 0xff23) return noise.write(addr, data);
  if(addr >= 0xff24 && addr <= 0xff26) return master.write(addr, data);
  if(addr >= 0xff30 && addr <= 0xff3f) return wave.write(addr, data);
}

}
