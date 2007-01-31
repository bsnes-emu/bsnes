namespace event {

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

  return window_main.file_load(fn,
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

void load_rom_normal() {
char fn[4096];
  if(load_rom(fn) == false)return;

  if(cartridge.loaded() == true)cartridge.unload();
  cartridge.load_begin(Cartridge::CART_NORMAL);
  cartridge.load(fn);
  cartridge.load_end();
  snes.power();
}

void unload_rom() {
  cartridge.unload();
  uiAudio->clear_audio();
}

};
