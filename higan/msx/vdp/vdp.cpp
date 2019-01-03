#include <msx/msx.hpp>

//456 clocks/scanline

namespace MSX {

VDP vdp;
#include "io.cpp"
#include "background.cpp"
#include "sprites.cpp"
#include "serialization.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  if(io.vcounter < 192) {
    uint8 y = io.vcounter;
    sprite(y);
    auto line = buffer + y * 256;
    for(uint8 x : range(256)) {
      background(x, y);
      sprite(x, y);
      line[x] = output.color;
      step(1);
    }
    step(200);
  } else {
    step(456);
  }

  io.vcounter++;
  if(io.vcounter == 262) io.vcounter = 0;
  if(io.vcounter ==   0) io.irqLine = 0;
  if(io.vcounter == 192) io.irqLine = 1, scheduler.exit(Scheduler::Event::Frame);
}

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 192);
}

auto VDP::power() -> void {
  create(VDP::Enter, system.colorburst() * 2);
  vram.allocate(0x4000);

  io = {};
}

}
