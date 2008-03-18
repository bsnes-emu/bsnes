#ifdef SNES_CPP

void SNES::Video::set_mode(Mode mode_) {
  mode = mode_;
}

void SNES::Video::update() {
  uint16_t *data = (uint16_t*)ppu.output;
  unsigned width, height;

  switch(mode) { default:
    case ModeNTSC: {
      width  = 256;
      height = 224;
      data  += ((int)ppu.overscan() << 13) + 1024;
    } break;
    case ModePAL: {
      width  = 256;
      height = 239;
      data  += 1024;
      break;
    }
  }

  if(frame_hires) width <<= 1;
  if(frame_interlace) height <<= 1;

  snesinterface.video_refresh(
    data,
    /* pitch = */ height <= 240 ? 2048 : 1024,
    /* *line = */ height <= 240 ? (pline_width + 1) : (iline_width + 2),
    width, height
  );

  frame_hires = false;
  frame_interlace = false;
}

void SNES::Video::scanline() {
  int y = cpu.vcounter();
  int o = (mode == ModeNTSC) ? ((int)ppu.overscan() << 3) : 0;
  if(y <= (0 + o) || y >= (225 + o)) return;
  y -= o;

  pline_width[y] = iline_width[y * 2 + (int)ppu.field()] = (ppu.hires() == false) ? 256 : 512;
  frame_hires |= ppu.hires();
  frame_interlace |= ppu.interlace();
}

void SNES::Video::init() {
  for(unsigned i = 0; i < 240; i++) pline_width[i] = 256;
  for(unsigned i = 0; i < 480; i++) iline_width[i] = 256;
  frame_hires = false;
  frame_interlace = false;
  set_mode(ModeNTSC);
}

#endif //ifdef SNES_CPP
