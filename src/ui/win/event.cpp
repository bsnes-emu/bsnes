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

void capture_screenshot() {
  uiVideo->capture_screenshot();
}

void set_video_profile(uint profile) {
  if(profile > 7)profile = 0;

  config::video.profile = profile;
  uiVideo->update_video_settings();

string t;
  if(bool(config::video.fullscreen) == true) {
    strcpy(t, "topmost|popup");
    if(wMain.Visible())strcat(t, "|visible");
    wMain.HideMenu();
    HideCursor();
  } else {
    strcpy(t, config::misc.window_style.sget());
    if(wMain.Visible())strcat(t, "|visible");
    wMain.ShowMenu();
    ShowCursor();
  }
  wMain.SetStyle(strptr(t));
  wMain.Resize(uiVideo->settings.resolution_width, uiVideo->settings.resolution_height, true);

  uiVideo->update_video_profile();

  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_0, profile == 0);
  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_1, profile == 1);
  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_2, profile == 2);
  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_3, profile == 3);
  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_4, profile == 4);
  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_5, profile == 5);
  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_6, profile == 6);
  wMain.CheckMenuItem(MENU_SETTINGS_VIDEOPROFILE_7, profile == 7);
}

void toggle_fullscreen() {
  config::video.fullscreen.toggle();
  if(bool(config::video.fullscreen) == true) {
    wSettings.Hide();
    wAbout.Hide();
    debugger.deactivate();
  }
  event::set_video_profile(config::video.profile);
}

void load_rom() {
OPENFILENAME ofn;
char t[MAX_PATH];
string dir;
  strcpy(t, "");
  strcpy(dir, config::fs.rom_path.sget());

//append base path if rom path is relative
  if(strbegin(dir, "./")) {
    strltrim(dir, "./");
    strcpy(dir[1], dir[0]);
    strcpy(dir[0], config::fs.base_path.sget());
    strcat(dir[0], dir[1]);
  }

//GetOpenFileName doesn't like forward slashes in its directory path
  replace(dir, "/", "\\");

  memset(&ofn, 0, sizeof(ofn));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner   = wMain.hwnd;
  ofn.lpstrFilter = "SNES ROM Images (*.smc;*.sfc;*.swc;*.fig;*.ufo;*.gd3;*.078"
#ifdef GZIP_SUPPORT
                      ";.gz;.z;.zip"
#endif
#ifdef JMA_SUPPORT
                      ";.jma"
#endif
                      ")\0"
                      "*.smc;*.sfc;*.swc;*.fig;*.ufo;*.gd3;*.078"
#ifdef GZIP_SUPPORT
                      ";*.gz;*.z;*.zip"
#endif
#ifdef JMA_SUPPORT
                      ";*.jma"
#endif
                      "\0"
                    "All Files (*.*)\0"
                      "*.*\0";
  ofn.lpstrFile       = t;
  ofn.lpstrInitialDir = strptr(dir);
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "smc";

  if(!GetOpenFileName(&ofn))return;

  if(cartridge.loaded() == true)cartridge.unload();
  wDebug.Clear();

  if(cartridge.load(t) == false)return;
  wCheatEditor.Refresh();

  bsnes->power();
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
