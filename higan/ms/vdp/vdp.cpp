#include <ms/ms.hpp>

namespace MasterSystem {

VDP vdp;
#include "io.cpp"
#include "background.cpp"
#include "sprite.cpp"
#include "serialization.cpp"

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
  if(y < vlines()) {
    uint32* screen = buffer + (24 + y) * 256;
    for(uint x : range(256)) {
      background.run();
      sprite.run();
      step(2);

      uint12 color = palette(16 | io.backdropColor);
      if(!io.leftClip || x >= 8) {
        if(background.output.priority || !sprite.output.color) {
          color = palette(background.output.palette << 4 | background.output.color);
        } else if(sprite.output.color) {
          color = palette(16 | sprite.output.color);
        }
      }
      if(!io.displayEnable) color = 0;
      *screen++ = color;
    }
  } else {
    //Vblank
    step(512);
  }
  step(172);

  if(io.vcounter == 240) scheduler.exit(Scheduler::Event::Frame);
}

auto VDP::step(uint clocks) -> void {
  while(clocks--) {
    if(++io.hcounter == 684) {
      io.hcounter = 0;
      if(++io.vcounter == (Region::NTSC() ? 262 : 312)) {
        io.vcounter = 0;
      }
    }

    cpu.setINT((io.lineInterrupts && io.intLine) || (io.frameInterrupts && io.intFrame));
    Thread::step(1);
    synchronize(cpu);
  }
}

auto VDP::refresh() -> void {
  if(Model::MasterSystem()) {
    //center the video output vertically in the viewport
    uint32* screen = buffer;
    if(vlines() == 224) screen += 16 * 256;
    if(vlines() == 240) screen += 24 * 256;

    Emulator::video.refresh(screen, 256 * sizeof(uint32), 256, 240);
  }

  if(Model::GameGear()) {
    Emulator::video.refresh(buffer + 48 * 256 + 48, 256 * sizeof(uint32), 160, 144);
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

  memory::fill<uint32>(buffer, 256 * 264);
  io = {};

  background.power();
  sprite.power();
}

auto VDP::palette(uint5 index) -> uint12 {
  if(Model::MasterSystem()) {
    return cram[index];
  }

  if(Model::GameGear()) {
    return cram[index * 2 + 0] << 0 | cram[index * 2 + 1] << 8;
  }

  return 0;
}

}
