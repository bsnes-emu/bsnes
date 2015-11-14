Video video;

Video::Video() {
  palette = new uint32_t[1 << 19]();
}

Video::~Video() {
  delete[] palette;
}

auto Video::generate_palette(Emulator::Interface::PaletteMode mode) -> void {
  for(auto color : range(1 << 19)) {
    if(mode == Emulator::Interface::PaletteMode::Literal) {
      palette[color] = color;
      continue;
    }

    uint l = (color >> 15) & 15;
    uint b = (color >> 10) & 31;
    uint g = (color >>  5) & 31;
    uint r = (color >>  0) & 31;

    if(mode == Emulator::Interface::PaletteMode::Channel) {
      l = image::normalize(l, 4, 16);
      r = image::normalize(r, 5, 16);
      g = image::normalize(g, 5, 16);
      b = image::normalize(b, 5, 16);
      palette[color] = interface->videoColor(color, l, r, g, b);
      continue;
    }

    if(mode == Emulator::Interface::PaletteMode::Emulation) {
      r = gamma_ramp[r];
      g = gamma_ramp[g];
      b = gamma_ramp[b];
    } else {
      r = image::normalize(r, 5, 8);
      g = image::normalize(g, 5, 8);
      b = image::normalize(b, 5, 8);
    }

    double L = (1.0 + l) / 16.0;
    if(l == 0) L *= 0.5;
    uint R = L * image::normalize(r, 8, 16);
    uint G = L * image::normalize(g, 8, 16);
    uint B = L * image::normalize(b, 8, 16);

    palette[color] = interface->videoColor(color, 0, R, G, B);
  }
}

//internal

const uint8 Video::gamma_ramp[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};

const uint8 Video::cursor[15 * 15] = {
  0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
  0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,
  0,0,0,1,2,2,1,2,1,2,2,1,0,0,0,
  0,0,1,2,1,1,0,1,0,1,1,2,1,0,0,
  0,1,2,1,0,0,0,1,0,0,0,1,2,1,0,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,
  1,2,1,0,0,1,1,2,1,1,0,0,1,2,1,
  1,2,2,1,1,2,2,2,2,2,1,1,2,2,1,
  1,2,1,0,0,1,1,2,1,1,0,0,1,2,1,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,
  0,1,2,1,0,0,0,1,0,0,0,1,2,1,0,
  0,0,1,2,1,1,0,1,0,1,1,2,1,0,0,
  0,0,0,1,2,2,1,2,1,2,2,1,0,0,0,
  0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,
  0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
};

auto Video::draw_cursor(uint16 color, int x, int y) -> void {
  uint32* data = (uint32*)ppu.output;
  if(ppu.interlace() && ppu.field()) data += 512;

  for(int cy = 0; cy < 15; cy++) {
    int vy = y + cy - 7;
    if(vy <= 0 || vy >= 240) continue;  //do not draw offscreen

    bool hires = (line_width[vy] == 512);
    for(int cx = 0; cx < 15; cx++) {
      int vx = x + cx - 7;
      if(vx < 0 || vx >= 256) continue;  //do not draw offscreen
      uint8_t pixel = cursor[cy * 15 + cx];
      if(pixel == 0) continue;
      uint32_t pixelcolor = (15 << 15) | ((pixel == 1) ? 0 : color);

      if(hires == false) {
        *((uint32_t*)data + vy * 1024 + vx) = pixelcolor;
      } else {
        *((uint32_t*)data + vy * 1024 + vx * 2 + 0) = pixelcolor;
        *((uint32_t*)data + vy * 1024 + vx * 2 + 1) = pixelcolor;
      }
    }
  }
}

auto Video::update() -> void {
  switch(configuration.controllerPort2) {
  case Device::ID::SuperScope:
    if(dynamic_cast<SuperScope*>(device.controllerPort2)) {
      auto controller = (SuperScope&)*device.controllerPort2;
      draw_cursor(0x7c00, controller.x, controller.y);
    }
    break;
  case Device::ID::Justifier:
  case Device::ID::Justifiers:
    if(dynamic_cast<Justifier*>(device.controllerPort2)) {
      auto controller = (Justifier&)*device.controllerPort2;
      draw_cursor(0x001f, controller.player1.x, controller.player1.y);
      if(!controller.chained) break;
      draw_cursor(0x02e0, controller.player2.x, controller.player2.y);
    }
    break;
  }

  auto data = (uint32*)ppu.output;
  if(ppu.interlace() && ppu.field()) data += 512;

  if(hires) {
    //normalize line widths
    for(unsigned y = 0; y < 240; y++) {
      if(line_width[y] == 512) continue;
      uint32* buffer = data + y * 1024;
      for(signed x = 255; x >= 0; x--) {
        buffer[(x * 2) + 0] = buffer[(x * 2) + 1] = buffer[x];
      }
    }
  }

  //overscan: when disabled, shift image down (by scrolling video buffer up) to center image onscreen
  //(memory before ppu.output is filled with black scanlines)
  interface->videoRefresh(
    video.palette,
    ppu.output - (ppu.overscan() ? 0 : 7 * 1024),
    4 * (1024 >> ppu.interlace()),
    256 << hires,
    240 << ppu.interlace()
  );

  hires = false;
}

auto Video::scanline() -> void {
  uint y = cpu.vcounter();
  if(y >= 240) return;

  hires |= ppu.hires();
  uint width = (ppu.hires() == false ? 256 : 512);
  line_width[y] = width;
}

auto Video::init() -> void {
  hires = false;
  for(auto& n : line_width) n = 256;
}
