auto PPU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.array(vram);
  s.array(oam);
  s.array(bgp);
  s.array(obp[0]);
  s.array(obp[1]);
  s.array(bgpd);
  s.array(obpd);

  s.integer(status.irq);
  s.integer(status.lx);

  s.integer(status.displayEnable);
  s.integer(status.windowTilemapSelect);
  s.integer(status.windowDisplayEnable);
  s.integer(status.bgTiledataSelect);
  s.integer(status.bgTilemapSelect);
  s.integer(status.obSize);
  s.integer(status.obEnable);
  s.integer(status.bgEnable);

  s.integer(status.interruptLYC);
  s.integer(status.interruptOAM);
  s.integer(status.interruptVblank);
  s.integer(status.interruptHblank);
  s.integer(status.mode);

  s.integer(status.scy);
  s.integer(status.scx);

  s.integer(status.ly);
  s.integer(status.lyc);

  s.integer(status.dmaActive);
  s.integer(status.dmaClock);
  s.integer(status.dmaBank);

  s.integer(status.wy);
  s.integer(status.wx);

  s.integer(status.vramBank);

  s.integer(status.bgpiIncrement);
  s.integer(status.bgpi);

  s.integer(status.obpiIncrement);
  s.integer(status.obpi);

  s.array(screen);

  s.integer(bg.color);
  s.integer(bg.palette);
  s.integer(bg.priority);

  s.integer(ob.color);
  s.integer(ob.palette);
  s.integer(ob.priority);

  for(auto& o : sprite) {
    s.integer(o.x);
    s.integer(o.y);
    s.integer(o.tile);
    s.integer(o.attr);
  }
  s.integer(sprites);

  s.integer(background.attr);
  s.integer(background.data);

  s.integer(window.attr);
  s.integer(window.data);
}
