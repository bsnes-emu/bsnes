bool MainWindow::input_ready() {
  //allow input if main window has focus
  if(focused() == true) return true;
  //allow input if config set to never block input
  if(config::input.capture_mode == 0) return true;
  //block input
  return false;
}

uintptr_t MainWindow::close(event_t) {
  event::quit();
  return false;
}

uintptr_t MainWindow::event(event_t e) {
  if(e.type == event_t::Tick) {
    if(e.widget == &menu_system_load) {
      event::load_cart();
    }

    if(e.widget == &menu_system_power_on) { event::modify_system_state(event::PowerOn); }
    if(e.widget == &menu_system_power_off) { event::modify_system_state(event::PowerOff); }
    if(e.widget == &menu_system_reset) { event::modify_system_state(event::Reset); }

    if(e.widget == &menu_system_controller_port1_none)       { event::update_controller_port1(SNES::Input::DeviceNone); }
    if(e.widget == &menu_system_controller_port1_joypad)     { event::update_controller_port1(SNES::Input::DeviceJoypad); }
    if(e.widget == &menu_system_controller_port1_multitap)   { event::update_controller_port1(SNES::Input::DeviceMultitap); }
    if(e.widget == &menu_system_controller_port1_mouse)      { event::update_controller_port1(SNES::Input::DeviceMouse); }

    if(e.widget == &menu_system_controller_port2_none)       { event::update_controller_port2(SNES::Input::DeviceNone); }
    if(e.widget == &menu_system_controller_port2_joypad)     { event::update_controller_port2(SNES::Input::DeviceJoypad); }
    if(e.widget == &menu_system_controller_port2_multitap)   { event::update_controller_port2(SNES::Input::DeviceMultitap); }
    if(e.widget == &menu_system_controller_port2_mouse)      { event::update_controller_port2(SNES::Input::DeviceMouse); }
    if(e.widget == &menu_system_controller_port2_superscope) { event::update_controller_port2(SNES::Input::DeviceSuperScope); }
    if(e.widget == &menu_system_controller_port2_justifier)  { event::update_controller_port2(SNES::Input::DeviceJustifier); }
    if(e.widget == &menu_system_controller_port2_justifiers) { event::update_controller_port2(SNES::Input::DeviceJustifiers); }

    if(e.widget == &menu_system_expansion_port_none) { config::snes.expansion_port = SNES::ExpansionNone; }
    if(e.widget == &menu_system_expansion_port_bsx)  { config::snes.expansion_port = SNES::ExpansionBSX;  }

    if(e.widget == &menu_system_region_auto) { config::snes.region = SNES::Autodetect; }
    if(e.widget == &menu_system_region_ntsc) { config::snes.region = SNES::NTSC; }
    if(e.widget == &menu_system_region_pal)  { config::snes.region = SNES::PAL; }

    if(e.widget == &menu_system_exit) { event::quit(); }

    if(e.widget == &menu_settings_videomode_1x) { event::update_multiplier(1); }
    if(e.widget == &menu_settings_videomode_2x) { event::update_multiplier(2); }
    if(e.widget == &menu_settings_videomode_3x) { event::update_multiplier(3); }
    if(e.widget == &menu_settings_videomode_4x) { event::update_multiplier(4); }
    if(e.widget == &menu_settings_videomode_5x) { event::update_multiplier(5); }

    if(e.widget == &menu_settings_videomode_aspect_correction) {
      event::update_aspect_correction(menu_settings_videomode_aspect_correction.checked());
    }

    if(e.widget == &menu_settings_videomode_fullscreen) { event::toggle_fullscreen(); }

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

    if(e.widget == &menu_settings_emuspeed_videosync) {
      config::video.synchronize = menu_settings_emuspeed_videosync.checked();
      video.set(Video::Synchronize, config::video.synchronize);
    }

    if(e.widget == &menu_settings_emuspeed_audiosync) {
      config::audio.synchronize = menu_settings_emuspeed_audiosync.checked();
      audio.set(Audio::Synchronize, config::audio.synchronize);
    }

    if(e.widget == &menu_settings_config) { window_settings.show(); }

    if(e.widget == &menu_help_readme) {
      window_textview.set_text(translate["bsnes Readme"]);
      window_textview.text.set_text(text_readme);
      window_textview.focus();
      window_textview.ok.focus();
    }
    if(e.widget == &menu_help_license) {
      window_textview.set_text(translate["bsnes License"]);
      window_textview.text.set_text(text_license);
      window_textview.focus();
      window_textview.ok.focus();
    }
    if(e.widget == &menu_help_about) { window_about.focus(); }
  }

  return true;
}

uintptr_t MainWindow::input(event_t e) {
  uint16_t code = e.param;
  int16_t state = e.param >> 16;

  if(state == 0 && code >= mouse::button && code < mouse::button + mouse::buttons) {
    event::acquire();
  }

  return true;
}

uintptr_t MainWindow::block(event_t) {
  audio.clear();
  return true;
}

void MainWindow::setup() {
  create(Window::AutoCenter, 256, 224, BSNES_TITLE);
  set_background_color(0, 0, 0);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  MenuRadioItemGroup group;
  attach(menu_system.create(translate["{{menu}}System"]));
    menu_system.attach(menu_system_load.create(string() << translate["{{system}}Load Cartridge"] << " ..."));

    menu_system.attach(menu_system_sep1.create());
    menu_system.attach(menu_system_power.create(translate["{{system}}Power"]));
      group.add(&menu_system_power_on);
      group.add(&menu_system_power_off);
      menu_system_power.attach(menu_system_power_on.create(group, translate["{{power}}On"]));
      menu_system_power.attach(menu_system_power_off.create(group, translate["{{power}}Off"]));
      group.reset();
    menu_system.attach(menu_system_reset.create(translate["{{system}}Reset"]));

    menu_system.attach(menu_system_sep2.create());
    menu_system.attach(menu_system_controller_port1.create(translate["{{system}}Controller Port 1"]));
      group.add(&menu_system_controller_port1_none);
      group.add(&menu_system_controller_port1_joypad);
      group.add(&menu_system_controller_port1_multitap);
      group.add(&menu_system_controller_port1_mouse);
      menu_system_controller_port1.attach(menu_system_controller_port1_none.create    (group, translate["{{controllerport}}None"]));
      menu_system_controller_port1.attach(menu_system_controller_port1_joypad.create  (group, translate["{{controllerport}}Joypad"]));
      menu_system_controller_port1.attach(menu_system_controller_port1_multitap.create(group, translate["{{controllerport}}Multitap"]));
      menu_system_controller_port1.attach(menu_system_controller_port1_mouse.create   (group, translate["{{controllerport}}Mouse"]));
      group.reset();
    menu_system.attach(menu_system_controller_port2.create(translate["{{system}}Controller Port 2"]));
      group.add(&menu_system_controller_port2_none);
      group.add(&menu_system_controller_port2_joypad);
      group.add(&menu_system_controller_port2_multitap);
      group.add(&menu_system_controller_port2_mouse);
      group.add(&menu_system_controller_port2_superscope);
      group.add(&menu_system_controller_port2_justifier);
      group.add(&menu_system_controller_port2_justifiers);
      menu_system_controller_port2.attach(menu_system_controller_port2_none.create      (group, translate["{{controllerport}}None"]));
      menu_system_controller_port2.attach(menu_system_controller_port2_joypad.create    (group, translate["{{controllerport}}Joypad"]));
      menu_system_controller_port2.attach(menu_system_controller_port2_multitap.create  (group, translate["{{controllerport}}Multitap"]));
      menu_system_controller_port2.attach(menu_system_controller_port2_mouse.create     (group, translate["{{controllerport}}Mouse"]));
      menu_system_controller_port2.attach(menu_system_controller_port2_superscope.create(group, translate["{{controllerport}}Super Scope"]));
      menu_system_controller_port2.attach(menu_system_controller_port2_justifier.create (group, translate["{{controllerport}}Justifier"]));
      menu_system_controller_port2.attach(menu_system_controller_port2_justifiers.create(group, translate["{{controllerport}}Two Justifiers"]));
      group.reset();

    menu_system_sep3.create();
    menu_system_expansion_port.create(translate["{{system}}Expansion Port"]);
      group.add(&menu_system_expansion_port_none);
      group.add(&menu_system_expansion_port_bsx);
      menu_system_expansion_port.attach(menu_system_expansion_port_none.create(group, translate["{{expansionport}}None"]));
      menu_system_expansion_port.attach(menu_system_expansion_port_bsx.create (group, translate["{{expansionport}}Satellaview"]));
      group.reset();
    menu_system_region.create(translate["{{system}}Region"]);
      group.add(&menu_system_region_auto);
      group.add(&menu_system_region_ntsc);
      group.add(&menu_system_region_pal);
      menu_system_region.attach(menu_system_region_auto.create(group, translate["{{region}}Auto-detect"]));
      menu_system_region.attach(menu_system_region_ntsc.create(group, translate["{{region}}NTSC"]));
      menu_system_region.attach(menu_system_region_pal.create (group, translate["{{region}}PAL"]));
      group.reset();

    if(config::advanced.enable == true) {
      menu_system.attach(menu_system_sep3);
      menu_system.attach(menu_system_expansion_port);
      menu_system.attach(menu_system_region);
    }

    menu_system_sep4.create();
    menu_system_exit.create(translate["{{system}}Exit"]);
    //menu_system.attach(menu_system_sep4);
    //menu_system.attach(menu_system_exit);

  attach(menu_settings.create(translate["{{menu}}Settings"]));
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
      menu_settings_videomode.attach(menu_settings_videomode_fullscreen.create(translate["Fullscreen"]));
      menu_settings_videomode.attach(menu_settings_videomode_sep2.create());
      group.add(&menu_settings_videomode_ntsc);
      group.add(&menu_settings_videomode_pal);
      menu_settings_videomode.attach(menu_settings_videomode_ntsc.create(group, translate["NTSC"]));
      menu_settings_videomode.attach(menu_settings_videomode_pal.create(group, translate["PAL"]));
      group.reset();

    menu_settings.attach(menu_settings_videofilter.create(translate["Video Filter"]));
      group.add(&menu_settings_videofilter_hwpoint);
      group.add(&menu_settings_videofilter_hwlinear);
      menu_settings_videofilter_hwpoint.create(group, translate["Point"]);
      menu_settings_videofilter_hwlinear.create(group, translate["Linear"]);
      group.reset();
      menu_settings_videofilter_sep1.create();
      if(video.cap(Video::Filter)) {
        menu_settings_videofilter.attach(menu_settings_videofilter_hwpoint);
        menu_settings_videofilter.attach(menu_settings_videofilter_hwlinear);
        menu_settings_videofilter.attach(menu_settings_videofilter_sep1);
      }
      group.add(&menu_settings_videofilter_swnone);
      group.add(&menu_settings_videofilter_swscanline);
      group.add(&menu_settings_videofilter_swscale2x);
      group.add(&menu_settings_videofilter_swhq2x);
      group.add(&menu_settings_videofilter_swntsc);
      menu_settings_videofilter.attach(menu_settings_videofilter_swnone.create(group, translate["None"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swscanline.create(group, translate["Scanline"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swscale2x.create(group, translate["Scale2x"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swhq2x.create(group, translate["HQ2x"]));
      menu_settings_videofilter.attach(menu_settings_videofilter_swntsc.create(group, translate["{{filter}}NTSC"]));
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
    menu_settings_emuspeed.create(translate["Emulation Speed"]);
      group.add(&menu_settings_emuspeed_slowest);
      group.add(&menu_settings_emuspeed_slow);
      group.add(&menu_settings_emuspeed_normal);
      group.add(&menu_settings_emuspeed_fast);
      group.add(&menu_settings_emuspeed_fastest);
      menu_settings_emuspeed_slowest.create(group, translate["50%"]);
      menu_settings_emuspeed_slow.create(group, translate["75%"]);
      menu_settings_emuspeed_normal.create(group, translate["100%"]);
      menu_settings_emuspeed_fast.create(group, translate["150%"]);
      menu_settings_emuspeed_fastest.create(group, translate["200%"]);
      group.reset();
      menu_settings_emuspeed_sep1.create();
      menu_settings_emuspeed_videosync.create(translate["Sync Video"]);
      menu_settings_emuspeed_audiosync.create(translate["Sync Audio"]);

      if(audio.cap(Audio::Frequency)) {
        //only audio can sync to specific frequency rates; video syncs only to monitor refresh rate
        menu_settings_emuspeed.attach(menu_settings_emuspeed_slowest);
        menu_settings_emuspeed.attach(menu_settings_emuspeed_slow);
        menu_settings_emuspeed.attach(menu_settings_emuspeed_normal);
        menu_settings_emuspeed.attach(menu_settings_emuspeed_fast);
        menu_settings_emuspeed.attach(menu_settings_emuspeed_fastest);
        menu_settings_emuspeed.attach(menu_settings_emuspeed_sep1);
      }

      if(video.cap(Video::Synchronize)) {
        menu_settings_emuspeed.attach(menu_settings_emuspeed_videosync);
      }
      if(audio.cap(Audio::Synchronize)) {
        menu_settings_emuspeed.attach(menu_settings_emuspeed_audiosync);
      }

    if(video.cap(Video::Synchronize) || audio.cap(Audio::Synchronize)) {
      menu_settings.attach(menu_settings_emuspeed);
    }
    menu_settings.attach(menu_settings_config.create(string() << translate["Configuration"] << " ..."));

  attach(menu_help.create(translate["{{menu}}Help"]));
    menu_help.attach(menu_help_readme.create(string() << translate["{{help}}Readme"] << " ..."));
    menu_help.attach(menu_help_license.create(string() << translate["{{help}}License"] << " ..."));
    menu_help.attach(menu_help_sep1.create());
    menu_help.attach(menu_help_about.create(string() << translate["{{help}}About"] << " ..."));

  menu_system_power.disable();
  menu_system_reset.disable();

  view.create(0, 256, 224);
  view.on_input = bind(&MainWindow::input, this);
  attach(view, 0, 0);

  on_close = bind(&MainWindow::close, this);
  on_block = bind(&MainWindow::block, this);

  menu_system_load.on_tick =
  menu_system_power_on.on_tick =
  menu_system_power_off.on_tick =
  menu_system_reset.on_tick =

  menu_system_controller_port1_none.on_tick =
  menu_system_controller_port1_joypad.on_tick =
  menu_system_controller_port1_multitap.on_tick =
  menu_system_controller_port1_mouse.on_tick =

  menu_system_controller_port2_none.on_tick =
  menu_system_controller_port2_joypad.on_tick =
  menu_system_controller_port2_multitap.on_tick =
  menu_system_controller_port2_mouse.on_tick =
  menu_system_controller_port2_superscope.on_tick =
  menu_system_controller_port2_justifier.on_tick =
  menu_system_controller_port2_justifiers.on_tick =

  menu_system_expansion_port_none.on_tick =
  menu_system_expansion_port_bsx.on_tick =

  menu_system_region_auto.on_tick =
  menu_system_region_ntsc.on_tick =
  menu_system_region_pal.on_tick =

  menu_system_exit.on_tick =

  menu_settings_videomode_1x.on_tick =
  menu_settings_videomode_2x.on_tick =
  menu_settings_videomode_3x.on_tick =
  menu_settings_videomode_4x.on_tick =
  menu_settings_videomode_5x.on_tick =
  menu_settings_videomode_aspect_correction.on_tick =
  menu_settings_videomode_fullscreen.on_tick =
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
  menu_settings_emuspeed_videosync.on_tick =
  menu_settings_emuspeed_audiosync.on_tick =
  menu_settings_config.on_tick =

  menu_help_readme.on_tick =
  menu_help_license.on_tick =
  menu_help_about.on_tick =

  bind(&MainWindow::event, this);

  status.show();
}

void MainWindow::sync() {
  event::load_video_settings();

  switch(config::snes.controller_port1) { default:
    case SNES::Input::DeviceNone:       menu_system_controller_port1_none.check();       break;
    case SNES::Input::DeviceJoypad:     menu_system_controller_port1_joypad.check();     break;
    case SNES::Input::DeviceMultitap:   menu_system_controller_port1_multitap.check();   break;
    case SNES::Input::DeviceMouse:      menu_system_controller_port1_mouse.check();      break;
  }

  switch(config::snes.controller_port2) { default:
    case SNES::Input::DeviceNone:       menu_system_controller_port2_none.check();       break;
    case SNES::Input::DeviceJoypad:     menu_system_controller_port2_joypad.check();     break;
    case SNES::Input::DeviceMultitap:   menu_system_controller_port2_multitap.check();   break;
    case SNES::Input::DeviceMouse:      menu_system_controller_port2_mouse.check();      break;
    case SNES::Input::DeviceSuperScope: menu_system_controller_port2_superscope.check(); break;
    case SNES::Input::DeviceJustifier:  menu_system_controller_port2_justifier.check();  break;
    case SNES::Input::DeviceJustifiers: menu_system_controller_port2_justifiers.check(); break;
  }

  switch(config::snes.expansion_port) { default:
    case SNES::ExpansionNone: menu_system_expansion_port_none.check(); break;
    case SNES::ExpansionBSX:  menu_system_expansion_port_bsx.check();  break;
  }

  switch(config::snes.region) { default:
    case SNES::Autodetect: menu_system_region_auto.check(); break;
    case SNES::NTSC:       menu_system_region_ntsc.check(); break;
    case SNES::PAL:        menu_system_region_pal.check();  break;
  }

  switch(event::video_settings.multiplier) { default:
    case 1: menu_settings_videomode_1x.check(); break;
    case 2: menu_settings_videomode_2x.check(); break;
    case 3: menu_settings_videomode_3x.check(); break;
    case 4: menu_settings_videomode_4x.check(); break;
    case 5: menu_settings_videomode_5x.check(); break;
  }

  menu_settings_videomode_aspect_correction.check(event::video_settings.aspect_correction);
  menu_settings_videomode_fullscreen.check(event::video_settings.mode == 1);

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
  }

  menu_settings_emuspeed_videosync.check(config::video.synchronize);
  menu_settings_emuspeed_audiosync.check(config::audio.synchronize);
}
