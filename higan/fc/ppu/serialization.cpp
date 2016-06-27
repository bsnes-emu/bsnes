auto PPU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(r.mdr);

  s.integer(r.field);
  s.integer(r.lx);
  s.integer(r.ly);

  s.integer(r.busData);

  s.integer(r.v.value);
  s.integer(r.t.value);

  s.integer(r.nmiHold);
  s.integer(r.nmiFlag);

  s.integer(r.vramIncrement);
  s.integer(r.spriteAddress);
  s.integer(r.bgAddress);
  s.integer(r.spriteHeight);
  s.integer(r.masterSelect);
  s.integer(r.nmiEnable);

  s.integer(r.grayscale);
  s.integer(r.bgEdgeEnable);
  s.integer(r.spriteEdgeEnable);
  s.integer(r.bgEnable);
  s.integer(r.spriteEnable);
  s.integer(r.emphasis);

  s.integer(r.spriteOverflow);
  s.integer(r.spriteZeroHit);

  s.integer(r.oamAddress);

  s.integer(l.nametable);
  s.integer(l.attribute);
  s.integer(l.tiledataLo);
  s.integer(l.tiledataHi);

  s.integer(l.oamIterator);
  s.integer(l.oamCounter);

  for(auto& o : l.oam) o.serialize(s);
  for(auto& o : l.soam) o.serialize(s);

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
