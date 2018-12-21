auto VDP::serialize(serializer& s) -> void {
  Thread::serialize(s);

  background.serialize(s);
  sprite.serialize(s);

  s.array(vram);
  s.array(cram);

  s.integer(io.vcounter);
  s.integer(io.hcounter);
  s.integer(io.lcounter);
  s.integer(io.intLine);
  s.integer(io.intFrame);
  s.integer(io.spriteOverflow);
  s.integer(io.spriteCollision);
  s.integer(io.fifthSprite);
  s.integer(io.controlLatch);
  s.integer(io.controlData);
  s.integer(io.code);
  s.integer(io.address);
  s.integer(io.vramLatch);
  s.integer(io.externalSync);
  s.integer(io.spriteShift);
  s.integer(io.lineInterrupts);
  s.integer(io.leftClip);
  s.integer(io.horizontalScrollLock);
  s.integer(io.verticalScrollLock);
  s.integer(io.spriteDouble);
  s.integer(io.spriteTile);
  s.integer(io.frameInterrupts);
  s.integer(io.displayEnable);
  s.integer(io.mode);
  s.integer(io.nameTableAddress);
  s.integer(io.colorTableAddress);
  s.integer(io.patternTableAddress);
  s.integer(io.spriteAttributeTableAddress);
  s.integer(io.spritePatternTableAddress);
  s.integer(io.backdropColor);
  s.integer(io.hscroll);
  s.integer(io.vscroll);
  s.integer(io.lineCounter);
}

auto VDP::Background::serialize(serializer& s) -> void {
  s.integer(output.color);
  s.integer(output.palette);
  s.integer(output.priority);
}

auto VDP::Sprite::serialize(serializer& s) -> void {
  s.integer(output.color);
  for(auto& object : objects) {
    s.integer(object.x);
    s.integer(object.y);
    s.integer(object.pattern);
    s.integer(object.color);
  }
  s.integer(objectsValid);
}
