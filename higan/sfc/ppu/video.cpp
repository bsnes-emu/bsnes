//note: this source file is currently unused
//saved temporarily only for reference

auto Video::refresh() -> void {
  auto output = this->output() + 16 * 512;  //add offset for overscan
  auto& palette = settings.colorEmulation ? paletteEmulation : paletteStandard;

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
  interface->videoRefresh(output - (ppu.overscan() ? 0 : 7 * 1024), 512 * sizeof(uint32), 512, 480);
}

auto Video::drawCursor(uint32 color, int x, int y) -> void {
  static const uint8 cursor[15 * 15] = {
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

  auto output = this->output() + 16 * 512;
  for(int cy = 0; cy < 15; cy++) {
    int vy = y + cy - 7;
    if(vy <= 0 || vy >= 240) continue;  //do not draw offscreen

    for(int cx = 0; cx < 15; cx++) {
      int vx = x + cx - 7;
      if(vx < 0 || vx >= 256) continue;  //do not draw offscreen
      uint8 pixel = cursor[cy * 15 + cx];
      if(pixel == 0) continue;
      uint32 pixelcolor = pixel == 1 ? (uint32)(255 << 24) : color;

      *(output + vy * 1024 + vx * 2 + 0) = pixelcolor;
      *(output + vy * 1024 + vx * 2 + 1) = pixelcolor;
      *(output + vy * 1024 + 512 + vx * 2 + 0) = pixelcolor;
      *(output + vy * 1024 + 512 + vx * 2 + 1) = pixelcolor;
    }
  }
}

auto Video::drawCursors() -> void {
  switch(settings.controllerPort2) {
  case Device::SuperScope:
    if(dynamic_cast<SuperScope*>(peripherals.controllerPort2)) {
      auto& controller = (SuperScope&)*peripherals.controllerPort2;
      drawCursor(0xff0000ff, controller.x, controller.y);
    }
    break;
  case Device::Justifier:
  case Device::Justifiers:
    if(dynamic_cast<Justifier*>(peripherals.controllerPort2)) {
      auto& controller = (Justifier&)*peripherals.controllerPort2;
      drawCursor(0xffff0000, controller.player1.x, controller.player1.y);
      if(!controller.chained) break;
      drawCursor(0xff00bf00, controller.player2.x, controller.player2.y);
    }
    break;
  }
}
