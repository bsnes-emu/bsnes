#ifdef SYSTEM_CPP

Video video;

const uint8_t Video::cursor[15 * 15] = {
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

void Video::draw_cursor(uint16_t color, int x, int y) {
  uint16_t *data = (uint16_t*)ppu.output;
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
      uint16_t pixelcolor = (pixel == 1) ? 0 : color;

      if(hires == false) {
        *((uint16_t*)data + vy * 1024 + vx) = pixelcolor;
      } else {
        *((uint16_t*)data + vy * 1024 + vx * 2 + 0) = pixelcolor;
        *((uint16_t*)data + vy * 1024 + vx * 2 + 1) = pixelcolor;
      }
    }
  }
}

void Video::update() {
  switch(input.port[1].device) {
    case Input::Device::SuperScope: draw_cursor(0x001f, input.port[1].superscope.x, input.port[1].superscope.y); break;
    case Input::Device::Justifiers: draw_cursor(0x02e0, input.port[1].justifier.x2, input.port[1].justifier.y2); //fallthrough
    case Input::Device::Justifier:  draw_cursor(0x001f, input.port[1].justifier.x1, input.port[1].justifier.y1); break;
  }

  uint16_t *data = (uint16_t*)ppu.output;
  if(ppu.interlace() && ppu.field()) data += 512;

  if(hires) {
    //normalize line widths
    for(unsigned y = 0; y < 240; y++) {
      if(line_width[y] == 512) continue;
      uint16_t *buffer = data + y * 1024;
      for(signed x = 255; x >= 0; x--) {
        buffer[(x * 2) + 0] = buffer[(x * 2) + 1] = buffer[x];
      }
    }
  }

  system.interface->video_refresh(ppu.surface, hires, ppu.interlace(), ppu.overscan());

  hires = false;
}

void Video::scanline() {
  unsigned y = cpu.vcounter();
  if(y >= 240) return;

  hires |= ppu.hires();
  unsigned width = (ppu.hires() == false ? 256 : 512);
  line_width[y] = width;
}

void Video::init() {
  hires = false;
  for(unsigned i = 0; i < 240; i++) line_width[i] = 256;
}

#endif
