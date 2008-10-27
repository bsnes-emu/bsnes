namespace event {

void input_event(uint16_t code) {
  if(code == keyboard::escape && input.acquired()) {
    unacquire();
    return;
  }

  if(window_main.focused()) {
    if(code == inputuigeneral.load.code) load_cart();
    if(code == inputuigeneral.pause.code) {
      app.pause = !app.pause; //toggle pause state
      if(app.pause) {
        audio.clear();
        if(cartridge.loaded()) status.update();
      }
    }
    if(code == inputuigeneral.reset.code) modify_system_state(Reset);
    if(code == inputuigeneral.power.code) modify_system_state(PowerCycle);
    if(code == inputuigeneral.quit.code) quit();
    if(code == inputuigeneral.speed_decrease.code) {
      update_emulation_speed(config::system.emulation_speed - 1);
    }
    if(code == inputuigeneral.speed_increase.code) {
      update_emulation_speed(config::system.emulation_speed + 1);
    }
    if(code == inputuigeneral.frameskip_decrease.code) {
      update_frameskip(config::video.frameskip - 1);
    }
    if(code == inputuigeneral.frameskip_increase.code) {
      update_frameskip(config::video.frameskip + 1);
    }
    if(code == inputuigeneral.toggle_fullscreen.code) toggle_fullscreen();
    if(code == inputuigeneral.toggle_menubar.code) toggle_menubar();
    if(code == inputuigeneral.toggle_statusbar.code) toggle_statusbar();
  }
}

void acquire() {
  if(cartridge.loaded() == true) {
    if(config::snes.controller_port1 == SNES::Input::DeviceMouse
    || config::snes.controller_port2 == SNES::Input::DeviceMouse
    || config::snes.controller_port2 == SNES::Input::DeviceSuperScope
    || config::snes.controller_port2 == SNES::Input::DeviceJustifier
    || config::snes.controller_port2 == SNES::Input::DeviceJustifiers
    ) input.acquire();
  }
}

void unacquire() {
  input.unacquire();
}

void load_video_settings() {
  video_settings.mode = config::video.mode;
  video_settings.synchronize = config::video.synchronize;

  switch(video_settings.mode) { default:
    case 0: { //windowed
      video_settings.aspect_correction = config::video.windowed.aspect_correction;
      video_settings.region            = config::video.windowed.region;
      video_settings.multiplier        = config::video.windowed.multiplier;
      video_settings.hardware_filter   = config::video.windowed.hardware_filter;
      video_settings.software_filter   = config::video.windowed.software_filter;
    } break;

    case 1: { //fullscreen
      video_settings.aspect_correction = config::video.fullscreen.aspect_correction;
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

  audio.set(Audio::Resample, outfreq != infreq);  //only resample when necessary
  audio.set(Audio::ResampleOutputFrequency, outfreq);
  audio.set(Audio::ResampleInputFrequency,  infreq);

  window_main.sync();
}

void modify_system_state(system_state_t state) {
  video.clear();
  audio.clear();

  switch(state) {
    case LoadCart: {
      if(cartridge.loaded() == false) break;

      app.power = true;
      app.pause = false;
      snes.power();

      status.flush();
      string t = translate["Loaded $."];
      replace(t, "$", cartridge.info.filename);
      status.enqueue(t);
      if(cartridge.info.patched) status.enqueue(translate["UPS patch applied."]);

      //warn if unsupported hardware detected
      string message;
      message = translate["Warning: unsupported $ chip detected."];
      if(cartridge.info.superfx) { replace(message, "$", "SuperFX"); status.enqueue(message); }
      if(cartridge.info.sa1)     { replace(message, "$", "SA-1");    status.enqueue(message); }
      if(cartridge.info.st011)   { replace(message, "$", "ST011");   status.enqueue(message); }
      if(cartridge.info.st018)   { replace(message, "$", "ST018");   status.enqueue(message); }

      message = translate["Warning: partially supported $ chip detected."];
      if(cartridge.info.dsp3)    { replace(message, "$", "DSP-3");   status.enqueue(message); }
    } break;

    case UnloadCart: {
      if(cartridge.loaded() == true) break;
      cartridge.unload();

      app.power = false;
      app.pause = true;

      status.flush();
      string t = translate["Unloaded $."];
      replace(t, "$", cartridge.info.filename);
      status.enqueue(t);
    } break;

    case PowerOn: {
      if(cartridge.loaded() == false || app.power == true) break;

      app.power = true;
      app.pause = false;
      snes.power();

      status.flush();
      status.enqueue(translate["Power on."]);
    } break;

    case PowerOff: {
      if(cartridge.loaded() == false || app.power == false) break;

      app.power = false;
      app.pause = true;

      status.flush();
      status.enqueue(translate["Power off."]);
    } break;

    case PowerCycle: {
      if(cartridge.loaded() == false) break;

      app.power = true;
      app.pause = false;
      snes.power();

      status.flush();
      status.enqueue(translate["System power was cycled."]);
    } break;

    case Reset: {
      if(cartridge.loaded() == false || app.power == false) break;

      app.pause = false;
      snes.reset();

      status.flush();
      status.enqueue(translate["System was reset."]);
    } break;
  }

  window_main.menu_system_power.enable(cartridge.loaded());
  window_main.menu_system_reset.enable(cartridge.loaded() && app.power);
  window_main.menu_system_expansion_port_none.enable(cartridge.loaded() && !app.power);
  window_main.menu_system_expansion_port_bsx.enable(cartridge.loaded() && !app.power);
  window_main.menu_system_region_auto.enable(cartridge.loaded() && !app.power);
  window_main.menu_system_region_ntsc.enable(cartridge.loaded() && !app.power);
  window_main.menu_system_region_pal.enable(cartridge.loaded() && !app.power);
  app.power
    ? window_main.menu_system_power_on.check()
    : window_main.menu_system_power_off.check();
  window_cheat_editor.refresh();
}

void update_controller_port1(int device) {
  unsigned current_device = config::snes.controller_port1;

  if(device != current_device) {
    snes.input.port_set_device(0, config::snes.controller_port1 = device);
  }

  window_main.sync();
}

void update_controller_port2(int device) {
  unsigned current_device = config::snes.controller_port2;

  if(device != current_device) {
    snes.input.port_set_device(1, config::snes.controller_port2 = device);
  }

  window_main.sync();
}

void update_video_settings() {
  load_video_settings();

  SNES::Video::Mode mode = (video_settings.region == 0 ? SNES::Video::ModeNTSC : SNES::Video::ModePAL);
  unsigned width = 256;
  unsigned height = (mode == SNES::Video::ModeNTSC ? 224 : 239);
  unsigned multiplier = max(1, min(5, video_settings.multiplier));

  width  *= multiplier;
  height *= multiplier;
  if(video_settings.aspect_correction == true) {
    double scalar;
    if(mode == SNES::Video::ModeNTSC) {
      scalar = (double)config::video.aspect_ntsc_x / (double)config::video.aspect_ntsc_y;
    } else {
      scalar = (double)config::video.aspect_pal_x  / (double)config::video.aspect_pal_y;
    }
    width = (unsigned)((double)width * (double)scalar);
  }

  if(video_settings.mode == 0) {
    window_main.unfullscreen();
    window_main.resize(width, height);
  } else {
    window_main.fullscreen();
  }

  //get maximum possible size for visible area
  unsigned viewwidth  = window_main.get_width();
  unsigned viewheight = window_main.get_height();

  //if requested size exceeds visible area,
  //constrain proportions and preserve aspect ratio
  if(height > viewheight) {
    double scalar = (double)viewheight / (double)height;
    width  = (unsigned)((double)width * (double)scalar);
    height = viewheight;
  }

  if(width > viewwidth) {
    double scalar = (double)viewwidth / (double)width;
    width  = viewwidth;
    height = (unsigned)((double)height * (double)scalar);
  }

  //center video output within visible area
  window_main.move(window_main.view, (viewwidth - width) / 2, (viewheight - height) / 2);
  window_main.view.resize(width, height);

  libfilter::FilterInterface::FilterType filter;
  switch(video_settings.software_filter) { default:
    case 0: filter = libfilter::FilterInterface::Direct;   break;
    case 1: filter = libfilter::FilterInterface::Scanline; break;
    case 2: filter = libfilter::FilterInterface::Scale2x;  break;
    case 3: filter = libfilter::FilterInterface::HQ2x;     break;
    case 4: filter = libfilter::FilterInterface::NTSC;     break;
  }

  libfilter::filter.set(filter);
  snes.video.set_mode(mode);
  video.set(Video::Synchronize, video_settings.synchronize);
  video.set(Video::Filter, video_settings.hardware_filter);
  window_main.sync();  //update main window video mode checkbox settings
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

bool load_cart(char *fn) {
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

//used by File -> Load ROM and "Load Cartridge" hotkey
void load_cart() {
  char fn[PATH_MAX];
  if(load_cart(fn) == false) return;
  load_image(fn);
}

void load_image(const char *filename) {
  Cartridge::cartinfo_t cartinfo;
  if(!cartridge.inspect_image(cartinfo, filename)) return;

  switch(cartinfo.type) {
    case Cartridge::TypeNormal: {
      load_cart_normal(filename);
    } break;

    case Cartridge::TypeBSC: {
      window_bsxloader.mode = BSXLoaderWindow::ModeBSC;
      window_bsxloader.set_text(translate["Load BS-X Slotted Cartridge"]);
      window_bsxloader.tbase.set_text(filename);
      window_bsxloader.tslot.set_text("");
      window_bsxloader.load.focus();
      window_bsxloader.focus();
    } break;

    case Cartridge::TypeBSXBIOS: {
      window_bsxloader.mode = BSXLoaderWindow::ModeBSX;
      window_bsxloader.set_text(translate["Load BS-X Cartridge"]);
      window_bsxloader.tbase.set_text(filename);
      window_bsxloader.tslot.set_text("");
      window_bsxloader.load.focus();
      window_bsxloader.focus();
    } break;

    case Cartridge::TypeBSX: {
      window_bsxloader.mode = BSXLoaderWindow::ModeBSX;
      window_bsxloader.set_text(translate["Load BS-X Cartridge"]);
      window_bsxloader.tbase.set_text(config::path.bsx);
      window_bsxloader.tslot.set_text(filename);
      window_bsxloader.load.focus();
      window_bsxloader.focus();
    } break;

    case Cartridge::TypeSufamiTurboBIOS: {
      window_stloader.tbase.set_text(filename);
      window_stloader.tslotA.set_text("");
      window_stloader.tslotB.set_text("");
      window_stloader.load.focus();
      window_stloader.focus();
    } break;

    case Cartridge::TypeSufamiTurbo: {
      window_stloader.tbase.set_text(config::path.st);
      window_stloader.tslotA.set_text(filename);
      window_stloader.tslotB.set_text("");
      window_stloader.load.focus();
      window_stloader.focus();
    } break;
  }
}

void load_cart_normal(const char *base) {
  if(!base || !*base) return;

  unload_cart();
  cartridge.load_cart_normal(base);
  if(cartridge.loaded() == false) return;
  modify_system_state(LoadCart);
}

void load_cart_bsc(const char *base, const char *slot) {
  if(!base || !*base) return;

  unload_cart();
  cartridge.load_cart_bsc(base, slot);
  if(cartridge.loaded() == false) return;
  modify_system_state(LoadCart);
}

void load_cart_bsx(const char *base, const char *slot) {
  if(!base || !*base) return;

  unload_cart();
  cartridge.load_cart_bsx(base, slot);
  if(cartridge.loaded() == false) return;
  modify_system_state(LoadCart);
}

void load_cart_st(const char *base, const char *slotA, const char *slotB) {
  if(!base || !*base) return;

  unload_cart();
  cartridge.load_cart_st(base, slotA, slotB);
  if(cartridge.loaded() == false) return;
  modify_system_state(LoadCart);
}

void unload_cart() {
  if(cartridge.loaded() == false) return;
  cartridge.unload();
  modify_system_state(UnloadCart);
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
