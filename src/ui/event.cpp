namespace event {

void keydown(uint16_t key) {
  if(window_main.focused()) {
    if(key == input_manager.gui.load) load_rom();
    if(key == input_manager.gui.pause) {
      app.pause = !app.pause; //toggle pause state
      if(app.pause) {
        audio.clear();
        if(cartridge.loaded()) status.update();
      }
    }
    if(key == input_manager.gui.reset) reset();
    if(key == input_manager.gui.power) power();
    if(key == input_manager.gui.quit) quit();
    if(key == input_manager.gui.speed_decrease) {
      update_emulation_speed(config::system.emulation_speed - 1);
    }
    if(key == input_manager.gui.speed_increase) {
      update_emulation_speed(config::system.emulation_speed + 1);
    }
    if(key == input_manager.gui.frameskip_decrease) {
      update_frameskip(config::video.frameskip - 1);
    }
    if(key == input_manager.gui.frameskip_increase) {
      update_frameskip(config::video.frameskip + 1);
    }
    if(key == input_manager.gui.toggle_fullscreen) toggle_fullscreen();
    if(key == input_manager.gui.toggle_menubar) toggle_menubar();
    if(key == input_manager.gui.toggle_statusbar) toggle_statusbar();
  }

  if(window_input_capture.focused()) {
    if(window_input_capture.waiting == true) {
      if(window_input_capture.locked == false) {
        window_input_capture.assign(key);
      }
    }
  }
}

void keyup(uint16_t key) {
  if(window_input_capture.focused()) {
    if(window_input_capture.waiting == true) {
      if(window_input_capture.locked == true) {
        window_input_capture.locked = input.key_down(keyboard::return_) || input.key_down(keyboard::spacebar);
      }
    }
  }
}

void load_video_settings() {
  video_settings.mode = config::video.mode;
  switch(video_settings.mode) { default:
    case 0: { //windowed
      video_settings.aspect_correction = config::video.windowed.aspect_correction;
      video_settings.synchronize       = config::video.windowed.synchronize;
      video_settings.region            = config::video.windowed.region;
      video_settings.multiplier        = config::video.windowed.multiplier;
      video_settings.hardware_filter   = config::video.windowed.hardware_filter;
      video_settings.software_filter   = config::video.windowed.software_filter;
    } break;

    case 1: { //fullscreen
      video_settings.aspect_correction = config::video.fullscreen.aspect_correction;
      video_settings.synchronize       = config::video.fullscreen.synchronize;
      video_settings.region            = config::video.fullscreen.region;
      video_settings.multiplier        = config::video.fullscreen.multiplier;
      video_settings.hardware_filter   = config::video.fullscreen.hardware_filter;
      video_settings.software_filter   = config::video.fullscreen.software_filter;
    } break;
  }
}

void update_aspect_correction(bool aspect_correction) {
  switch(config::video.mode) { default:
    case 0: config::video.windowed.aspect_correction   = aspect_correction; break;
    case 1: config::video.fullscreen.aspect_correction = aspect_correction; break;
  }
  update_video_settings();
}

void update_multiplier(uint multiplier) {
  switch(config::video.mode) { default:
    case 0: config::video.windowed.multiplier   = multiplier; break;
    case 1: config::video.fullscreen.multiplier = multiplier; break;
  }
  update_video_settings();
}

void update_region(uint region) {
  switch(config::video.mode) { default:
    case 0: config::video.windowed.region   = region; break;
    case 1: config::video.fullscreen.region = region; break;
  }
  update_video_settings();
}

void update_hardware_filter(uint hardware_filter) {
  switch(config::video.mode) { default:
    case 0: config::video.windowed.hardware_filter   = hardware_filter; break;
    case 1: config::video.fullscreen.hardware_filter = hardware_filter; break;
  }
  update_video_settings();
}

void update_software_filter(uint software_filter) {
  switch(config::video.mode) { default:
    case 0: config::video.windowed.software_filter   = software_filter; break;
    case 1: config::video.fullscreen.software_filter = software_filter; break;
  }
  update_video_settings();
}

void update_frameskip(int speed) {
  config::video.frameskip = max(0, min(9, speed));
  window_main.sync();
}

void update_emulation_speed(int speed) {
  config::system.emulation_speed = speed = max(0, min(4, speed));

  double scale[] = { 0.50, 0.75, 1.00, 1.50, 2.00 };
  unsigned outfreq = config::audio.output_frequency;
  unsigned infreq  = config::audio.input_frequency * scale[speed] + 0.5;

  audio.set(Audio::Resample, outfreq != infreq); //only resample when necessary
  audio.set(Audio::ResampleOutputFrequency, outfreq);
  audio.set(Audio::ResampleInputFrequency,  infreq);

  window_main.sync();
}

void update_controller_port1(int device) {
  unsigned current_device = config::snes.controller_port1;
  unsigned new_device;

  switch(device) { default:
    case 0: new_device = SNES::Input::DeviceNone;     break;
    case 1: new_device = SNES::Input::DeviceJoypad;   break;
    case 2: new_device = SNES::Input::DeviceMultitap; break;
  }

  if(new_device != current_device) {
    snes.input.port_set_device(0, config::snes.controller_port1 = new_device);
  }

  window_main.sync();
}

void update_controller_port2(int device) {
  unsigned current_device = config::snes.controller_port2;
  unsigned new_device;

  switch(device) { default:
    case 0: new_device = SNES::Input::DeviceNone;     break;
    case 1: new_device = SNES::Input::DeviceJoypad;   break;
    case 2: new_device = SNES::Input::DeviceMultitap; break;
  }

  if(new_device != current_device) {
    snes.input.port_set_device(1, config::snes.controller_port2 = new_device);
  }

  window_main.sync();
}

void update_video_settings() {
  load_video_settings();

  uint width  = 256;
  uint height = video_settings.region == 0 ? 224 : 239;
  uint multiplier = minmax<1, 5>(video_settings.multiplier);
  width  *= multiplier;
  height *= multiplier;
  if(video_settings.aspect_correction == true) {
    if(video_settings.region == 0) {
      //NTSC
      width = uint( double(width) * double(config::video.aspect_ntsc_x) / double(config::video.aspect_ntsc_y) );
    } else {
      //PAL
      width = uint( double(width) * double(config::video.aspect_pal_x)  / double(config::video.aspect_pal_y)  );
    }
  }

  switch(video_settings.mode) { default:
    case 0: { //windowed
      window_main.unfullscreen();
      //try and clamp windows larger than the screen to the screen size.
      //note that since APIs such as X11 lack the ability to get the full window size
      //(with border, etc), this can never be a perfect fit to the screen.
      int w = min(width,  hiro().screen_width());
      int h = min(height, hiro().screen_height());
      window_main.resize(w, h);
      window_main.move(window_main.view, 0, 0);
      window_main.view.resize(w, h);
    } break;

    case 1: { //fullscreen
      window_main.fullscreen();
      int view_width  = window_main.get_width();
      int view_height = window_main.get_height();
      window_main.move(window_main.view,
        //center view inside window, do not exceed window size
        width  < view_width  ? (view_width  - width)  / 2 : 0,
        height < view_height ? (view_height - height) / 2 : 0
      );
      window_main.view.resize(min(width, view_width), min(height, view_height));
    } break;
  }

  libfilter::FilterInterface::FilterType filter;
  switch(video_settings.software_filter) { default:
    case 0: filter = libfilter::FilterInterface::Direct;   break;
    case 1: filter = libfilter::FilterInterface::Scanline; break;
    case 2: filter = libfilter::FilterInterface::Scale2x;  break;
    case 3: filter = libfilter::FilterInterface::HQ2x;     break;
    case 4: filter = libfilter::FilterInterface::NTSC;     break;
  }

  libfilter::filter.set(filter);

  SNES::Video::Mode mode;
  switch(video_settings.region) { default:
    case 0: mode = SNES::Video::ModeNTSC; break;
    case 1: mode = SNES::Video::ModePAL;  break;
  }

  snes.video.set_mode(mode);

  video.set(Video::Synchronize, video_settings.synchronize);
  video.set(Video::Filter, video_settings.hardware_filter);

  //update main window video mode checkbox settings
  window_main.sync();
}

void update_opacity() {
  //convert opacity from 50-100 (percentage) to 128-255 (8-bit alpha)
  unsigned opacity = max(50, min(100, config::misc.opacity));
  opacity = unsigned(256.0 / 100.0 * opacity);
  opacity = max(128, min(255, opacity));

  window_settings.set_opacity(opacity);
  window_input_capture.set_opacity(opacity);
  window_bsxloader.set_opacity(opacity);
  window_stloader.set_opacity(opacity);
  window_about.set_opacity(opacity);
}

void toggle_fullscreen() {
  if(config::video.mode != 1) { //switch to fullscreen mode if not already in it
    config::video.mode = 1;
  } else { //switch to windowed mode if already in fullscreen mode
    config::video.mode = 0;
  }
  update_video_settings();
}

void toggle_menubar() {
  window_main.menu.show(!window_main.menu.visible());
  update_video_settings();
}

void toggle_statusbar() {
  window_main.status.show(!window_main.status.visible());
  update_video_settings();
}

//

bool load_rom(char *fn) {
  audio.clear();

  lstring dir;
  strcpy(fn, "");
  strcpy(dir[0], config::path.rom);
  replace(dir[0], "\\", "/");
  if(strlen(dir[0]) && !strend(dir[0], "/")) strcat(dir[0], "/");

  //append base path if rom path is relative
  if(strbegin(dir[0], "./")) {
    ltrim(dir[0], "./");
    strcpy(dir[1], dir[0]);
    strcpy(dir[0], config::path.base);
    strcat(dir[0], dir[1]);
  }

  return hiro().file_open(0, fn,
    dir[0],
    "SNES images\t"
    "*.smc,*.sfc,*.swc,*.fig,*.bs,*.st"
    #if defined(GZIP_SUPPORT)
    ",*.gz,*.z,*.zip"
    #endif
    #if defined(JMA_SUPPORT)
    ",*.jma"
    #endif
    "\n"
    "All files\t"
    "*.*"
  );
}

void load_rom() {
  char fn[PATH_MAX];
  if(load_rom(fn) == false) return;
  load_cart_normal(fn);
}

void load_cart_normal(const char *filename) {
  if(!filename || !*filename) return;

  if(cartridge.loaded() == true) cartridge.unload();
  cartridge.load_cart_normal(filename);

  app.pause = false;
  snes.power();
  window_main.menu_file_unload.enable();
  window_main.menu_file_reset.enable();
  window_main.menu_file_power.enable();
  window_cheat_editor.refresh();

  status.flush();
  string t = translate["Loaded $."];
  replace(t, "$", cartridge.info.filename);
  status.enqueue(t);
  if(cartridge.info.patched) status.enqueue(translate["UPS patch applied."]);

  //warn if unsupported hardware detected
  string message = translate["Warning: unsupported $ chip detected."];
  if(cartridge.info.superfx) { replace(message, "$", "SuperFX"); status.enqueue(message); }
  if(cartridge.info.sa1)     { replace(message, "$", "SA-1");    status.enqueue(message); }
  if(cartridge.info.st011)   { replace(message, "$", "ST011");   status.enqueue(message); }
  if(cartridge.info.st018)   { replace(message, "$", "ST018");   status.enqueue(message); }
}

void load_cart_bsx(const char *base, const char *slot) {
  if(!base || !*base) return;

  if(cartridge.loaded() == true) cartridge.unload();
  cartridge.load_cart_bsx(base, slot);

  app.pause = false;
  snes.power();
  window_main.menu_file_unload.enable();
  window_main.menu_file_reset.enable();
  window_main.menu_file_power.enable();
  window_cheat_editor.refresh();

  status.flush();
  string t = translate["Loaded $."];
  replace(t, "$", cartridge.info.filename);
  status.enqueue(t);
}

void load_cart_bsc(const char *base, const char *slot) {
  if(!base || !*base) return;

  if(cartridge.loaded() == true) cartridge.unload();
  cartridge.load_cart_bsc(base, slot);

  app.pause = false;
  snes.power();
  window_main.menu_file_unload.enable();
  window_main.menu_file_reset.enable();
  window_main.menu_file_power.enable();
  window_cheat_editor.refresh();

  status.flush();
  string t = translate["Loaded $."];
  replace(t, "$", cartridge.info.filename);
  status.enqueue(t);
}

void load_cart_st(const char *base, const char *slotA, const char *slotB) {
  if(!base || !*base) return;

  if(cartridge.loaded() == true) cartridge.unload();
  cartridge.load_cart_st(base, slotA, slotB);

  app.pause = false;
  snes.power();
  window_main.menu_file_unload.enable();
  window_main.menu_file_reset.enable();
  window_main.menu_file_power.enable();
  window_cheat_editor.refresh();

  status.flush();
  string t = translate["Loaded $."];
  replace(t, "$", cartridge.info.filename);
  status.enqueue(t);
}

void unload_rom() {
  if(cartridge.loaded() == true) {
    cartridge.unload();
    video.clear();
    audio.clear();
  }
  window_main.menu_file_unload.disable();
  window_main.menu_file_reset.disable();
  window_main.menu_file_power.disable();
  window_cheat_editor.refresh();

  status.flush();
  string t = translate["Unloaded $."];
  replace(t, "$", cartridge.info.filename);
  status.enqueue(t);
}

void reset() {
  if(cartridge.loaded() == true) {
    snes.reset();
    status.flush();
    status.enqueue(translate["System was reset."]);
  }
}

void power() {
  if(cartridge.loaded() == true) {
    snes.power();
    status.flush();
    status.enqueue(translate["System power was cycled."]);
  }
}

void quit() {
  app.term = true;
  window_about.hide();
  window_settings.hide();
  window_bsxloader.hide();
  window_stloader.hide();
  window_main.hide();
}

};
