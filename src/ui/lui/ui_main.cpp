bool MainWindow::message(uint id, uintptr_t param) {
ui::Control *control = (ui::Control*)param;
  if(id == ui::Message::Close) {
    _term_ = true;
    hide();
    return false;
  }

  if(id == ui::Message::Block) {
    if(uiAudio) { uiAudio->clear_audio(); }
    return true;
  }

  if(id == ui::Message::KeyDown) {
    if(uiInput) { uiInput->signal_key_down(param); }
    if(param == keymap::esc) { event::toggle_menu(); }
    if(param == keymap::f11) { event::toggle_fullscreen(); }
    return true;
  }

  if(id == ui::Message::KeyUp) {
    if(uiInput) { uiInput->signal_key_up(param); }
    return true;
  }

  if(id == ui::Message::Clicked) {
    if(control == &menu_file_load) {
      event::load_rom();
    }

    if(control == &menu_file_unload) {
      event::unload_rom();
    }

    if(control == &menu_file_reset) {
      event::reset();
    }

    if(control == &menu_file_power) {
      event::power();
    }

    if(control == &menu_file_exit) {
      message(ui::Message::Close);
    }

    if(control == &menu_settings_videomode_1x) { config::video.multiplier = 1; event::update_video_settings(); }
    if(control == &menu_settings_videomode_2x) { config::video.multiplier = 2; event::update_video_settings(); }
    if(control == &menu_settings_videomode_3x) { config::video.multiplier = 3; event::update_video_settings(); }
    if(control == &menu_settings_videomode_4x) { config::video.multiplier = 4; event::update_video_settings(); }
    if(control == &menu_settings_videomode_5x) { config::video.multiplier = 5; event::update_video_settings(); }

    if(control == &menu_settings_videomode_aspect_correction) {
      config::video.aspect_correction = menu_settings_videomode_aspect_correction.checked();
      event::update_video_settings();
    }

    if(control == &menu_settings_videomode_ntsc) { config::video.region = 0; event::update_raster_settings(); event::update_video_settings(); }
    if(control == &menu_settings_videomode_pal)  { config::video.region = 1; event::update_raster_settings(); event::update_video_settings(); }

    if(control == &menu_settings_videofilter_hwpoint)  { config::video.hardware_filter = 0; uiVideo->update_hardware_filter(); }
    if(control == &menu_settings_videofilter_hwlinear) { config::video.hardware_filter = 1; uiVideo->update_hardware_filter(); }

    if(control == &menu_settings_videofilter_swnone)    { config::video.software_filter = 0; event::update_raster_settings(); }
    if(control == &menu_settings_videofilter_swntsc)    { config::video.software_filter = 1; event::update_raster_settings(); }
    if(control == &menu_settings_videofilter_swhq2x)    { config::video.software_filter = 2; event::update_raster_settings(); }
    if(control == &menu_settings_videofilter_swscale2x) { config::video.software_filter = 3; event::update_raster_settings(); }

    if(control == &menu_settings_videoframeskip_0) { config::video.frameskip = 0; }
    if(control == &menu_settings_videoframeskip_1) { config::video.frameskip = 1; }
    if(control == &menu_settings_videoframeskip_2) { config::video.frameskip = 2; }
    if(control == &menu_settings_videoframeskip_3) { config::video.frameskip = 3; }
    if(control == &menu_settings_videoframeskip_4) { config::video.frameskip = 4; }
    if(control == &menu_settings_videoframeskip_5) { config::video.frameskip = 5; }
    if(control == &menu_settings_videoframeskip_6) { config::video.frameskip = 6; }
    if(control == &menu_settings_videoframeskip_7) { config::video.frameskip = 7; }
    if(control == &menu_settings_videoframeskip_8) { config::video.frameskip = 8; }
    if(control == &menu_settings_videoframeskip_9) { config::video.frameskip = 9; }

    if(control == &menu_settings_mute) {
      config::snes.mute = menu_settings_mute.checked();
    }

    if(control == &menu_settings_speedreg_enable) {
      config::system.regulate_speed = menu_settings_speedreg_enable.checked();
    }

    if(control == &menu_settings_speedreg_slowest) { config::system.speed = 1; uiAudio->update_frequency(); }
    if(control == &menu_settings_speedreg_slow)    { config::system.speed = 2; uiAudio->update_frequency(); }
    if(control == &menu_settings_speedreg_normal)  { config::system.speed = 3; uiAudio->update_frequency(); }
    if(control == &menu_settings_speedreg_fast)    { config::system.speed = 4; uiAudio->update_frequency(); }
    if(control == &menu_settings_speedreg_fastest) { config::system.speed = 5; uiAudio->update_frequency(); }

    if(control == &menu_settings_config) { window_settings.show(); }

    if(control == &menu_misc_logaudio) {
      (menu_misc_logaudio.checked() == true) ? snes.log_audio_enable() : snes.log_audio_disable();
    }

    if(control == &menu_misc_about) {
      window_about.focus();
    }

    return true;
  }

  return true;
}

void MainWindow::setup() {
ui::ControlGroup group;
  create(ui::Window::Center, 256, 224, BSNES_TITLE);
  set_background_color(0, 0, 0);

  menu.create(*this);
    menu_file.create(menu, "File");
      menu_file_load.create(menu_file, "Load Cartridge");
      menu_file_unload.create(menu_file, "Unload");
      menu_file_sep1.create(menu_file);
      menu_file_reset.create(menu_file, "Reset");
      menu_file_power.create(menu_file, "Power");
      menu_file_sep2.create(menu_file);
      menu_file_exit.create(menu_file, "Exit");
    menu_file.finish();

    menu_settings.create(menu, "Settings");
      menu_settings_videomode.create(menu_settings, "Video Mode");
        group.add(menu_settings_videomode_1x);
        group.add(menu_settings_videomode_2x);
        group.add(menu_settings_videomode_3x);
        group.add(menu_settings_videomode_4x);
        group.add(menu_settings_videomode_5x);
        menu_settings_videomode_1x.create(menu_settings_videomode, group, "Scale 1x");
        menu_settings_videomode_2x.create(menu_settings_videomode, group, "Scale 2x");
        menu_settings_videomode_3x.create(menu_settings_videomode, group, "Scale 3x");
        menu_settings_videomode_4x.create(menu_settings_videomode, group, "Scale 4x");
        menu_settings_videomode_5x.create(menu_settings_videomode, group, "Scale 5x");
        group.reset();
        menu_settings_videomode_sep1.create(menu_settings_videomode);
        menu_settings_videomode_aspect_correction.create(menu_settings_videomode, "Correct Aspect Ratio");
        menu_settings_videomode_sep2.create(menu_settings_videomode);
        group.add(menu_settings_videomode_ntsc);
        group.add(menu_settings_videomode_pal);
        menu_settings_videomode_ntsc.create(menu_settings_videomode, group, "NTSC");
        menu_settings_videomode_pal.create(menu_settings_videomode, group, "PAL");
        group.reset();
      menu_settings_videomode.finish();

      menu_settings_videofilter.create(menu_settings, "Video Filter");
        group.add(menu_settings_videofilter_hwpoint);
        group.add(menu_settings_videofilter_hwlinear);
        menu_settings_videofilter_hwpoint.create(menu_settings_videofilter, group, "Point");
        menu_settings_videofilter_hwlinear.create(menu_settings_videofilter, group, "Linear");
        group.reset();
        menu_settings_videofilter_sep1.create(menu_settings_videofilter);
        group.add(menu_settings_videofilter_swnone);
        group.add(menu_settings_videofilter_swntsc);
        group.add(menu_settings_videofilter_swhq2x);
        group.add(menu_settings_videofilter_swscale2x);
        menu_settings_videofilter_swnone.create(menu_settings_videofilter, group, "None");
        menu_settings_videofilter_swntsc.create(menu_settings_videofilter, group, "NTSC");
        menu_settings_videofilter_swhq2x.create(menu_settings_videofilter, group, "HQ2x");
        menu_settings_videofilter_swscale2x.create(menu_settings_videofilter, group, "Scale2x");
        group.reset();
      menu_settings_videofilter.finish();

      menu_settings_videoframeskip.create(menu_settings, "Video Frameskip");
        group.add(menu_settings_videoframeskip_0);
        group.add(menu_settings_videoframeskip_1);
        group.add(menu_settings_videoframeskip_2);
        group.add(menu_settings_videoframeskip_3);
        group.add(menu_settings_videoframeskip_4);
        group.add(menu_settings_videoframeskip_5);
        group.add(menu_settings_videoframeskip_6);
        group.add(menu_settings_videoframeskip_7);
        group.add(menu_settings_videoframeskip_8);
        group.add(menu_settings_videoframeskip_9);
        menu_settings_videoframeskip_0.create(menu_settings_videoframeskip, group, "0 (off)");
        menu_settings_videoframeskip_sep1.create(menu_settings_videoframeskip);
        menu_settings_videoframeskip_1.create(menu_settings_videoframeskip, group, "1");
        menu_settings_videoframeskip_2.create(menu_settings_videoframeskip, group, "2");
        menu_settings_videoframeskip_3.create(menu_settings_videoframeskip, group, "3");
        menu_settings_videoframeskip_4.create(menu_settings_videoframeskip, group, "4");
        menu_settings_videoframeskip_5.create(menu_settings_videoframeskip, group, "5");
        menu_settings_videoframeskip_6.create(menu_settings_videoframeskip, group, "6");
        menu_settings_videoframeskip_7.create(menu_settings_videoframeskip, group, "7");
        menu_settings_videoframeskip_8.create(menu_settings_videoframeskip, group, "8");
        menu_settings_videoframeskip_9.create(menu_settings_videoframeskip, group, "9");
        group.reset();
      menu_settings_videoframeskip.finish();

      menu_settings_sep1.create(menu_settings);
      menu_settings_mute.create(menu_settings, "Mute Sound Output");
      menu_settings_sep2.create(menu_settings);

      menu_settings_speedreg.create(menu_settings, "Speed Regulation");
        menu_settings_speedreg_enable.create(menu_settings_speedreg, "Enable");
        menu_settings_speedreg_sep1.create(menu_settings_speedreg);
        group.add(menu_settings_speedreg_slowest);
        group.add(menu_settings_speedreg_slow);
        group.add(menu_settings_speedreg_normal);
        group.add(menu_settings_speedreg_fast);
        group.add(menu_settings_speedreg_fastest);
        menu_settings_speedreg_slowest.create(menu_settings_speedreg, group, "Slowest");
        menu_settings_speedreg_slow.create(menu_settings_speedreg, group, "Slow");
        menu_settings_speedreg_normal.create(menu_settings_speedreg, group, "Normal");
        menu_settings_speedreg_fast.create(menu_settings_speedreg, group, "Fast");
        menu_settings_speedreg_fastest.create(menu_settings_speedreg, group, "Fastest");
        group.reset();
      menu_settings_speedreg.finish();

      menu_settings_sep3.create(menu_settings);
      menu_settings_config.create(menu_settings, "Configuration");
    menu_settings.finish();

    menu_misc.create(menu, "Misc");
      menu_misc_logaudio.create(menu_misc, "Log Audio Data");
      menu_misc_sep1.create(menu_misc);
      menu_misc_about.create(menu_misc, "About ...");
    menu_misc.finish();
  menu.finish();

  view.create(*this, 0, 0, 0, 256, 224);
  view.set_background_color(0, 0, 0);
}

void MainWindow::setup_menu() {
  config::video.multiplier = minmax<1, 5>(uint(config::video.multiplier));
  switch(uint(config::video.multiplier)) {
  case 1: menu_settings_videomode_1x.check(); break;
  case 2: menu_settings_videomode_2x.check(); break;
  case 3: menu_settings_videomode_3x.check(); break;
  case 4: menu_settings_videomode_4x.check(); break;
  case 5: menu_settings_videomode_5x.check(); break;
  }

  menu_settings_videomode_aspect_correction.check(config::video.aspect_correction);

  config::video.region = minmax<0, 1>(uint(config::video.region));
  switch(uint(config::video.region)) {
  case 0: menu_settings_videomode_ntsc.check(); break;
  case 1: menu_settings_videomode_pal.check();  break;
  }

  config::video.hardware_filter = minmax<0, 1>(uint(config::video.hardware_filter));
  switch(uint(config::video.hardware_filter)) {
  case 0: menu_settings_videofilter_hwpoint.check();  break;
  case 1: menu_settings_videofilter_hwlinear.check(); break;
  }

  config::video.software_filter = minmax<0, 3>(uint(config::video.software_filter));
  switch(uint(config::video.software_filter)) {
  case 0: menu_settings_videofilter_swnone.check();    break;
  case 1: menu_settings_videofilter_swntsc.check();    break;
  case 2: menu_settings_videofilter_swhq2x.check();    break;
  case 3: menu_settings_videofilter_swscale2x.check(); break;
  }

  menu_settings_mute.check(config::snes.mute);

  menu_settings_speedreg_enable.check(config::system.regulate_speed);
  menu_settings_speedreg_normal.check();
}
