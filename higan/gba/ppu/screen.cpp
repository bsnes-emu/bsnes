/*
auto PPU::renderScreen() -> void {
  uint32* line = output + regs.vcounter * 240;

  if(bg0.io.mosaic) renderMosaicBackground(BG0);
  if(bg1.io.mosaic) renderMosaicBackground(BG1);
  if(bg2.io.mosaic) renderMosaicBackground(BG2);
  if(bg3.io.mosaic) renderMosaicBackground(BG3);
  renderMosaicObject();

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
*/

auto PPU::Screen::run(uint x, uint y) -> uint15 {
  if(ppu.blank()) return 0x7fff;

  //determine active window
  uint1 active[6] = {true, true, true, true, true, true};  //enable all layers if no windows are enabled
  if(ppu.window0.io.enable || ppu.window1.io.enable || ppu.window2.io.enable) {
    memory::copy(&active, &ppu.window3.io.active, sizeof(active));
    if(ppu.window2.io.enable && ppu.window2.output) memory::copy(&active, &ppu.window2.io.active, sizeof(active));
    if(ppu.window1.io.enable && ppu.window1.output) memory::copy(&active, &ppu.window1.io.active, sizeof(active));
    if(ppu.window0.io.enable && ppu.window0.output) memory::copy(&active, &ppu.window0.io.active, sizeof(active));
  }

  //priority sorting: find topmost two pixels
  Pixel layers[6] = {
    ppu.objects.output,
    ppu.bg0.output,
    ppu.bg1.output,
    ppu.bg2.output,
    ppu.bg3.output,
    {true, 3, ppu.pram[0]},
  };

  uint aboveLayer = 5, belowLayer = 5;
  for(int priority = 3; priority >= 0; priority--) {
    for(int layer = 5; layer >= 0; layer--) {
      if(layers[layer].enable && layers[layer].priority == priority && active[layer]) {
        belowLayer = aboveLayer;
        aboveLayer = layer;
      }
    }
  }

  auto above = layers[aboveLayer];
  auto below = layers[belowLayer];
  auto eva = min(16u, (uint)io.blendEVA);
  auto evb = min(16u, (uint)io.blendEVB);
  auto evy = min(16u, (uint)io.blendEVY);
  uint15 color = above.color;

  //color blending
  if(active[SFX]) {
    if(above.translucent && io.blendBelow[belowLayer]) {
      color = blend(above.color, eva, below.color, evb);
    } else if(io.blendMode == 1 && io.blendAbove[aboveLayer] && io.blendBelow[belowLayer]) {
      color = blend(above.color, eva, below.color, evb);
    } else if(io.blendMode == 2 && io.blendAbove[aboveLayer]) {
      color = blend(above.color, 16 - evy, 0x7fff, evy);
    } else if(io.blendMode == 3 && io.blendAbove[aboveLayer]) {
      color = blend(above.color, 16 - evy, 0x0000, evy);
    }
  }

  return color;
}

auto PPU::Screen::blend(uint15 above, uint eva, uint15 below, uint evb) -> uint15 {
  uint5 ar = above >> 0, ag = above >> 5, ab = above >> 10;
  uint5 br = below >> 0, bg = below >> 5, bb = below >> 10;

  uint r = (ar * eva + br * evb) >> 4;
  uint g = (ag * eva + bg * evb) >> 4;
  uint b = (ab * eva + bb * evb) >> 4;

  return min(31u, r) << 0 | min(31u, g) << 5 | min(31u, b) << 10;
}

auto PPU::Screen::power() -> void {
  memory::fill(&io, sizeof(IO));
}
