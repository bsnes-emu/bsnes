void MainWindow::get_video_mode_info(uint8 mode) {
string t;
  if(mode > 9)mode = 0;

  switch(mode) {
  case 0: strcpy(t, config::video.mode0.sget()); break;
  case 1: strcpy(t, config::video.mode1.sget()); break;
  case 2: strcpy(t, config::video.mode2.sget()); break;
  case 3: strcpy(t, config::video.mode3.sget()); break;
  case 4: strcpy(t, config::video.mode4.sget()); break;
  case 5: strcpy(t, config::video.mode5.sget()); break;
  case 6: strcpy(t, config::video.mode6.sget()); break;
  case 7: strcpy(t, config::video.mode7.sget()); break;
  case 8: strcpy(t, config::video.mode8.sget()); break;
  case 9: strcpy(t, config::video.mode9.sget()); break;
  }

  vi.fullscreen = (mode >= 5);
char *p = strptr(t);
  vi.x = strdec(p);
  while(*p && *p != 'x')p++;
  vi.y = strdec(++p);
  if(mode < 5)return;
  while(*p && *p != '@')p++;
  vi.refresh_rate = strdec(++p);
  while(*p && *p != ':')p++;
  vi.width = strdec(++p);
  while(*p && *p != 'x')p++;
  vi.height = strdec(++p);
}

void MainWindow::set_video_mode(uint8 mode) {
  if(mode > 9)mode = 0;
  get_video_mode_info(mode);

  config::video.mode = mode;

  uiVideo->set_video_mode(vi.x, vi.y, vi.fullscreen,
    vi.refresh_rate, vi.width, vi.height);

  check(MENU_SETTINGS_VIDEOMODE_0, mode == 0);
  check(MENU_SETTINGS_VIDEOMODE_1, mode == 1);
  check(MENU_SETTINGS_VIDEOMODE_2, mode == 2);
  check(MENU_SETTINGS_VIDEOMODE_3, mode == 3);
  check(MENU_SETTINGS_VIDEOMODE_4, mode == 4);
  check(MENU_SETTINGS_VIDEOMODE_5, mode == 5);
  check(MENU_SETTINGS_VIDEOMODE_6, mode == 6);
  check(MENU_SETTINGS_VIDEOMODE_7, mode == 7);
  check(MENU_SETTINGS_VIDEOMODE_8, mode == 8);
  check(MENU_SETTINGS_VIDEOMODE_9, mode == 9);
}

void MainWindow::set_frameskip(uint8 fs) {
  frameskip     = fs % 10;
  frameskip_pos = 0;

  check(MENU_SETTINGS_FRAMESKIP_0, fs == 0);
  check(MENU_SETTINGS_FRAMESKIP_1, fs == 1);
  check(MENU_SETTINGS_FRAMESKIP_2, fs == 2);
  check(MENU_SETTINGS_FRAMESKIP_3, fs == 3);
  check(MENU_SETTINGS_FRAMESKIP_4, fs == 4);
  check(MENU_SETTINGS_FRAMESKIP_5, fs == 5);
  check(MENU_SETTINGS_FRAMESKIP_6, fs == 6);
  check(MENU_SETTINGS_FRAMESKIP_7, fs == 7);
  check(MENU_SETTINGS_FRAMESKIP_8, fs == 8);
  check(MENU_SETTINGS_FRAMESKIP_9, fs == 9);
}

void MainWindow::set_regulation_speed(uint8 speed) {
  speed %= 5;
  regulation_speed = speed;

  switch(speed) {
  case 0: uiAudio->set_frequency(config::system.speed_slowest); break;
  case 1: uiAudio->set_frequency(config::system.speed_slow);    break;
  case 2: uiAudio->set_frequency(config::system.speed_normal);  break;
  case 3: uiAudio->set_frequency(config::system.speed_fast);    break;
  case 4: uiAudio->set_frequency(config::system.speed_fastest); break;
  }

  check(MENU_SETTINGS_SPEED_REGULATION_SLOWEST, speed == 0);
  check(MENU_SETTINGS_SPEED_REGULATION_SLOW,    speed == 1);
  check(MENU_SETTINGS_SPEED_REGULATION_NORMAL,  speed == 2);
  check(MENU_SETTINGS_SPEED_REGULATION_FAST,    speed == 3);
  check(MENU_SETTINGS_SPEED_REGULATION_FASTEST, speed == 4);
}

void MainWindow::menu_load() {
OPENFILENAME ofn;
char t[4096];
  strcpy(t, "");
  memset(&ofn, 0, sizeof(ofn));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner   = hwnd;
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
  ofn.lpstrFile   = t;
  ofn.nMaxFile    = MAX_PATH;
  ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt = "smc";

  if(!GetOpenFileName(&ofn))return;

  cartridge.unload();
  dprintf("");
  cartridge.load(t);

  bsnes->power();
}

void MainWindow::menu_unload() {
  cartridge.unload();
  uiVideo->clear_video();
  uiAudio->clear_audio();
  uiInput->clear_input();
  SetWindowText(hwnd, BSNES_TITLE);
}

long MainWindow::wndproc(UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {
  case WM_ENTERMENULOOP:
    uiAudio->clear_audio();
    break;
  case WM_EXITMENULOOP: {
  time_t timeout = time(0);
    while(difftime(time(0), timeout) < 3) {
      if(!KeyDown(VK_RETURN))break;
    }
    uiInput->clear_input();
    break;
  }
  case WM_KEYDOWN:
    if(wparam == VK_ESCAPE) {
      if(GetMenu(hwnd) == NULL) {
        show_menu();
      } else {
        hide_menu();
      }
      center();
    } else if(wparam == VK_PAUSE) {
      if(bsnes->get_state() == bSNES::RUN) {
        uiAudio->clear_audio();
        bsnes->set_state(bSNES::PAUSE);
        SetWindowText(hwnd, BSNES_TITLE " (Paused)");
      } else if(bsnes->get_state() == bSNES::PAUSE) {
        bsnes->set_state(bSNES::RUN);
      }
    } else if(wparam >= '0' && wparam <= '9' && KeyDown(VK_CONTROL)) {
      set_video_mode((wparam != '0') ? (wparam - '0' - 1) : 9);
    } else if(wparam == VK_SUBTRACT || (wparam == VK_OEM_MINUS && !KeyDown(VK_CONTROL))) {
      if(frameskip > 0)set_frameskip(frameskip - 1);
    } else if(wparam == VK_ADD || (wparam == VK_OEM_PLUS && !KeyDown(VK_CONTROL))) {
      if(frameskip < 9)set_frameskip(frameskip + 1);
    } else if(wparam == VK_OEM_MINUS && KeyDown(VK_CONTROL)) {
      if(regulation_speed > 0)set_regulation_speed(regulation_speed - 1);
    } else if(wparam == VK_OEM_PLUS && KeyDown(VK_CONTROL)) {
      if(regulation_speed < 4)set_regulation_speed(regulation_speed + 1);
    }
    break;
  case WM_PAINT:
    if(r_mem->cart_loaded() == true) {
      uiVideo->redraw();
    } else {
      uiVideo->clear_video();
    }
    break;
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case MENU_FILE_LOAD:
      menu_load();
      break;
    case MENU_FILE_UNLOAD:
      menu_unload();
      break;
    case MENU_FILE_RESET:
      bsnes->reset();
      dprintf("* Reset");
      break;
    case MENU_FILE_POWER:
      bsnes->power();
      dprintf("* Power");
      break;
    case MENU_FILE_EXIT:
      PostQuitMessage(0);
      break;
    case MENU_SETTINGS_VIDEOMODE_0:
    case MENU_SETTINGS_VIDEOMODE_1:
    case MENU_SETTINGS_VIDEOMODE_2:
    case MENU_SETTINGS_VIDEOMODE_3:
    case MENU_SETTINGS_VIDEOMODE_4:
    case MENU_SETTINGS_VIDEOMODE_5:
    case MENU_SETTINGS_VIDEOMODE_6:
    case MENU_SETTINGS_VIDEOMODE_7:
    case MENU_SETTINGS_VIDEOMODE_8:
    case MENU_SETTINGS_VIDEOMODE_9:
      set_video_mode(LOWORD(wparam) - MENU_SETTINGS_VIDEOMODE_0);
      break;
    case MENU_SETTINGS_FRAMESKIP_0:
    case MENU_SETTINGS_FRAMESKIP_1:
    case MENU_SETTINGS_FRAMESKIP_2:
    case MENU_SETTINGS_FRAMESKIP_3:
    case MENU_SETTINGS_FRAMESKIP_4:
    case MENU_SETTINGS_FRAMESKIP_5:
    case MENU_SETTINGS_FRAMESKIP_6:
    case MENU_SETTINGS_FRAMESKIP_7:
    case MENU_SETTINGS_FRAMESKIP_8:
    case MENU_SETTINGS_FRAMESKIP_9:
      set_frameskip(LOWORD(wparam) - MENU_SETTINGS_FRAMESKIP_0);
      break;
    case MENU_SETTINGS_COLORADJUST_COLORCURVE:
      config::snes.video_color_curve.toggle();
      check(MENU_SETTINGS_COLORADJUST_COLORCURVE, config::snes.video_color_curve);
      break;
    case MENU_SETTINGS_COLORADJUST_NORMAL:
    case MENU_SETTINGS_COLORADJUST_GRAYSCALE:
    case MENU_SETTINGS_COLORADJUST_VGA:
    case MENU_SETTINGS_COLORADJUST_GENESIS: {
    uint32 i = LOWORD(wparam) - MENU_SETTINGS_COLORADJUST_NORMAL;
      config::snes.video_color_adjust_mode = i;
      check(MENU_SETTINGS_COLORADJUST_NORMAL,    i == 0);
      check(MENU_SETTINGS_COLORADJUST_GRAYSCALE, i == 1);
      check(MENU_SETTINGS_COLORADJUST_VGA,       i == 2);
      check(MENU_SETTINGS_COLORADJUST_GENESIS,   i == 3);
      break;
    }
    case MENU_SETTINGS_VIDEO_OPTIONS_USEVRAM:
      config::video.use_vram.toggle();
      set_video_mode(config::video.mode);
      check(MENU_SETTINGS_VIDEO_OPTIONS_USEVRAM, config::video.use_vram);
      break;
    case MENU_SETTINGS_VIDEO_OPTIONS_TRIPLEBUF:
      config::video.triple_buffering.toggle();
      check(MENU_SETTINGS_VIDEO_OPTIONS_TRIPLEBUF, config::video.triple_buffering);
      break;
    case MENU_SETTINGS_VIDEO_OPTIONS_SHOWFPS:
      config::gui.show_fps.toggle();
      SetWindowText(hwnd, BSNES_TITLE);
      check(MENU_SETTINGS_VIDEO_OPTIONS_SHOWFPS, config::gui.show_fps);
      break;
    case MENU_SETTINGS_VIDEO_OPTIONS_PPUCFG:
      wPPUCfg.show();
      break;
    case MENU_SETTINGS_MUTE:
      config::snes.mute.toggle();
      check(MENU_SETTINGS_MUTE, config::snes.mute);
      break;
    case MENU_SETTINGS_INPUTCFG_JOYPAD1:
      wInputCfg.dev_to_update = SNES::DEV_JOYPAD1;
      wInputCfg.show();
      break;
    case MENU_SETTINGS_INPUTCFG_JOYPAD2:
      wInputCfg.dev_to_update = SNES::DEV_JOYPAD2;
      wInputCfg.show();
      break;
    case MENU_SETTINGS_SPEED_REGULATION_ENABLE:
      config::system.regulate_speed.toggle();
      check(MENU_SETTINGS_SPEED_REGULATION_ENABLE, config::system.regulate_speed);
      break;
    case MENU_SETTINGS_SPEED_REGULATION_SLOWEST:
    case MENU_SETTINGS_SPEED_REGULATION_SLOW:
    case MENU_SETTINGS_SPEED_REGULATION_NORMAL:
    case MENU_SETTINGS_SPEED_REGULATION_FAST:
    case MENU_SETTINGS_SPEED_REGULATION_FASTEST:
      set_regulation_speed(LOWORD(wparam) - MENU_SETTINGS_SPEED_REGULATION_SLOWEST);
      break;
    case MENU_MISC_SCREENSHOT:
      snes->capture_screenshot();
      break;
    case MENU_MISC_LOGAUDIO: {
      if(!checked(MENU_MISC_LOGAUDIO)) {
        check(MENU_MISC_LOGAUDIO);
        snes->log_audio_enable();
      } else {
        uncheck(MENU_MISC_LOGAUDIO);
        snes->log_audio_disable();
      }
      break;
    }
    case MENU_MISC_ABOUT:
      alert(BSNES_TITLE "\n" "Written by: byuu");
      break;
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void MainWindow::setup() {
HMENU submenu, branchmenu;
  menu = CreateMenu();

  submenu = CreatePopupMenu();
  AppendMenu(submenu, MF_STRING, MENU_FILE_LOAD,   "&Load ROM");
  AppendMenu(submenu, MF_STRING, MENU_FILE_UNLOAD, "&Unload ROM");
  AppendMenu(submenu, MF_SEPARATOR, 0, "");
  AppendMenu(submenu, MF_STRING, MENU_FILE_RESET,  "&Reset");
  AppendMenu(submenu, MF_STRING, MENU_FILE_POWER,  "&Power (Hard Reset)");
  AppendMenu(submenu, MF_SEPARATOR, 0, "");
  AppendMenu(submenu, MF_STRING, MENU_FILE_EXIT,   "E&xit");
  AppendMenu(menu, MF_STRING | MF_POPUP, (uint)submenu, "&File");

  submenu = CreatePopupMenu();

  branchmenu = CreatePopupMenu();
char v[512];
  for(int i = 0; i <= 4; i++) {
    get_video_mode_info(i);
    sprintf(v, "%d. %dx%d", i + 1, vi.x, vi.y);
    AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_VIDEOMODE_0 + i, v);
  }
  AppendMenu(branchmenu, MF_SEPARATOR, 0, "");
  for(int i = 5; i <= 9; i++) {
    get_video_mode_info(i);
    sprintf(v, "%d. %dx%d@%dhz <%dx%d>", (i + 1) % 10, vi.x, vi.y, vi.refresh_rate, vi.width, vi.height);
    AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_VIDEOMODE_0 + i, v);
  }
  AppendMenu(submenu, MF_STRING | MF_POPUP, (uint)branchmenu, "&Video Mode");

  branchmenu = CreatePopupMenu();
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_0, "Off");
  AppendMenu(branchmenu, MF_SEPARATOR, 0, "");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_1, "1");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_2, "2");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_3, "3");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_4, "4");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_5, "5");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_6, "6");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_7, "7");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_8, "8");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_FRAMESKIP_9, "9");
  AppendMenu(submenu, MF_STRING | MF_POPUP, (uint)branchmenu, "&Frameskip");

  branchmenu = CreatePopupMenu();
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_COLORADJUST_COLORCURVE, "Use &Color Curve");
  AppendMenu(branchmenu, MF_SEPARATOR, 0, "");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_COLORADJUST_NORMAL,    "Normal (RGB555)");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_COLORADJUST_GRAYSCALE, "Grayscale Mode (L5)");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_COLORADJUST_VGA,       "VGA Mode (RGB332)");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_COLORADJUST_GENESIS,   "Genesis Mode (RGB333)");
  AppendMenu(submenu, MF_STRING | MF_POPUP, (uint)branchmenu, "&Color Adjust");

  branchmenu = CreatePopupMenu();
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_VIDEO_OPTIONS_USEVRAM,   "Use &Video Memory Surface");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_VIDEO_OPTIONS_TRIPLEBUF, "Use &Triple Buffering");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_VIDEO_OPTIONS_SHOWFPS,   "&Show FPS");
  AppendMenu(branchmenu, MF_SEPARATOR, 0, "");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_VIDEO_OPTIONS_PPUCFG,    "&PPU Configuration");
  AppendMenu(submenu, MF_STRING | MF_POPUP, (uint)branchmenu, "&Video Options");
  AppendMenu(submenu, MF_SEPARATOR, 0, "");
  AppendMenu(submenu, MF_STRING, MENU_SETTINGS_MUTE, "&Mute Sound Output");
  AppendMenu(submenu, MF_SEPARATOR, 0, "");

  branchmenu = CreatePopupMenu();
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_INPUTCFG_JOYPAD1, "Joypad 1");
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_INPUTCFG_JOYPAD2, "Joypad 2");
  AppendMenu(submenu, MF_STRING | MF_POPUP, (uint)branchmenu, "Configure &Input Devices");

  AppendMenu(submenu, MF_SEPARATOR, 0, "");

  branchmenu = CreatePopupMenu();
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_SPEED_REGULATION_ENABLE, "&Enable");
  AppendMenu(branchmenu, MF_SEPARATOR, 0, "");
  sprintf(v, "&Slowest (%d%%, %dhz)", uint(100.0 * ((double)config::system.speed_slowest / 32000.0)),
    (uint)config::system.speed_slowest);
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_SPEED_REGULATION_SLOWEST, v);
  sprintf(v, "&Slow (%d%%, %dhz)", uint(100.0 * ((double)config::system.speed_slow / 32000.0)),
    (uint)config::system.speed_slow);
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_SPEED_REGULATION_SLOW, v);
  sprintf(v, "&Normal (%d%%, %dhz)", uint(100.0 * ((double)config::system.speed_normal / 32000.0)),
    (uint)config::system.speed_normal);
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_SPEED_REGULATION_NORMAL, v);
  sprintf(v, "&Fast (%d%%, %dhz)", uint(100.0 * ((double)config::system.speed_fast / 32000.0)),
    (uint)config::system.speed_fast);
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_SPEED_REGULATION_FAST, v);
  sprintf(v, "&Fastest (%d%%, %dhz)", uint(100.0 * ((double)config::system.speed_fastest / 32000.0)),
    (uint)config::system.speed_fastest);
  AppendMenu(branchmenu, MF_STRING, MENU_SETTINGS_SPEED_REGULATION_FASTEST, v);
  AppendMenu(submenu, MF_STRING | MF_POPUP, (uint)branchmenu, "&Speed Regulation");

  AppendMenu(menu, MF_STRING | MF_POPUP, (uint)submenu, "&Settings");

  submenu = CreatePopupMenu();
  AppendMenu(submenu, MF_STRING, MENU_MISC_SCREENSHOT, "&Capture Screenshot");
  AppendMenu(submenu, MF_STRING, MENU_MISC_LOGAUDIO,   "&Log Audio Data");
  AppendMenu(submenu, MF_SEPARATOR, 0, "");
  AppendMenu(submenu, MF_STRING, MENU_MISC_ABOUT, "&About");
  AppendMenu(menu, MF_STRING | MF_POPUP, (uint)submenu, "&Misc");

  show_menu();

  check(MENU_SETTINGS_COLORADJUST_COLORCURVE,  config::snes.video_color_curve);
int32 i = config::snes.video_color_adjust_mode;
  check(MENU_SETTINGS_COLORADJUST_NORMAL,      i == 0);
  check(MENU_SETTINGS_COLORADJUST_GRAYSCALE,   i == 1);
  check(MENU_SETTINGS_COLORADJUST_VGA,         i == 2);
  check(MENU_SETTINGS_COLORADJUST_GENESIS,     i == 3);
  check(MENU_SETTINGS_VIDEO_OPTIONS_USEVRAM,   config::video.use_vram);
  check(MENU_SETTINGS_VIDEO_OPTIONS_TRIPLEBUF, config::video.triple_buffering);
  check(MENU_SETTINGS_VIDEO_OPTIONS_SHOWFPS,   config::gui.show_fps);
  check(MENU_SETTINGS_MUTE,                    config::snes.mute);
  check(MENU_SETTINGS_SPEED_REGULATION_ENABLE, config::system.regulate_speed);

  set_video_mode(config::video.mode);
  set_frameskip(0);
  set_regulation_speed(2);
}
