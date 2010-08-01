#ifdef SPPU_CPP

void sPPU::serialize(serializer &s) {
  PPU::serialize(s);

  s.integer(display.interlace);
  s.integer(display.overscan);

  s.integer(regs.ppu1_mdr);
  s.integer(regs.ppu2_mdr);

  s.integer(regs.vram_readbuffer);
  s.integer(regs.oam_latchdata);
  s.integer(regs.cgram_latchdata);
  s.integer(regs.bgofs_latchdata);
  s.integer(regs.mode7_latchdata);
  s.integer(regs.counters_latched);
  s.integer(regs.latch_hcounter);
  s.integer(regs.latch_vcounter);

  s.integer(regs.ioamaddr);
  s.integer(regs.icgramaddr);

  s.integer(regs.display_disabled);
  s.integer(regs.display_brightness);

  s.integer(regs.oam_baseaddr);
  s.integer(regs.oam_addr);
  s.integer(regs.oam_priority);

  s.integer(regs.bg3_priority);
  s.integer(regs.bgmode);

  s.integer(regs.mode7_hoffset);
  s.integer(regs.mode7_voffset);

  s.integer(regs.vram_incmode);
  s.integer(regs.vram_mapping);
  s.integer(regs.vram_incsize);

  s.integer(regs.vram_addr);

  s.integer(regs.mode7_repeat);
  s.integer(regs.mode7_vflip);
  s.integer(regs.mode7_hflip);

  s.integer(regs.m7a);
  s.integer(regs.m7b);
  s.integer(regs.m7c);
  s.integer(regs.m7d);
  s.integer(regs.m7x);
  s.integer(regs.m7y);

  s.integer(regs.cgram_addr);

  s.integer(regs.mode7_extbg);
  s.integer(regs.pseudo_hires);
  s.integer(regs.overscan);
  s.integer(regs.interlace);

  s.integer(regs.hcounter);
  s.integer(regs.vcounter);

  bg1.serialize(s);
  bg2.serialize(s);
  bg3.serialize(s);
  bg4.serialize(s);
  oam.serialize(s);
  window.serialize(s);
  screen.serialize(s);
}

void sPPU::Background::serialize(serializer &s) {
  s.integer(id);

  s.integer(t.x);
  s.integer(t.mosaic_y);
  s.integer(t.mosaic_countdown);

  s.integer(regs.tiledata_addr);
  s.integer(regs.screen_addr);
  s.integer(regs.screen_size);
  s.integer(regs.mosaic);
  s.integer(regs.tile_size);

  s.integer(regs.mode);
  s.integer(regs.priority0);
  s.integer(regs.priority1);

  s.integer(regs.main_enabled);
  s.integer(regs.sub_enabled);

  s.integer(regs.hoffset);
  s.integer(regs.voffset);

  s.integer(output.main.priority);
  s.integer(output.main.palette);
  s.integer(output.main.tile);

  s.integer(output.sub.priority);
  s.integer(output.sub.palette);
  s.integer(output.sub.tile);
}

void sPPU::Sprite::serialize(serializer &s) {
  for(unsigned i = 0; i < 128; i++) {
    s.integer(list[i].width);
    s.integer(list[i].height);
    s.integer(list[i].x);
    s.integer(list[i].y);
    s.integer(list[i].character);
    s.integer(list[i].nameselect);
    s.integer(list[i].vflip);
    s.integer(list[i].hflip);
    s.integer(list[i].priority);
    s.integer(list[i].palette);
  }

  s.integer(t.x);
  s.integer(t.y);
  s.integer(t.item_count);
  s.integer(t.tile_count);
  s.array(t.output_palette);
  s.array(t.output_priority);
  s.array(t.item_list);
  for(unsigned i = 0; i < 34; i++) {
    s.integer(t.tile_list[i].x);
    s.integer(t.tile_list[i].y);
    s.integer(t.tile_list[i].priority);
    s.integer(t.tile_list[i].palette);
    s.integer(t.tile_list[i].tile);
    s.integer(t.tile_list[i].hflip);
  }
  s.integer(t.active_sprite);

  s.integer(regs.main_enabled);
  s.integer(regs.sub_enabled);
  s.integer(regs.interlace);

  s.integer(regs.base_size);
  s.integer(regs.nameselect);
  s.integer(regs.tiledata_addr);
  s.integer(regs.first_sprite);

  s.integer(regs.priority0);
  s.integer(regs.priority1);
  s.integer(regs.priority2);
  s.integer(regs.priority3);

  s.integer(regs.time_over);
  s.integer(regs.range_over);

  s.integer(output.main.priority);
  s.integer(output.main.palette);

  s.integer(output.sub.priority);
  s.integer(output.sub.palette);
}

void sPPU::Window::serialize(serializer &s) {
  s.integer(t.x);

  s.integer(regs.bg1_one_enable);
  s.integer(regs.bg1_one_invert);
  s.integer(regs.bg1_two_enable);
  s.integer(regs.bg1_two_invert);

  s.integer(regs.bg2_one_enable);
  s.integer(regs.bg2_one_invert);
  s.integer(regs.bg2_two_enable);
  s.integer(regs.bg2_two_invert);

  s.integer(regs.bg3_one_enable);
  s.integer(regs.bg3_one_invert);
  s.integer(regs.bg3_two_enable);
  s.integer(regs.bg3_two_invert);

  s.integer(regs.bg4_one_enable);
  s.integer(regs.bg4_one_invert);
  s.integer(regs.bg4_two_enable);
  s.integer(regs.bg4_two_invert);

  s.integer(regs.oam_one_enable);
  s.integer(regs.oam_one_invert);
  s.integer(regs.oam_two_enable);
  s.integer(regs.oam_two_invert);

  s.integer(regs.col_one_enable);
  s.integer(regs.col_one_invert);
  s.integer(regs.col_two_enable);
  s.integer(regs.col_two_invert);

  s.integer(regs.one_left);
  s.integer(regs.one_right);
  s.integer(regs.two_left);
  s.integer(regs.two_right);

  s.integer(regs.bg1_mask);
  s.integer(regs.bg2_mask);
  s.integer(regs.bg3_mask);
  s.integer(regs.bg4_mask);
  s.integer(regs.oam_mask);
  s.integer(regs.col_mask);

  s.integer(regs.bg1_main_enable);
  s.integer(regs.bg1_sub_enable);
  s.integer(regs.bg2_main_enable);
  s.integer(regs.bg2_sub_enable);
  s.integer(regs.bg3_main_enable);
  s.integer(regs.bg3_sub_enable);
  s.integer(regs.bg4_main_enable);
  s.integer(regs.bg4_sub_enable);
  s.integer(regs.oam_main_enable);
  s.integer(regs.oam_sub_enable);

  s.integer(regs.col_main_mask);
  s.integer(regs.col_sub_mask);

  s.integer(output.main.color_enable);
  s.integer(output.sub.color_enable);

}

void sPPU::Screen::serialize(serializer &s) {
  s.integer(regs.addsub_mode);
  s.integer(regs.direct_color);

  s.integer(regs.color_mode);
  s.integer(regs.color_halve);
  s.integer(regs.bg1_color_enable);
  s.integer(regs.bg2_color_enable);
  s.integer(regs.bg3_color_enable);
  s.integer(regs.bg4_color_enable);
  s.integer(regs.oam_color_enable);
  s.integer(regs.back_color_enable);

  s.integer(regs.color_b);
  s.integer(regs.color_g);
  s.integer(regs.color_r);
}

#endif
