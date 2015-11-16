auto PPU::render_mosaic_background(uint id) -> void {
  if(regs.mosaic.bghsize == 0) return;
  uint width = 1 + regs.mosaic.bghsize;
  auto& buffer = layer[id];

  for(uint x = 0; x < 240;) {
    for(uint m = 1; m < width; m++) {
      if(x + m >= 240) break;
      buffer[x + m] = buffer[x];
    }
    x += width;
  }
}

auto PPU::render_mosaic_object() -> void {
  if(regs.mosaic.objhsize == 0) return;
  uint width = 1 + regs.mosaic.objhsize;
  auto& buffer = layer[OBJ];

  Pixel mosaicPixel;
  mosaicPixel.mosaic = false;
  uint counter = 0;

  for(auto x : range(240)) {
    if(counter == width || mosaicPixel.mosaic == false) {
      mosaicPixel = buffer[x];
      if(counter == width) counter = 0;
    } else {
      if(buffer[x].mosaic) buffer[x] = mosaicPixel;
    }
    counter++;
  }
}
