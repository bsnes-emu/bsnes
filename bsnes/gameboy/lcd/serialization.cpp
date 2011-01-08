#ifdef LCD_CPP

void LCD::serialize(serializer &s) {
  s.integer(status.lx);

  s.integer(status.display_enable);
  s.integer(status.window_tilemap_select);
  s.integer(status.window_display_enable);
  s.integer(status.bg_tiledata_select);
  s.integer(status.bg_tilemap_select);
  s.integer(status.obj_size);
  s.integer(status.obj_enable);
  s.integer(status.bg_enable);

  s.integer(status.interrupt_lyc);
  s.integer(status.interrupt_oam);
  s.integer(status.interrupt_vblank);
  s.integer(status.interrupt_hblank);

  s.integer(status.scy);
  s.integer(status.scx);
  s.integer(status.ly);
  s.integer(status.lyc);

  s.array(status.bgp);
  s.array(status.obp[0]);
  s.array(status.obp[1]);

  s.integer(status.wy);
  s.integer(status.wx);

  s.array(screen);
  s.array(vram);
  s.array(oam);
  s.array(line);
}

#endif
