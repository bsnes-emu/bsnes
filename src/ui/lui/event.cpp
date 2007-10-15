namespace event {

void load_video_settings() {
  video_settings.mode = config::video.mode;
  switch(video_settings.mode) { default:
  case 0: //windowed
    video_settings.aspect_correction = config::video.windowed.aspect_correction;
    video_settings.synchronize       = config::video.windowed.synchronize;
    video_settings.region            = config::video.windowed.region;
    video_settings.multiplier        = config::video.windowed.multiplier;
    video_settings.hardware_filter   = config::video.windowed.hardware_filter;
    video_settings.software_filter   = config::video.windowed.software_filter;
    break;
  case 1: //fullscreen
    video_settings.aspect_correction = config::video.fullscreen.aspect_correction;
    video_settings.synchronize       = config::video.fullscreen.synchronize;
    video_settings.region            = config::video.fullscreen.region;
    video_settings.multiplier        = config::video.fullscreen.multiplier;
    video_settings.hardware_filter   = config::video.fullscreen.hardware_filter;
    video_settings.software_filter   = config::video.fullscreen.software_filter;
    break;
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

void update_frame_counter() {
  if(r_ppu->status.frames_updated) {
    r_ppu->status.frames_updated = false;
    if(config::misc.show_frame_counter == true) {
      window_main.set_text(string() << BSNES_TITLE << " [" << r_ppu->status.frames_executed << "]");
    }
  }
}

void update_video_settings() {
  load_video_settings();

uint width  = 256;
uint height = video_settings.region == 0 ? 224 : 239;
uint multiplier = minmax<1, 5>(video_settings.multiplier);
  width  *= multiplier;
  height *= multiplier;
  if(video_settings.aspect_correction == true) {
    if(video_settings.region == 0) { //NTSC
      width = uint( double(width) * double(config::video.aspect_ntsc_x) / double(config::video.aspect_ntsc_y) );
    } else { //PAL
      width = uint( double(width) * double(config::video.aspect_pal_x)  / double(config::video.aspect_pal_y)  );
    }
  }

  switch(video_settings.mode) { default:
  case 0: //windowed
    window_main.unfullscreen();
    window_main.resize(width, height);
    window_main.view.move(0, 0);
    window_main.view.resize(width, height);
    break;
  case 1: //fullscreen
    window_main.fullscreen();
    window_main.view.move((ui::get_screen_width() - width) / 2, (ui::get_screen_height() - height) / 2);
    window_main.view.resize(width, height);
    break;
  }

uint filter, standard;
  switch(video_settings.software_filter) { default:
  case 0: filter = VIDEOFILTER_DIRECT;  break;
  case 1: filter = VIDEOFILTER_NTSC;    break;
  case 2: filter = VIDEOFILTER_HQ2X;    break;
  case 3: filter = VIDEOFILTER_SCALE2X; break;
  }

  switch(video_settings.region) { default:
  case 0: standard = SNES::VIDEOSTANDARD_NTSC; break;
  case 1: standard = SNES::VIDEOSTANDARD_PAL;  break;
  }

  snes.set_video_filter(filter);
  snes.set_video_standard(standard);

  if(uiVideo) {
    uiVideo->settings.synchronize = video_settings.synchronize;
    uiVideo->settings.filter = video_settings.hardware_filter;
    uiVideo->update_settings();
  }

//update main window video mode checkbox settings
  window_main.update_menu_settings();
}

void toggle_menu() {
  window_main.menu.show(!window_main.menu.visible());
  update_video_settings();
}

void toggle_fullscreen() {
  if(config::video.mode != 1) { //switch to fullscreen mode if not already in it
    config::video.mode = 1;
  } else { //switch to windowed mode if already in fullscreen mode
    config::video.mode = 0;
  }
  update_video_settings();
}

//

bool load_rom(char *fn) {
stringarray dir;
  strcpy(fn, "");
  strcpy(dir, config::path.rom);
  replace(dir, "\\", "/");
  if(strlen(dir) && !strend(dir, "/")) { strcat(dir, "/"); }

//append base_path if rom_path is relative
  if(strbegin(dir, "./")) {
    strltrim(dir, "./");
    strcpy(dir[1], dir[0]);
    strcpy(dir[0], config::path.base);
    strcat(dir[0], dir[1]);
  }

  return ui::file_load(&window_main, fn,
    "SNES images;*.smc,*.sfc,*.swc,*.fig,*.ufo,*.gd3,*.078,*.st"
  #if defined(GZIP_SUPPORT)
    ",*.gz,*.z,*.zip"
  #endif
  #if defined(JMA_SUPPORT)
    ",*.jma"
  #endif
    "|All files;*.*",
    strptr(dir));
}

void load_rom() {
char fn[PATH_MAX];
  if(load_rom(fn) == false)return;

  if(cartridge.loaded() == true)cartridge.unload();
  cartridge.load_begin(Cartridge::CartridgeNormal);
  cartridge.load(fn);
  cartridge.load_end();

//warn if unsupported hardware detected
  if(cartridge.info.superfx)alert("Warning: unsupported SuperFX chip detected.");
  if(cartridge.info.sa1)    alert("Warning: unsupported SA-1 chip detected.");
  if(cartridge.info.st011)  alert("Warning: unsupported ST011 chip detected.");
  if(cartridge.info.st018)  alert("Warning: unsupported ST018 chip detected.");

  snes.power();
  window_cheat_editor.refresh();
}

void load_rom_st() {
char fn[PATH_MAX];
  if(load_rom(fn) == false)return;

  if(cartridge.loaded() == true)cartridge.unload();
  cartridge.load_begin(Cartridge::CartridgeSufamiTurbo);
  cartridge.load(fn);
  cartridge.load_end();
  snes.power();
  window_cheat_editor.refresh();
}

void load_rom_stdual() {
char fn_a[PATH_MAX], fn_b[PATH_MAX];
  if(load_rom(fn_a) == false)return;
  if(load_rom(fn_b) == false)return;

  if(cartridge.loaded() == true)cartridge.unload();
  cartridge.load_begin(Cartridge::CartridgeSufamiTurboDual);
  cartridge.load(fn_a);
  cartridge.load(fn_b);
  cartridge.load_end();
  snes.power();
  window_cheat_editor.refresh();
}

void unload_rom() {
  if(cartridge.loaded() == true) {
    cartridge.unload();
    uiVideo->clear_video();
    uiAudio->clear_audio();
  }
  window_main.set_text(BSNES_TITLE);
  window_cheat_editor.refresh();
}

void reset() {
  if(cartridge.loaded() == true) {
    snes.reset();
    dprintf("* Reset");
  }
}

void power() {
  if(cartridge.loaded() == true) {
    snes.power();
    dprintf("* Power");
  }
}

};
