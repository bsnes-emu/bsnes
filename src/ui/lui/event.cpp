namespace event {

void update_frame_counter() {
  if(r_ppu->status.frames_updated) {
    r_ppu->status.frames_updated = false;
    if(bool(true) == true) { //TODO: add config file variable to toggle fps counter
      window_main.set_text("%s [%d]", BSNES_TITLE, r_ppu->status.frames_executed);
    }
  }
}

void update_video_settings() {
uint width  = 256;
uint height = config::video.region == 0 ? 224 : 239;
uint multiplier = minmax<1, 5>(uint(config::video.multiplier));
  width  *= multiplier;
  height *= multiplier;
  if(config::video.aspect_correction == true) {
    width = uint( double(width) * 8.0 / 7.0 );
  }
  window_main.resize(width, height);
  window_main.view.resize(width, height);
}

void update_raster_settings() {
uint filter, standard;
  switch(uint(config::video.software_filter)) { default:
  case 0: filter = VIDEOFILTER_DIRECT;  break;
  case 1: filter = VIDEOFILTER_NTSC;    break;
  case 2: filter = VIDEOFILTER_HQ2X;    break;
  case 3: filter = VIDEOFILTER_SCALE2X; break;
  }

  switch(uint(config::video.region)) { default:
  case 0: standard = SNES::VIDEOSTANDARD_NTSC; break;
  case 1: standard = SNES::VIDEOSTANDARD_PAL;  break;
  }

  snes.set_video_filter(filter);
  snes.set_video_standard(standard);
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
  cartridge.load_begin(Cartridge::CART_NORMAL);
  cartridge.load(fn);
  cartridge.load_end();
  snes.power();
}

void unload_rom() {
  if(cartridge.loaded() == true) {
    cartridge.unload();
    uiVideo->clear_video();
    uiAudio->clear_audio();
  }
  window_main.set_text(BSNES_TITLE);
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
