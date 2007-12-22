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

void update_speed_regulation(uint speed) {
  config::system.speed = max(1, min(speed, 5));
  if(uiAudio && uiAudio->cap(Audio::Frequency)) {
    switch(config::system.speed) {
      case 1: uiAudio->set(Audio::Frequency, 16000); break;
      case 2: uiAudio->set(Audio::Frequency, 24000); break;
      case 3: uiAudio->set(Audio::Frequency, 32000); break;
      case 4: uiAudio->set(Audio::Frequency, 48000); break;
      case 5: uiAudio->set(Audio::Frequency, 64000); break;
    }
  }
}

void update_frame_counter() {
  if(ppu.status.frames_updated) {
    ppu.status.frames_updated = false;
    if(config::misc.show_frame_counter == true) {
      window_main.set_text(string() << BSNES_TITLE << " [" << ppu.status.frames_executed << "]");
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
    case 0: { //windowed
      window_main.unfullscreen();
      window_main.resize(width, height);
      window_main.move(window_main.view, 0, 0);
      window_main.view.resize(width, height);
    } break;
    case 1: { //fullscreen
      window_main.fullscreen();
      window_main.move(window_main.view,
        (miu().screen_width() - width) / 2,
        (miu().screen_height() - height) / 2);
      window_main.view.resize(width, height);
    } break;
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
    uiVideo->set(Video::Synchronize, video_settings.synchronize);
    uiVideo->set(Video::Filter, video_settings.hardware_filter);
  }

//update main window video mode checkbox settings
  window_main.update_menu_settings();
}

void toggle_menu() {
  window_main.menu_show(!window_main.menu_visible());
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

  return miu().file_load(0, fn,
    "SNES images;*.smc,*.sfc,*.swc,*.fig,*.bs,*.st"
  #if defined(GZIP_SUPPORT)
    ",*.gz,*.z,*.zip"
  #endif
  #if defined(JMA_SUPPORT)
    ",*.jma"
  #endif
    "|All files;*.*",
    dir[0]);
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

//warn if unsupported hardware detected
  if(cartridge.info.superfx) alert("Warning: unsupported SuperFX chip detected.");
  if(cartridge.info.sa1)     alert("Warning: unsupported SA-1 chip detected.");
  if(cartridge.info.st011)   alert("Warning: unsupported ST011 chip detected.");
  if(cartridge.info.st018)   alert("Warning: unsupported ST018 chip detected.");

  snes.power();
  window_cheat_editor.refresh();
}

void load_cart_bsx(const char *base, const char *slot) {
  if(!base || !*base) return;

  if(cartridge.loaded() == true) cartridge.unload();
  cartridge.load_cart_bsx(base, slot);

  snes.power();
  window_cheat_editor.refresh();
}

void load_cart_bsc(const char *base, const char *slot) {
  if(!base || !*base) return;

  if(cartridge.loaded() == true) cartridge.unload();
  cartridge.load_cart_bsc(base, slot);

  snes.power();
  window_cheat_editor.refresh();
}

void load_cart_st(const char *base, const char *slotA, const char *slotB) {
  if(!base || !*base) return;

  if(cartridge.loaded() == true) cartridge.unload();
  cartridge.load_cart_st(base, slotA, slotB);

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
