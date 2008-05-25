bool MainWindow::input_ready() {
  //allow input if main window has focus
  if(focused() == true) return true;
  //allow input if config set to never block input
  if(config::input.capture_mode == 0) return true;
  //block input
  return false;
}

uintptr_t MainWindow::close(Event) {
  event::quit();
  return false;
}

uintptr_t MainWindow::event(Event e) {
  if(e.type == Event::Tick) {
    if(e.widget == &menu_file_load) {
      event::load_rom();
    }

    if(e.widget == &menu_file_load_bsx) {
      window_bsxloader.mode = BSXLoaderWindow::ModeBSX;
      window_bsxloader.set_text(translate["Load BS-X Cartridge"]);
      window_bsxloader.tbase.set_text(config::path.bsx);
      window_bsxloader.focus();
    }

    if(e.widget == &menu_file_load_bsc) {
      window_bsxloader.mode = BSXLoaderWindow::ModeBSC;
      window_bsxloader.set_text(translate["Load BS-X Slotted Cartridge"]);
      window_bsxloader.tbase.set_text("");
      window_bsxloader.focus();
    }

    if(e.widget == &menu_file_load_st) {
      window_stloader.tbase.set_text(config::path.st);
      window_stloader.focus();
    }

    if(e.widget == &menu_file_unload) {
      event::unload_rom();
    }

    if(e.widget == &menu_file_reset) {
      event::reset();
    }

    if(e.widget == &menu_file_power) {
      event::power();
    }

    if(e.widget == &menu_file_exit) {
      event(Event(Event::Close));
    }

    if(e.widget == &menu_settings_videomode_1x) { event::update_multiplier(1); }
    if(e.widget == &menu_settings_videomode_2x) { event::update_multiplier(2); }
    if(e.widget == &menu_settings_videomode_3x) { event::update_multiplier(3); }
    if(e.widget == &menu_settings_videomode_4x) { event::update_multiplier(4); }
    if(e.widget == &menu_settings_videomode_5x) { event::update_multiplier(5); }

    if(e.widget == &menu_settings_videomode_aspect_correction) {
      event::update_aspect_correction(menu_settings_videomode_aspect_correction.checked());
    }

    if(e.widget == &menu_settings_videomode_ntsc) { event::update_region(0); }
    if(e.widget == &menu_settings_videomode_pal)  { event::update_region(1); }

    if(e.widget == &menu_settings_videofilter_hwpoint)  { event::update_hardware_filter(0); }
    if(e.widget == &menu_settings_videofilter_hwlinear) { event::update_hardware_filter(1); }

    if(e.widget == &menu_settings_videofilter_swnone)     { event::update_software_filter(0); }
    if(e.widget == &menu_settings_videofilter_swscanline) { event::update_software_filter(1); }
    if(e.widget == &menu_settings_videofilter_swscale2x)  { event::update_software_filter(2); }
    if(e.widget == &menu_settings_videofilter_swhq2x)     { event::update_software_filter(3); }
    if(e.widget == &menu_settings_videofilter_swntsc)     { event::update_software_filter(4); }

    if(e.widget == &menu_settings_videoframeskip_0) { event::update_frameskip(0); }
    if(e.widget == &menu_settings_videoframeskip_1) { event::update_frameskip(1); }
    if(e.widget == &menu_settings_videoframeskip_2) { event::update_frameskip(2); }
    if(e.widget == &menu_settings_videoframeskip_3) { event::update_frameskip(3); }
    if(e.widget == &menu_settings_videoframeskip_4) { event::update_frameskip(4); }
    if(e.widget == &menu_settings_videoframeskip_5) { event::update_frameskip(5); }
    if(e.widget == &menu_settings_videoframeskip_6) { event::update_frameskip(6); }
    if(e.widget == &menu_settings_videoframeskip_7) { event::update_frameskip(7); }
    if(e.widget == &menu_settings_videoframeskip_8) { event::update_frameskip(8); }
    if(e.widget == &menu_settings_videoframeskip_9) { event::update_frameskip(9); }

    if(e.widget == &menu_settings_mute) {
      config::audio.mute = menu_settings_mute.checked();
    }

    if(e.widget == &menu_settings_emuspeed_slowest)  { event::update_emulation_speed(0); }
    if(e.widget == &menu_settings_emuspeed_slow)     { event::update_emulation_speed(1); }
    if(e.widget == &menu_settings_emuspeed_normal)   { event::update_emulation_speed(2); }
    if(e.widget == &menu_settings_emuspeed_fast)     { event::update_emulation_speed(3); }
    if(e.widget == &menu_settings_emuspeed_fastest)  { event::update_emulation_speed(4); }
    if(e.widget == &menu_settings_emuspeed_disabled) { event::update_emulation_speed(5); }

    if(e.widget == &menu_settings_config) { window_settings.show(); }

    if(e.widget == &menu_misc_logaudio) {
      (menu_misc_logaudio.checked() == true) ? snes.audio.log_enable() : snes.audio.log_disable();
    }

    if(e.widget == &menu_misc_showstatus) {
      status.show(config::misc.status_enable = menu_misc_showstatus.checked());
    }

    if(e.widget == &menu_misc_about) {
      window_about.focus();
    }
  }

  return true;
}

uintptr_t MainWindow::block(Event) {
  audio.clear();
  return true;
}

void MainWindow::setup() {
  create(Window::AutoCenter, 256, 224, BSNES_TITLE);
  set_background_color(0, 0, 0);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  MenuRadioItemGroup group;
  attach(menu_file.create(translate["System"]));
    menu_file.attach(menu_file_load.create(string() << translate["Load Cartridge"] << " ..."));
    menu_file.attach(menu_file_load_special.create(translate["Load Special"]));
      menu_file_load_special.attach(menu_file_load_bsx.create(string() << translate["Load BS-X Cartridge"] << " ..."));
      menu_file_load_special.attach(menu_file_load_bsc.create(string() << translate["Load BS-X Slotted Cartridge"] << " ..."));
      menu_file_load_special.attach(menu_file_load_st.create(string() << translate["Load Sufami Turbo Cartridge"] << " ..."));
    menu_file.attach(menu_file_unload.create(translate["Unload Cartridge"]));
    menu_file.attach(menu_file_sep1.create());
    menu_file.attach(menu_file_reset.create(translate["Reset"]));
    menu_file_power.create(translate["Power Cycle"]);
    if(config::advanced.enable) menu_file.attach(menu_file_power);
    menu_file.attach(menu_file_sep2.create());
    menu_file.attach(menu_file_exit.create(translate["Exit"]));

  attach(menu_settings.create(translate["Settings"]));
    menu_settings.attach(menu_settings_videomode.create(translate["Video Mode"]));
      group.add(&menu_settings_videomode_1x);
      group.add(&menu_settings_videomode_2x);
      group.add(&menu_settings_videomode_3x);
      group.add(&menu_settings_videomode_4x);
      group.add(&menu_settings_videomode_5x);
      menu_settings_videomode.attach(menu_settings_videomode_1x.create(group, translate["Scale 1x"]));
      menu_settings_videomode.attach(menu_settings_videomode_2x.create(group, translate["Scale 2x"]));
      menu_settings_videomode.attach(menu_settings_videomode_3x.create(group, translate["Scale 3x"]));
      menu_settings_videomode.attach(menu_settings_videomode_4x.create(group, translate["Scale 4x"]));
      menu_settings_videomode.attach(menu_settings_videomode_5x.create(group, translate["Scale 5x"]));
      group.reset();
      menu_settings_videomode.attach(menu_settings_videomode_sep1.create());
      menu_settings_videomode.attach(menu_settings_videomode_aspect_correction.create(translate["Correct Aspect Ratio"]));
      menu_settings_videomode.attach(menu_settings_videomode_sep2.create());
      group.add(&menu_settings_videomode_ntsc);
      group.add(&menu_settings_videomode_pal);
      menu_settings_videomode.attach(menu_settings_videomode_ntsc.create(group, translate["NTSC"]));
      menu_settings_videomode.attach(menu_settings_videomode_pal.create(group, translate["PAL"]));
      group.reset();

    menu_settings.attach(menu_settings_videofilter.create(translate["Video Filter"]));
      group.add(&menu_settings_videofilter_hwpoint);
      group.add(&menu_settings_videofilter_hwlinear);
      menu_settings_videofilter.attach(menu_settings_videofilter_hwpoint.create(group, translate["Point"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_hwlinear.create(group, translate["Linear"]));
      group.reset();
      menu_settings_videofilter.attach(menu_settings_videofilter_sep1.create());
      group.add(&menu_settings_videofilter_swnone);
      group.add(&menu_settings_videofilter_swscanline);
      group.add(&menu_settings_videofilter_swscale2x);
      group.add(&menu_settings_videofilter_swhq2x);
      group.add(&menu_settings_videofilter_swntsc);
      menu_settings_videofilter.attach(menu_settings_videofilter_swnone.create(group, translate["None"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swscanline.create(group, translate["Scanline"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swscale2x.create(group, translate["Scale2x"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swhq2x.create(group, translate["HQ2x"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swntsc.create(group, translate["NTSC"]));
      group.reset();

    menu_settings.attach(menu_settings_videoframeskip.create(translate["Video Frameskip"]));
      group.add(&menu_settings_videoframeskip_0);
      group.add(&menu_settings_videoframeskip_1);
      group.add(&menu_settings_videoframeskip_2);
      group.add(&menu_settings_videoframeskip_3);
      group.add(&menu_settings_videoframeskip_4);
      group.add(&menu_settings_videoframeskip_5);
      group.add(&menu_settings_videoframeskip_6);
      group.add(&menu_settings_videoframeskip_7);
      group.add(&menu_settings_videoframeskip_8);
      group.add(&menu_settings_videoframeskip_9);
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_0.create(group, translate["0"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_1.create(group, translate["1"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_2.create(group, translate["2"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_3.create(group, translate["3"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_4.create(group, translate["4"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_5.create(group, translate["5"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_6.create(group, translate["6"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_7.create(group, translate["7"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_8.create(group, translate["8"]));
      menu_settings_videoframeskip.attach(menu_settings_videoframeskip_9.create(group, translate["9"]));
      group.reset();

    menu_settings.attach(menu_settings_sep1.create());
    menu_settings.attach(menu_settings_mute.create(translate["Mute Audio Output"]));
    menu_settings.attach(menu_settings_sep2.create());

    menu_settings.attach(menu_settings_emuspeed.create(translate["Emulation Speed"]));
      group.add(&menu_settings_emuspeed_slowest);
      group.add(&menu_settings_emuspeed_slow);
      group.add(&menu_settings_emuspeed_normal);
      group.add(&menu_settings_emuspeed_fast);
      group.add(&menu_settings_emuspeed_fastest);
      group.add(&menu_settings_emuspeed_disabled);
      menu_settings_emuspeed.attach(menu_settings_emuspeed_slowest.create(group, translate["50%"]));
      menu_settings_emuspeed.attach(menu_settings_emuspeed_slow.create(group, translate["75%"]));
      menu_settings_emuspeed.attach(menu_settings_emuspeed_normal.create(group, translate["100%"]));
      menu_settings_emuspeed.attach(menu_settings_emuspeed_fast.create(group, translate["150%"]));
      menu_settings_emuspeed.attach(menu_settings_emuspeed_fastest.create(group, translate["200%"]));
      menu_settings_emuspeed.attach(menu_settings_emuspeed_disabled.create(group, translate["Uncapped"]));
      group.reset();

    menu_settings.attach(menu_settings_sep3.create());
    menu_settings.attach(menu_settings_config.create(string() << translate["Configuration"] << " ..."));

  attach(menu_misc.create(translate["Misc"]));
    menu_misc.attach(menu_misc_logaudio.create(translate["Log Audio Data"]));
    menu_misc.attach(menu_misc_showstatus.create(translate["Show Statusbar"]));
    menu_misc.attach(menu_misc_sep1.create());
    menu_misc.attach(menu_misc_about.create(string() << translate["About"] << " ..."));

  menu_file_unload.disable();
  menu_file_reset.disable();
  menu_file_power.disable();

  view.create(0, 256, 224);
  attach(view, 0, 0);

  on_close = bind(&MainWindow::close, this);
  on_block = bind(&MainWindow::block, this);

  menu_file_exit.on_tick = bind(&MainWindow::close, this);

  menu_file_load.on_tick =
  menu_file_load_bsx.on_tick =
  menu_file_load_bsc.on_tick =
  menu_file_load_st.on_tick =
  menu_file_unload.on_tick =
  menu_file_reset.on_tick =
  menu_file_power.on_tick =

  menu_settings_videomode_1x.on_tick =
  menu_settings_videomode_2x.on_tick =
  menu_settings_videomode_3x.on_tick =
  menu_settings_videomode_4x.on_tick =
  menu_settings_videomode_5x.on_tick =
  menu_settings_videomode_aspect_correction.on_tick =
  menu_settings_videomode_ntsc.on_tick =
  menu_settings_videomode_pal.on_tick =

  menu_settings_videofilter_hwpoint.on_tick =
  menu_settings_videofilter_hwlinear.on_tick =
  menu_settings_videofilter_swnone.on_tick =
  menu_settings_videofilter_swscanline.on_tick =
  menu_settings_videofilter_swscale2x.on_tick =
  menu_settings_videofilter_swhq2x.on_tick =
  menu_settings_videofilter_swntsc.on_tick =

  menu_settings_videoframeskip_0.on_tick =
  menu_settings_videoframeskip_1.on_tick =
  menu_settings_videoframeskip_2.on_tick =
  menu_settings_videoframeskip_3.on_tick =
  menu_settings_videoframeskip_4.on_tick =
  menu_settings_videoframeskip_5.on_tick =
  menu_settings_videoframeskip_6.on_tick =
  menu_settings_videoframeskip_7.on_tick =
  menu_settings_videoframeskip_8.on_tick =
  menu_settings_videoframeskip_9.on_tick =

  menu_settings_mute.on_tick =
  menu_settings_emuspeed_slowest.on_tick =
  menu_settings_emuspeed_slow.on_tick =
  menu_settings_emuspeed_normal.on_tick =
  menu_settings_emuspeed_fast.on_tick =
  menu_settings_emuspeed_fastest.on_tick =
  menu_settings_emuspeed_disabled.on_tick =
  menu_settings_config.on_tick =

  menu_misc_logaudio.on_tick =
  menu_misc_showstatus.on_tick =
  menu_misc_about.on_tick =

  bind(&MainWindow::event, this);

  if(config::misc.status_enable) status.show();
}

void MainWindow::sync() {
  event::load_video_settings();

  switch(event::video_settings.multiplier) { default:
    case 1: menu_settings_videomode_1x.check(); break;
    case 2: menu_settings_videomode_2x.check(); break;
    case 3: menu_settings_videomode_3x.check(); break;
    case 4: menu_settings_videomode_4x.check(); break;
    case 5: menu_settings_videomode_5x.check(); break;
  }

  menu_settings_videomode_aspect_correction.check(event::video_settings.aspect_correction);

  switch(event::video_settings.region) { default:
    case 0: menu_settings_videomode_ntsc.check(); break;
    case 1: menu_settings_videomode_pal.check(); break;
  }

  switch(event::video_settings.hardware_filter) { default:
    case 0: menu_settings_videofilter_hwpoint.check(); break;
    case 1: menu_settings_videofilter_hwlinear.check(); break;
  }

  switch(event::video_settings.software_filter) { default:
    case 0: menu_settings_videofilter_swnone.check(); break;
    case 1: menu_settings_videofilter_swscanline.check(); break;
    case 2: menu_settings_videofilter_swscale2x.check(); break;
    case 3: menu_settings_videofilter_swhq2x.check(); break;
    case 4: menu_settings_videofilter_swntsc.check(); break;
  }

  menu_settings_mute.check(config::audio.mute);

  switch(config::video.frameskip) {
    case 0: menu_settings_videoframeskip_0.check(); break;
    case 1: menu_settings_videoframeskip_1.check(); break;
    case 2: menu_settings_videoframeskip_2.check(); break;
    case 3: menu_settings_videoframeskip_3.check(); break;
    case 4: menu_settings_videoframeskip_4.check(); break;
    case 5: menu_settings_videoframeskip_5.check(); break;
    case 6: menu_settings_videoframeskip_6.check(); break;
    case 7: menu_settings_videoframeskip_7.check(); break;
    case 8: menu_settings_videoframeskip_8.check(); break;
    case 9: menu_settings_videoframeskip_9.check(); break;
  }

  switch(config::system.emulation_speed) {
    case 0: menu_settings_emuspeed_slowest.check(); break;
    case 1: menu_settings_emuspeed_slow.check(); break;
    case 2: menu_settings_emuspeed_normal.check(); break;
    case 3: menu_settings_emuspeed_fast.check(); break;
    case 4: menu_settings_emuspeed_fastest.check(); break;
    case 5: menu_settings_emuspeed_disabled.check(); break;
  }

  menu_misc_showstatus.check(config::misc.status_enable);
}
