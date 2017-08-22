auto VDP::serialize(serializer& s) -> void {
  Thread::serialize(s);

  dma.serialize(s);
  planeA.serialize(s);
  window.serialize(s);
  planeB.serialize(s);
  sprite.serialize(s);

  vram.serialize(s);
  vsram.serialize(s);
  cram.serialize(s);

  s.integer(io.command);
  s.integer(io.address);
  s.integer(io.commandPending);
  s.integer(io.displayOverlayEnable);
  s.integer(io.counterLatch);
  s.integer(io.horizontalBlankInterruptEnable);
  s.integer(io.leftColumnBlank);
  s.integer(io.videoMode);
  s.integer(io.overscan);
  s.integer(io.verticalBlankInterruptEnable);
  s.integer(io.displayEnable);
  s.integer(io.externalVRAM);
  s.integer(io.backgroundColor);
  s.integer(io.horizontalInterruptCounter);
  s.integer(io.externalInterruptEnable);
  s.integer(io.displayWidth);
  s.integer(io.interlaceMode);
  s.integer(io.shadowHighlightEnable);
  s.integer(io.externalColorEnable);
  s.integer(io.horizontalSync);
  s.integer(io.verticalSync);
  s.integer(io.nametableBasePatternA);
  s.integer(io.nametableBasePatternB);
  s.integer(io.dataIncrement);

  s.integer(latch.overscan);
  s.integer(latch.horizontalInterruptCounter);
  s.integer(latch.displayWidth);

  s.integer(state.hdot);
  s.integer(state.hcounter);
  s.integer(state.vcounter);
  s.integer(state.field);
}

auto VDP::DMA::serialize(serializer& s) -> void {
  s.integer(io.mode);
  s.integer(io.source);
  s.integer(io.length);
  s.integer(io.fill);
  s.integer(io.enable);
  s.integer(io.wait);
}

auto VDP::Background::serialize(serializer& s) -> void {
  s.integer(io.nametableAddress);
  s.integer(io.nametableWidth);
  s.integer(io.nametableHeight);
  s.integer(io.horizontalScrollAddress);
  s.integer(io.horizontalScrollMode);
  s.integer(io.verticalScrollMode);
  s.integer(io.horizontalDirection);
  s.integer(io.horizontalOffset);
  s.integer(io.verticalDirection);
  s.integer(io.verticalOffset);

  s.integer(state.horizontalScroll);
  s.integer(state.verticalScroll);

  s.integer(output.color);
  s.integer(output.priority);
}

auto VDP::Object::serialize(serializer& s) -> void {
  s.integer(x);
  s.integer(y);
  s.integer(tileWidth);
  s.integer(tileHeight);
  s.integer(horizontalFlip);
  s.integer(verticalFlip);
  s.integer(palette);
  s.integer(priority);
  s.integer(address);
  s.integer(link);
}

auto VDP::Sprite::serialize(serializer& s) -> void {
  s.integer(io.attributeAddress);
  s.integer(io.nametableAddressBase);

  s.integer(output.color);
  s.integer(output.priority);

  for(uint n : range(80)) oam[n].serialize(s);
  for(uint n : range(20)) objects[n].serialize(s);
}

auto VDP::VRAM::serialize(serializer& s) -> void {
  s.array(memory);
}

auto VDP::VSRAM::serialize(serializer& s) -> void {
  s.array(memory);
}

auto VDP::CRAM::serialize(serializer& s) -> void {
  s.array(memory);
}
