/*****
 * event namespace
 *
 * Used to invoke common functions from anywhere within
 * the bsnes user interface.
 * For example, when unloading a ROM image, many things
 * must happen. The video, audio, and input buffers must
 * be cleared, the debugger must be disabled, and the
 * main window must be updated. Rather than binding this
 * functionality inside the main window class, it is
 * added here. This then makes it possible to easily add
 * an option to the main window, as well as a key binding,
 * as well as an option elsewhere to unload the ROM.
 *
 * The main purpose of the event namespace is to simplify
 * binding common functions to user-specified keyboard
 * combinations.
 *****/

namespace event {

void update_video_settings() {
uint width  = 256;
uint height = config::video.region == 0 ? 224 : 239;
uint multiplier = minmax<1, 5>(uint(config::video.multiplier));
  width  *= multiplier;
  height *= multiplier;
  if(config::video.aspect_correction == true) {
    width = uint( double(width) * 8.0 / 7.0 );
  }
  wMain.Resize(width, height, true);
}

bool load_rom(char *fn) {
OPENFILENAME ofn;
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

//GetOpenFileName doesn't like forward slashes in its directory path
  replace(dir, "/", "\\");

  memset(&ofn, 0, sizeof(ofn));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner   = wMain.hwnd;
  ofn.lpstrFilter = "SNES ROM Images (*.smc;*.sfc;*.swc;*.fig;*.ufo;*.gd3;*.078;*.st"
#ifdef GZIP_SUPPORT
                      ";.gz;.z;.zip"
#endif
#ifdef JMA_SUPPORT
                      ";.jma"
#endif
                      ")\0"
                      "*.smc;*.sfc;*.swc;*.fig;*.ufo;*.gd3;*.078;*.st"
#ifdef GZIP_SUPPORT
                      ";*.gz;*.z;*.zip"
#endif
#ifdef JMA_SUPPORT
                      ";*.jma"
#endif
                      "\0"
                    "All Files (*.*)\0"
                      "*.*\0";
  ofn.lpstrFile       = fn;
  ofn.lpstrInitialDir = strptr(dir);
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "smc";

  if(!GetOpenFileName(&ofn))return false;
  return true;
}

void load_rom_normal() {
char fn[MAX_PATH];
  if(load_rom(fn) == false)return;

  if(cartridge.loaded() == true)cartridge.unload();
  wDebug.Clear();

  cartridge.load_begin(Cartridge::CART_NORMAL);
  cartridge.load(fn);
  cartridge.load_end();
  snes.power();

  wCheatEditor.Refresh();
}

void load_rom_st() {
char fn[MAX_PATH];
  if(load_rom(fn) == false)return;

  if(cartridge.loaded() == true)cartridge.unload();
  wDebug.Clear();

  cartridge.load_begin(Cartridge::CART_ST);
  cartridge.load(fn);
  cartridge.load_end();
  snes.power();

  wCheatEditor.Refresh();
}

void load_rom_stdual() {
char fn_a[MAX_PATH], fn_b[MAX_PATH];
  if(load_rom(fn_a) == false)return;
  if(load_rom(fn_b) == false)return;

  if(cartridge.loaded() == true)cartridge.unload();
  wDebug.Clear();

  cartridge.load_begin(Cartridge::CART_STDUAL);
  cartridge.load(fn_a);
  cartridge.load(fn_b);
  cartridge.load_end();
  snes.power();

  wCheatEditor.Refresh();
}

void unload_rom() {
  if(cartridge.loaded() == false)return;

  cartridge.unload();
  uiVideo->clear_video();
  uiAudio->clear_audio();
  uiInput->clear_input();
  SetWindowText(wMain.hwnd, BSNES_TITLE);

  debugger.refresh();
  wCheatEditor.Clear();
}

};
