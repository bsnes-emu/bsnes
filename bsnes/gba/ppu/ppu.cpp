#include <gba/gba.hpp>

#define PPU_CPP
namespace GBA {

PPU ppu;

void PPU::Enter() { ppu.enter(); }

void PPU::enter() {
  while(true) {
  //scheduler.synchronize_all();

    step(16777216);
    static uint16_t buffer[240 * 160];
    for(unsigned y = 0; y < 160; y++) {
      uint16_t *dp = buffer + y * 240;
      for(unsigned x = 0; x < 240; x++) {
        *dp++ = x + y;
      }
    }
    interface->videoRefresh(buffer);
    scheduler.exit(Scheduler::ExitReason::FrameEvent);
  }
}

void PPU::step(unsigned clocks) {
  clock += clocks;
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) {
    co_switch(cpu.thread);
  }
}

void PPU::power() {
  create(PPU::Enter, 16777216);
}

}
