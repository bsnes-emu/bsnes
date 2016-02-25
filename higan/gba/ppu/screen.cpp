auto PPU::render_forceblank() -> void {
  uint32* line = output + regs.vcounter * 240;
  for(auto x : range(240)) line[x] = 0x7fff;
}

auto PPU::render_screen() -> void {
  uint32* line = output + regs.vcounter * 240;

  if(regs.bg[0].control.mosaic) render_mosaic_background(BG0);
  if(regs.bg[1].control.mosaic) render_mosaic_background(BG1);
  if(regs.bg[2].control.mosaic) render_mosaic_background(BG2);
  if(regs.bg[3].control.mosaic) render_mosaic_background(BG3);
  render_mosaic_object();

  for(auto x : range(240)) {
    Registers::WindowFlags flags;
    flags.enable[BG0] = true;  //enable all layers if no windows are enabled
    flags.enable[BG1] = true;
    flags.enable[BG2] = true;
    flags.enable[BG3] = true;
    flags.enable[OBJ] = true;
    flags.enable[SFX] = true;

    //determine active window
    if(regs.control.enablewindow[In0] || regs.control.enablewindow[In1] || regs.control.enablewindow[Obj]) {
      flags = regs.windowflags[Out];
      if(regs.control.enablewindow[Obj] && windowmask[Obj][x]) flags = regs.windowflags[Obj];
      if(regs.control.enablewindow[In1] && windowmask[In1][x]) flags = regs.windowflags[In1];
      if(regs.control.enablewindow[In0] && windowmask[In0][x]) flags = regs.windowflags[In0];
    }

    //priority sorting: find topmost two pixels
    uint a = 5, b = 5;
    for(int p = 3; p >= 0; p--) {
      for(int l = 5; l >= 0; l--) {
        if(layer[l][x].enable && layer[l][x].priority == p && flags.enable[l]) {
          b = a;
          a = l;
        }
      }
    }

    auto& above = layer[a];
    auto& below = layer[b];
    bool blendabove = regs.blend.control.above[a];
    bool blendbelow = regs.blend.control.below[b];
    uint color = above[x].color;
    auto eva = min(16u, (uint)regs.blend.eva);
    auto evb = min(16u, (uint)regs.blend.evb);
    auto evy = min(16u, (uint)regs.blend.evy);

    //perform blending, if needed
    if(flags.enable[SFX] == false) {
    } else if(above[x].translucent && blendbelow) {
      color = blend(above[x].color, eva, below[x].color, evb);
    } else if(regs.blend.control.mode == 1 && blendabove && blendbelow) {
      color = blend(above[x].color, eva, below[x].color, evb);
    } else if(regs.blend.control.mode == 2 && blendabove) {
      color = blend(above[x].color, 16 - evy, 0x7fff, evy);
    } else if(regs.blend.control.mode == 3 && blendabove) {
      color = blend(above[x].color, 16 - evy, 0x0000, evy);
    }

    //output pixel
    line[x] = color;
  }
}

auto PPU::render_window(uint w) -> void {
  uint y = regs.vcounter;

  uint y1 = regs.window[w].y1, y2 = regs.window[w].y2;
  uint x1 = regs.window[w].x1, x2 = regs.window[w].x2;

  if(y2 < y1 || y2 > 160) y2 = 160;
  if(x2 < x1 || x2 > 240) x2 = 240;

  if(y >= y1 && y < y2) {
    for(uint x = x1; x < x2; x++) {
      windowmask[w][x] = true;
    }
  }
}

auto PPU::blend(uint above, uint eva, uint below, uint evb) -> uint {
  uint5 ar = above >> 0, ag = above >> 5, ab = above >> 10;
  uint5 br = below >> 0, bg = below >> 5, bb = below >> 10;

  uint r = (ar * eva + br * evb) >> 4;
  uint g = (ag * eva + bg * evb) >> 4;
  uint b = (ab * eva + bb * evb) >> 4;

  return min(31, r) << 0 | min(31, g) << 5 | min(31, b) << 10;
}
