auto PPU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(io.mdr);

  s.integer(io.field);
  s.integer(io.lx);
  s.integer(io.ly);

  s.integer(io.busData);

  s.integer(io.v.value);
  s.integer(io.t.value);

  s.integer(io.nmiHold);
  s.integer(io.nmiFlag);

  s.integer(io.vramIncrement);
  s.integer(io.spriteAddress);
  s.integer(io.bgAddress);
  s.integer(io.spriteHeight);
  s.integer(io.masterSelect);
  s.integer(io.nmiEnable);

  s.integer(io.grayscale);
  s.integer(io.bgEdgeEnable);
  s.integer(io.spriteEdgeEnable);
  s.integer(io.bgEnable);
  s.integer(io.spriteEnable);
  s.integer(io.emphasis);

  s.integer(io.spriteOverflow);
  s.integer(io.spriteZeroHit);

  s.integer(io.oamAddress);

  s.integer(latch.nametable);
  s.integer(latch.attribute);
  s.integer(latch.tiledataLo);
  s.integer(latch.tiledataHi);

  s.integer(latch.oamIterator);
  s.integer(latch.oamCounter);

  for(auto& o : latch.oam) o.serialize(s);
  for(auto& o : latch.soam) o.serialize(s);

  s.array(ciram);
  s.array(cgram);
  s.array(oam);

  s.array(buffer);
}

auto PPU::OAM::serialize(serializer& s) -> void {
  s.integer(id);
  s.integer(y);
  s.integer(tile);
  s.integer(attr);
  s.integer(x);

  s.integer(tiledataLo);
  s.integer(tiledataHi);
}
