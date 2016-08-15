auto VDP::scanline() -> void {
  state.x = 0;
  if(++state.y >= 262) state.y = 0;
  if(state.y == 0) scheduler.exit(Scheduler::Event::Frame);

  if(state.y == 0) {
    sprite.frame();
  }

  if(state.y < 240) {
    planeA.scanline(state.y);
    window.scanline(state.y);
    planeB.scanline(state.y);
    sprite.scanline(state.y);
  }

  state.output = buffer + (state.y * 2 + 0) * 1280;
}

auto VDP::run() -> void {
  if(!io.displayEnable) return outputPixel(0);

  bool windowed = false;  //todo: broken
  windowed &= state.x >= io.windowHorizontalLo && state.x <= io.windowHorizontalHi;
  windowed &= state.y >= io.windowVerticalLo && state.y <= io.windowVerticalHi;
  auto& planeA = windowed ? this->window : this->planeA;

  planeA.run(state.x, state.y);
  planeB.run(state.x, state.y);
  sprite.run(state.x, state.y);

  auto output = io.backgroundColor;
  if(auto color = planeB.output.color) output = color;
  if(auto color = planeA.output.color) output = color;
  if(auto color = sprite.output.color) output = color;
  if(planeB.output.priority) if(auto color = planeB.output.color) output = color;
  if(planeA.output.priority) if(auto color = planeA.output.color) output = color;
  if(sprite.output.priority) if(auto color = sprite.output.color) output = color;

  outputPixel(cram[output]);
  state.x++;
}

auto VDP::outputPixel(uint9 color) -> void {
  for(auto n : range(4)) {
    state.output[   0 + n] = color;
    state.output[1280 + n] = color;
  }
  state.output += 4;
}
