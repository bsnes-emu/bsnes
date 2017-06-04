auto PPU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.array(vram, 96 * 1024);
  s.array(pram, 512);

  s.integer(io.gameBoyColorMode);
  s.integer(io.forceBlank);
  s.integer(io.greenSwap);
  s.integer(io.vblank);
  s.integer(io.hblank);
  s.integer(io.vcoincidence);
  s.integer(io.irqvblank);
  s.integer(io.irqhblank);
  s.integer(io.irqvcoincidence);
  s.integer(io.vcompare);
  s.integer(io.vcounter);

  s.integer(Background::IO::mode);
  s.integer(Background::IO::frame);
  s.integer(Background::IO::mosaicWidth);
  s.integer(Background::IO::mosaicHeight);
  bg0.serialize(s);
  bg1.serialize(s);
  bg2.serialize(s);
  bg3.serialize(s);
  objects.serialize(s);
  window0.serialize(s);
  window1.serialize(s);
  window2.serialize(s);
  window3.serialize(s);
  screen.serialize(s);
  for(auto& object : this->object) object.serialize(s);
  for(auto& param : this->objectParam) param.serialize(s);
}

auto PPU::Background::serialize(serializer& s) -> void {
  s.integer(id);

  s.integer(io.enable);
  s.integer(io.priority);
  s.integer(io.characterBase);
  s.integer(io.unused);
  s.integer(io.mosaic);
  s.integer(io.colorMode);
  s.integer(io.screenBase);
  s.integer(io.affineWrap);
  s.integer(io.screenSize);
  s.integer(io.hoffset);
  s.integer(io.voffset);
  s.integer(io.pa);
  s.integer(io.pb);
  s.integer(io.pc);
  s.integer(io.pd);
  s.integer(io.x);
  s.integer(io.y);
  s.integer(io.lx);
  s.integer(io.ly);

  s.integer(hmosaic);
  s.integer(vmosaic);
  s.integer(hoffset);
  s.integer(voffset);
  s.integer(fx);
  s.integer(fy);
}

auto PPU::Objects::serialize(serializer& s) -> void {
  s.integer(io.enable);
  s.integer(io.hblank);
  s.integer(io.mapping);
  s.integer(io.mosaicWidth);
  s.integer(io.mosaicHeight);
}

auto PPU::Window::serialize(serializer& s) -> void {
  s.integer(id);

  s.integer(io.enable);
  s.array(io.active);
  s.integer(io.x1);
  s.integer(io.x2);
  s.integer(io.y1);
  s.integer(io.y2);

  s.integer(output);
}

auto PPU::Screen::serialize(serializer& s) -> void {
  s.integer(io.blendMode);
  s.array(io.blendAbove);
  s.array(io.blendBelow);
  s.integer(io.blendEVA);
  s.integer(io.blendEVB);
  s.integer(io.blendEVY);
}

auto PPU::Object::serialize(serializer& s) -> void {
  s.integer(y);
  s.integer(affine);
  s.integer(affineSize);
  s.integer(mode);
  s.integer(mosaic);
  s.integer(colors);
  s.integer(shape);
  s.integer(x);
  s.integer(affineParam);
  s.integer(hflip);
  s.integer(vflip);
  s.integer(size);
  s.integer(character);
  s.integer(priority);
  s.integer(palette);
  s.integer(width);
  s.integer(height);
}

auto PPU::ObjectParam::serialize(serializer& s) -> void {
  s.integer(pa);
  s.integer(pb);
  s.integer(pc);
  s.integer(pd);
}
