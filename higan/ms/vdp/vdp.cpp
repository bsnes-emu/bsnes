#include <ms/ms.hpp>

namespace MasterSystem {

VDP vdp;
#include "io.cpp"
#include "background.cpp"
#include "sprite.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  if(io.vcounter <= vlines()) {
    if(io.lcounter-- == 0) {
      io.lcounter = io.lineCounter;
      io.intLine = 1;
    }
  } else {
    io.lcounter = io.lineCounter;
  }

  if(io.vcounter == vlines() + 1) {
    io.intFrame = 1;
  }

  background.scanline();
  sprite.scanline();

  //684 clocks/scanline
  uint y = io.vcounter;
  for(uint x : range(256)) {
    background.run();
    sprite.run();
    step(2);

    uint12 color = palette(io.backdropColor);
    if(background.output.color && (background.output.priority || !sprite.output.color)) {
      color = palette(background.output.palette << 4 | background.output.color);
    } else if(sprite.output.color) {
      color = palette(16 | sprite.output.color);
    }
    if(x <= 7 && io.leftClip) color = palette(io.backdropColor);
    if(!io.displayEnable || y >= vlines()) color = 0;
    buffer[io.vcounter * 256 + x] = color;
  }
  step(172);

  if(io.vcounter == 240) scheduler.exit(Scheduler::Event::Frame);
}

auto VDP::step(uint clocks) -> void {
  while(clocks--) {
    if(++io.hcounter == 684) {
      io.hcounter = 0;
      if(++io.vcounter == 262) {
        io.vcounter = 0;
      }
    }

    cpu.setINT((io.lineInterrupts && io.intLine) || (io.frameInterrupts && io.intFrame));
    Thread::step(1);
    synchronize(cpu);
  }
}

auto VDP::refresh() -> void {
  if(system.model() == Model::MasterSystem) {
    Emulator::video.refresh(buffer, 256 * sizeof(uint32), 256, 240);
  }

  if(system.model() == Model::GameGear) {
    Emulator::video.refresh(buffer + 24 * 256 + 48, 256 * sizeof(uint32), 160, 144);
  }
}

auto VDP::vlines() -> uint {
  if(io.lines240) return 240;
  if(io.lines224) return 224;
  return 192;
}

auto VDP::vblank() -> bool {
  return io.vcounter >= vlines();
}

auto VDP::power() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 5.0);

  memory::fill(&io, sizeof(IO));

  background.power();
  sprite.power();
}

auto VDP::palette(uint5 index) -> uint12 {
  if(system.model() == Model::MasterSystem) {
    return cram[index];
  }

  if(system.model() == Model::GameGear) {
    return cram[index * 2 + 0] << 0 | cram[index * 2 + 1] << 8;
  }

  return 0;
}

}
