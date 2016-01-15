Video video;

Video::Video() {
  output = new uint32[512 * 512]();
  paletteStandard = new uint32[1 << 19];
  paletteEmulation = new uint32[1 << 19];

  output += 16 * 512;  //overscan padding
}

Video::~Video() {
  output -= 16 * 512;
  delete[] output;
  delete[] paletteStandard;
  delete[] paletteEmulation;
}

auto Video::reset() -> void {
  memory::fill(output, 512 * 480 * sizeof(uint32));  //padding area already cleared

  for(auto color : range(1 << 19)) {
    uint l = (uint4)(color >> 15);
    uint b = (uint5)(color >> 10);
    uint g = (uint5)(color >>  5);
    uint r = (uint5)(color >>  0);

    double L = (1.0 + l) / 16.0 * (l ? 1.0 : 0.5);

    { uint R = L * image::normalize(r, 5, 8);
      uint G = L * image::normalize(g, 5, 8);
      uint B = L * image::normalize(b, 5, 8);
      paletteStandard[color] = (255 << 24) | (R << 16) | (G << 8) | (B << 0);
    }

    { uint R = L * gamma_ramp[r];
      uint G = L * gamma_ramp[g];
      uint B = L * gamma_ramp[b];
      paletteEmulation[color] = (255 << 24) | (R << 16) | (G << 8) | (B << 0);
    }
  }

  for(auto color : range(1 << 19)) {
    uint l = (uint4)(color >> 15);
    uint b = (uint5)(color >> 10);
    uint g = (uint5)(color >>  5);
    uint r = (uint5)(color >>  0);
  }
}

auto Video::refresh() -> void {
  auto palette = settings.colorEmulation ? paletteEmulation : paletteStandard;

  if(settings.scanlineEmulation) {
    for(uint y = 0; y < 240; y++) {
      auto sourceLo = ppu.output + y * 1024;
      auto sourceHi = ppu.output + y * 1024 + 512;
      auto targetLo = output + y * 1024;
      auto targetHi = output + y * 1024 + 512;
      if(!ppu.interlace()) {
        for(uint x = 0; x < 512; x++) {
          auto color = palette[*sourceLo++];
          *targetLo++ = color;
          *targetHi++ = (255 << 24) | ((color & 0xfefefe) >> 1);
        }
      } else if(!ppu.field()) {
        for(uint x = 0; x < 512; x++) {
          auto color = palette[*sourceHi++];
          *targetLo++ = palette[*sourceLo++];
          *targetHi++ = (255 << 24) | ((color & 0xfefefe) >> 1);
        }
      } else {
        for(uint x = 0; x < 512; x++) {
          auto color = palette[*sourceLo++];
          *targetLo++ = (255 << 24) | ((color & 0xfefefe) >> 1);
          *targetHi++ = palette[*sourceHi++];
        }
      }
    }
  } else {
    for(uint y = 0; y < 240; y++) {
      auto sourceLo = ppu.output + y * 1024;
      auto sourceHi = ppu.output + y * 1024 + 512;
      auto targetLo = output + y * 1024;
      auto targetHi = output + y * 1024 + 512;
      if(!ppu.interlace()) {
        for(uint x = 0; x < 512; x++) {
          auto color = palette[*sourceLo++];
          *targetLo++ = color;
          *targetHi++ = color;
        }
      } else {
        for(uint x = 0; x < 512; x++) {
          *targetLo++ = palette[*sourceLo++];
          *targetHi++ = palette[*sourceHi++];
        }
      }
    }
  }

  if(settings.blurEmulation) {
    for(uint y = 0; y < 480; y++) {
      auto target = output + y * 512;
      for(uint x = 0; x < 512; x++) {
        auto a = target[x];
        auto b = target[x + (x != 511)];
        target[x] = (a + b - ((a ^ b) & 0x01010101)) >> 1;
      }
    }
  }

  drawCursors();

  #if defined(PROFILE_ACCURACY)
  interface->videoRefresh(output - (ppu.overscan() ? 0 : 7 * 1024), 512 * sizeof(uint32), 512, 480);
  #endif

  #if defined(PROFILE_BALANCED) || defined(PROFILE_PERFORMANCE)
  if(hires) {
    //normalize line widths
    auto data = (uint32*)output;
    if(ppu.interlace() && ppu.field()) data += 512;

    for(uint y = 0; y < 240; y++) {
      if(line_width[y] == 512) continue;
      uint32* buffer = data + y * 1024;
      for(int x = 255; x >= 0; x--) {
        buffer[(x * 2) + 0] = buffer[(x * 2) + 1] = buffer[x];
      }
    }
  }

  //overscan: when disabled, shift image down (by scrolling video buffer up) to center image onscreen
  //(memory before ppu.output is filled with black scanlines)
  interface->videoRefresh(
    output - (ppu.overscan() ? 0 : 7 * 1024),
    4 * (1024 >> ppu.interlace()),
    256 << hires,
    240 << ppu.interlace()
  );

  hires = false;
  #endif
}

//internal

auto Video::drawCursor(uint32 color, int x, int y) -> void {
  auto data = (uint32*)output;
  if(ppu.interlace() && ppu.field()) data += 512;

  for(int cy = 0; cy < 15; cy++) {
    int vy = y + cy - 7;
    if(vy <= 0 || vy >= 240) continue;  //do not draw offscreen

    bool hires = (line_width[vy] == 512);
    for(int cx = 0; cx < 15; cx++) {
      int vx = x + cx - 7;
      if(vx < 0 || vx >= 256) continue;  //do not draw offscreen
      uint8 pixel = cursor[cy * 15 + cx];
      if(pixel == 0) continue;
      uint32 pixelcolor = pixel == 1 ? 0xff000000 : color;

      if(!hires) {
        *(data + vy * 1024 + vx) = pixelcolor;
      } else {
        *(data + vy * 1024 + vx * 2 + 0) = pixelcolor;
        *(data + vy * 1024 + vx * 2 + 1) = pixelcolor;
        *(data + vy * 1024 + 512 + vx * 2 + 0) = pixelcolor;
        *(data + vy * 1024 + 512 + vx * 2 + 1) = pixelcolor;
      }
    }
  }
}

auto Video::drawCursors() -> void {
  switch(configuration.controllerPort2) {
  case Device::ID::SuperScope:
    if(dynamic_cast<SuperScope*>(device.controllerPort2)) {
      auto& controller = (SuperScope&)*device.controllerPort2;
      drawCursor(0xff0000ff, controller.x, controller.y);
    }
    break;
  case Device::ID::Justifier:
  case Device::ID::Justifiers:
    if(dynamic_cast<Justifier*>(device.controllerPort2)) {
      auto& controller = (Justifier&)*device.controllerPort2;
      drawCursor(0xffff0000, controller.player1.x, controller.player1.y);
      if(!controller.chained) break;
      drawCursor(0xff00bf00, controller.player2.x, controller.player2.y);
    }
    break;
  }
}

auto Video::scanline() -> void {
  uint y = cpu.vcounter();
  if(y >= 240) return;

  hires |= ppu.hires();
  uint width = ppu.hires() ? 512 : 256;
  line_width[y] = width;
}

auto Video::init() -> void {
  hires = false;
  for(auto& n : line_width) n = 256;
}

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
