auto VDP::frame() -> void {
  latch.overscan = io.overscan;
}

auto VDP::scanline() -> void {
  state.hdot = 0;
  state.hcounter = 0;
  if(++state.vcounter >= frameHeight()) state.vcounter = 0;
  if(state.vcounter == 0) frame();

  latch.displayWidth = io.displayWidth;

  if(state.vcounter < screenHeight()) {
    planeA.scanline(state.vcounter);
    window.scanline(state.vcounter);
    planeB.scanline(state.vcounter);
    sprite.scanline(state.vcounter);
  }

  if(state.vcounter == 240) scheduler.exit(Scheduler::Event::Frame);

  state.output = buffer + (state.vcounter * 2 + 0) * 1280;
}

auto VDP::run() -> void {
  if(!io.displayEnable) return outputPixel(0);
  if(state.vcounter >= screenHeight()) return outputPixel(0);

  auto& planeA = window.isWindowed(state.hdot, state.vcounter) ? window : this->planeA;
  planeA.run(state.hdot, state.vcounter);
  planeB.run(state.hdot, state.vcounter);
  sprite.run(state.hdot, state.vcounter);

  auto output = io.backgroundColor;
  if(auto color = planeB.output.color) output = color;
  if(auto color = planeA.output.color) output = color;
  if(auto color = sprite.output.color) output = color;
  if(planeB.output.priority) if(auto color = planeB.output.color) output = color;
  if(planeA.output.priority) if(auto color = planeA.output.color) output = color;
  if(sprite.output.priority) if(auto color = sprite.output.color) output = color;

  outputPixel(cram.read(output));
  state.hdot++;
}

auto VDP::outputPixel(uint9 color) -> void {
  for(auto n : range(pixelWidth())) {
    state.output[   0 + n] = color;
    state.output[1280 + n] = color;
  }
  state.output += pixelWidth();
}
