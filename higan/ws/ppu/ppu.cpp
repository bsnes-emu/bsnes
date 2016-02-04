#include <ws/ws.hpp>

namespace WonderSwan {

PPU ppu;
#include "io.cpp"
#include "video.cpp"

auto PPU::Enter() -> void {
  ppu.main();
}

auto PPU::main() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    step(256);

    status.hclk = 0;
    if(++status.vclk == 159) {
      status.vclk = 0;
      video.refresh();
    }
  }
}

auto PPU::step(uint clocks) -> void {
  status.hclk += clocks;

  clock += clocks;
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}

auto PPU::power() -> void {
  create(PPU::Enter, 3072000);

  for(uint n = 0x0000; n <= 0x0001; n++) iomap[n] = this;
  for(uint n = 0x0004; n <= 0x0007; n++) iomap[n] = this;
  for(uint n = 0x0010; n <= 0x0015; n++) iomap[n] = this;
  for(uint n = 0x001c; n <= 0x001f; n++) iomap[n] = this;
  iomap[0x0060] = this;

  for(auto& n : output) n = 0;

  status.vclk = 0;
  status.hclk = 0;

  video.power();
}

}
