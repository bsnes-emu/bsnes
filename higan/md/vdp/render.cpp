auto VDP::scanline() -> void {
  if(state.vcounter < screenHeight()) {
    planeA.scanline(state.vcounter);
    window.scanline(state.vcounter);
    planeB.scanline(state.vcounter);
    sprite.scanline(state.vcounter);
  }

  if(state.vcounter == 240) scheduler.exit(Scheduler::Event::Frame);

  state.output = output + (state.vcounter * 2 + 0) * 1280;
}

auto VDP::run() -> void {
  if(!io.displayEnable) return outputPixel(0);
  if(state.vcounter >= screenHeight()) return outputPixel(0);

  auto& planeA = window.isWindowed(state.hdot, state.vcounter) ? window : this->planeA;
  planeA.run(state.hdot, state.vcounter);
  planeB.run(state.hdot, state.vcounter);
  sprite.run(state.hdot, state.vcounter);

  Pixel g = {io.backgroundColor, 0};
  Pixel a = planeA.output;
  Pixel b = planeB.output;
  Pixel s = sprite.output;

  auto& bg = a.above() || a.color && !b.above() ? a : b.color ? b : g;
  auto& fg = s.above() || s.color && !b.above() && !a.above() ? s : bg;
  uint mode = a.priority || b.priority;

  if(&fg == &s) switch(s.color) {
  case 0x0e:
  case 0x1e:
  case 0x2e: mode  = 1; break;
  case 0x3e: mode += 1; fg = bg; break;
  case 0x3f: mode  = 0; fg = bg; break;
  default:   mode |= s.priority; break;
  }

  auto color = cram.read(fg.color);
  if(!io.shadowHighlightEnable) mode = 1;
  outputPixel(mode << 9 | color);
}

auto VDP::outputPixel(uint32 color) -> void {
  uint32* field[2] = {&state.output[0], &state.output[1280]};
  if(!io.interlaceMode.bit(0)) {
    for(auto n : range(pixelWidth())) {
      field[0][n] = color;
      field[1][n] = color;
    }
  } else {
    for(auto n : range(pixelWidth())) {
      field[state.field][n] = color;
    }
  }
  state.output += pixelWidth();
}
