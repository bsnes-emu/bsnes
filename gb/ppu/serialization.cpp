#ifdef PPU_CPP

void PPU::serialize(serializer& s) {
  Thread::serialize(s);

  s.array(vram);
  s.array(oam);
  s.array(bgp);
  s.array(obp[0]);
  s.array(obp[1]);
  s.array(bgpd);
  s.array(obpd);

  s.integer(status.lx);

  s.integer(status.display_enable);
  s.integer(status.window_tilemap_select);
  s.integer(status.window_display_enable);
  s.integer(status.bg_tiledata_select);
  s.integer(status.bg_tilemap_select);
  s.integer(status.ob_size);
  s.integer(status.ob_enable);
  s.integer(status.bg_enable);

  s.integer(status.interrupt_lyc);
  s.integer(status.interrupt_oam);
  s.integer(status.interrupt_vblank);
  s.integer(status.interrupt_hblank);

  s.integer(status.scy);
  s.integer(status.scx);

  s.integer(status.ly);
  s.integer(status.lyc);

  s.integer(status.wy);
  s.integer(status.wx);

  s.integer(status.vram_bank);

  s.integer(status.bgpi_increment);
  s.integer(status.bgpi);

  s.integer(status.obpi_increment);
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

#endif
