void CALLBACK wMainInputTimerProc(HWND hwnd, UINT msg, UINT event, DWORD time) {
  if(uiInput) { ui_poll_input(&wMain); }
}

void MainWindow::SetFrameskip(uint fs) {
  frameskip     = fs % 10;
  frameskip_pos = 0;

  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_0, fs == 0);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_1, fs == 1);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_2, fs == 2);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_3, fs == 3);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_4, fs == 4);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_5, fs == 5);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_6, fs == 6);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_7, fs == 7);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_8, fs == 8);
  CheckMenuItem(MENU_SETTINGS_FRAMESKIP_9, fs == 9);
}

void MainWindow::SetRegulationSpeed(uint speed) {
  speed %= 5;
  regulation_speed = speed;

  switch(speed) {
  case 0: uiAudio->set_frequency(config::system.speed_slowest); break;
  case 1: uiAudio->set_frequency(config::system.speed_slow);    break;
  case 2: uiAudio->set_frequency(config::system.speed_normal);  break;
  case 3: uiAudio->set_frequency(config::system.speed_fast);    break;
  case 4: uiAudio->set_frequency(config::system.speed_fastest); break;
  }

  CheckMenuItem(MENU_SETTINGS_SPEED_REGULATION_SLOWEST, speed == 0);
  CheckMenuItem(MENU_SETTINGS_SPEED_REGULATION_SLOW,    speed == 1);
  CheckMenuItem(MENU_SETTINGS_SPEED_REGULATION_NORMAL,  speed == 2);
  CheckMenuItem(MENU_SETTINGS_SPEED_REGULATION_FAST,    speed == 3);
  CheckMenuItem(MENU_SETTINGS_SPEED_REGULATION_FASTEST, speed == 4);
}

bool MainWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_INPUTKEYDOWN: {
  keymap *key = &uiInput->key;
  uint id   = info.control_id;
  bool ctrl = uiInput->keydown(key->lctrl) || uiInput->keydown(key->rctrl);
    if(id == key->esc) {
      if(uiVideo->settings.fullscreen == false) {
        ShowMenu(!MenuVisible());
        Center();
      } else {
        event::toggle_fullscreen();
      }
    } else if(id == key->f11) {
      event::toggle_fullscreen();
    } else if(id == key->f12) {
      if(bsnes->get_state() == bSNES::RUN) {
        bsnes->set_state(bSNES::STOP);
      } else if(bsnes->get_state() == bSNES::STOP) {
        bsnes->set_state(bSNES::RUN);
      }
    } else if(id == key->num_1 && ctrl) {
      event::set_video_profile(0);
    } else if(id == key->num_2 && ctrl) {
      event::set_video_profile(1);
    } else if(id == key->num_3 && ctrl) {
      event::set_video_profile(2);
    } else if(id == key->num_4 && ctrl) {
      event::set_video_profile(3);
    } else if(id == key->num_5 && ctrl) {
      event::set_video_profile(4);
    } else if(id == key->num_6 && ctrl) {
      event::set_video_profile(5);
    } else if(id == key->num_7 && ctrl) {
      event::set_video_profile(6);
    } else if(id == key->num_8 && ctrl) {
      event::set_video_profile(7);
    } else if((id == key->minus && !ctrl) || id == key->numpad_minus) {
      if(frameskip > 0)SetFrameskip(frameskip - 1);
    } else if((id == key->plus && !ctrl) || id == key->numpad_plus) {
      if(frameskip < 9)SetFrameskip(frameskip + 1);
    } else if(id == key->minus && ctrl) {
      if(regulation_speed > 0)SetRegulationSpeed(regulation_speed - 1);
    } else if(id == key->plus && ctrl) {
      if(regulation_speed < 4)SetRegulationSpeed(regulation_speed + 1);
    }
  } break;

  case EVENT_DRAW: {
    if(r_mem->cart_loaded() == true) {
      uiVideo->redraw();
    }
  } break;

  case EVENT_MENUENTER: {
  //program is paused while menu is active, which
  //causes audio to hang. clear audio buffer so no
  //sound is output during this time.
    uiAudio->clear_audio();
  } break;

  case EVENT_MENUEXIT: {
  time_t timeout = time(0);
  //wait for enter key to be released...
    while(difftime(time(0), timeout) < 3) {
      if(!KeyDown(VK_RETURN))break;
    }
  //...and then clear input, so that emulation does
  //not see that enter was pressed during menu event
    uiInput->clear_input();
  } break;

  case EVENT_MENUCLICKED: {
    switch(info.control_id) {

    case MENU_FILE_LOAD: {
      event::load_rom();
    } break;

    case MENU_FILE_UNLOAD: {
      event::unload_rom();
    } break;

    case MENU_FILE_RESET: {
      if(cartridge.loaded() == true) {
        bsnes->reset();
        dprintf("* Reset");
      }
    } break;

    case MENU_FILE_POWER: {
      if(cartridge.loaded() == true) {
        bsnes->power();
        dprintf("* Power");
      }
    } break;

    case MENU_FILE_EXIT: {
      PostQuitMessage(0);
    } break;

    case MENU_SETTINGS_VIDEOPROFILE_0:
    case MENU_SETTINGS_VIDEOPROFILE_1:
    case MENU_SETTINGS_VIDEOPROFILE_2:
    case MENU_SETTINGS_VIDEOPROFILE_3:
    case MENU_SETTINGS_VIDEOPROFILE_4:
    case MENU_SETTINGS_VIDEOPROFILE_5:
    case MENU_SETTINGS_VIDEOPROFILE_6:
    case MENU_SETTINGS_VIDEOPROFILE_7: {
      event::set_video_profile(info.control_id - MENU_SETTINGS_VIDEOPROFILE_0);
    } break;

    case MENU_SETTINGS_FRAMESKIP_0:
    case MENU_SETTINGS_FRAMESKIP_1:
    case MENU_SETTINGS_FRAMESKIP_2:
    case MENU_SETTINGS_FRAMESKIP_3:
    case MENU_SETTINGS_FRAMESKIP_4:
    case MENU_SETTINGS_FRAMESKIP_5:
    case MENU_SETTINGS_FRAMESKIP_6:
    case MENU_SETTINGS_FRAMESKIP_7:
    case MENU_SETTINGS_FRAMESKIP_8:
    case MENU_SETTINGS_FRAMESKIP_9: {
      SetFrameskip(info.control_id - MENU_SETTINGS_FRAMESKIP_0);
    } break;

    case MENU_SETTINGS_SHOWFPS: {
      config::misc.show_fps.toggle();
      SetWindowText(hwnd, BSNES_TITLE);
      CheckMenuItem(MENU_SETTINGS_SHOWFPS, config::misc.show_fps);
    } break;

    case MENU_SETTINGS_MUTE: {
      config::snes.mute.toggle();
      CheckMenuItem(MENU_SETTINGS_MUTE, config::snes.mute);
    } break;

    case MENU_SETTINGS_SPEED_REGULATION_ENABLE: {
      config::system.regulate_speed.toggle();
      CheckMenuItem(MENU_SETTINGS_SPEED_REGULATION_ENABLE, config::system.regulate_speed);
    } break;

    case MENU_SETTINGS_SPEED_REGULATION_SLOWEST:
    case MENU_SETTINGS_SPEED_REGULATION_SLOW:
    case MENU_SETTINGS_SPEED_REGULATION_NORMAL:
    case MENU_SETTINGS_SPEED_REGULATION_FAST:
    case MENU_SETTINGS_SPEED_REGULATION_FASTEST: {
      SetRegulationSpeed(info.control_id - MENU_SETTINGS_SPEED_REGULATION_SLOWEST);
    } break;

    case MENU_SETTINGS_CONFIGURATION: {
      wSettings.Show();
    } break;

    case MENU_SETTINGS_DEBUGGER: {
      (debugger.active() == false) ? debugger.activate() : debugger.deactivate();
    } break;

    case MENU_MISC_SCREENSHOT: {
      event::capture_screenshot();
    } break;

    case MENU_MISC_LOGAUDIO: {
      if(MenuItemChecked(MENU_MISC_LOGAUDIO) == false) {
        CheckMenuItem(MENU_MISC_LOGAUDIO);
        snes->log_audio_enable();
      } else {
        UncheckMenuItem(MENU_MISC_LOGAUDIO);
        snes->log_audio_disable();
      }
    } break;

    case MENU_MISC_CHEATEDITOR: {
      settings.set_active_panel(&wCheatEditor);
      wSettings.Show();
    } break;

    case MENU_MISC_ABOUT: {
      wAbout.Show();
    } break;

    }
  } break;

  case EVENT_DESTROY: {
    PostQuitMessage(0);
  } break;

  }

  return false;
}

void MainWindow::Setup() {
char t[128];
  CreateMenu();

  AddMenuGroup("&File");
    AddMenuItem(MENU_FILE_LOAD,   "&Load ROM");
    AddMenuItem(MENU_FILE_UNLOAD, "&Unload ROM");
    AddMenuSeparator();
    AddMenuItem(MENU_FILE_RESET,  "&Reset");
    AddMenuItem(MENU_FILE_POWER,  "&Power (Hard Reset)");
    AddMenuSeparator();
    AddMenuItem(MENU_FILE_EXIT,   "E&xit");
  EndMenuGroup();

  AddMenuGroup("&Settings");
    AddMenuGroup("&Video Profile");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_0, "Profile &1");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_1, "Profile &2");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_2, "Profile &3");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_3, "Profile &4");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_4, "Profile &5");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_5, "Profile &6");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_6, "Profile &7");
      AddMenuItem(MENU_SETTINGS_VIDEOPROFILE_7, "Profile &8");
    EndMenuGroup();

    AddMenuGroup("&Frameskip");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_0, "&0 (Off)");
      AddMenuSeparator();
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_1, "&1");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_2, "&2");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_3, "&3");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_4, "&4");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_5, "&5");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_6, "&6");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_7, "&7");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_8, "&8");
      AddMenuItem(MENU_SETTINGS_FRAMESKIP_9, "&9");
    EndMenuGroup();

    AddMenuItem(MENU_SETTINGS_SHOWFPS, "&Show FPS");

    AddMenuSeparator();
    AddMenuItem(MENU_SETTINGS_MUTE, "&Mute Sound Output");
    AddMenuSeparator();

    AddMenuGroup("&Speed Regulation");
      AddMenuItem(MENU_SETTINGS_SPEED_REGULATION_ENABLE, "&Enable");
      AddMenuSeparator();
      sprintf(t, "&Slowest (%d%%)", uint(100.0 * (double(config::system.speed_slowest) / 32000.0)));
      AddMenuItem(MENU_SETTINGS_SPEED_REGULATION_SLOWEST, t);
      sprintf(t, "&Slow (%d%%)", uint(100.0 * (double(config::system.speed_slow) / 32000.0)));
      AddMenuItem(MENU_SETTINGS_SPEED_REGULATION_SLOW, t);
      sprintf(t, "&Normal (%d%%)", uint(100.0 * (double(config::system.speed_normal) / 32000.0)));
      AddMenuItem(MENU_SETTINGS_SPEED_REGULATION_NORMAL, t);
      sprintf(t, "&Fast (%d%%)", uint(100.0 * (double(config::system.speed_fast) / 32000.0)));
      AddMenuItem(MENU_SETTINGS_SPEED_REGULATION_FAST, t);
      sprintf(t, "&Fastest (%d%%)", uint(100.0 * (double(config::system.speed_fastest) / 32000.0)));
      AddMenuItem(MENU_SETTINGS_SPEED_REGULATION_FASTEST, t);
    EndMenuGroup();

    AddMenuItem(MENU_SETTINGS_CONFIGURATION, "&Configuration");
  #ifdef DEBUGGER
    AddMenuItem(MENU_SETTINGS_DEBUGGER, "&Debugger");
  #endif
  EndMenuGroup();

  AddMenuGroup("&Misc");
    AddMenuItem(MENU_MISC_SCREENSHOT,  "Capture &Screenshot");
    AddMenuItem(MENU_MISC_LOGAUDIO,    "&Log Audio Data");
    AddMenuItem(MENU_MISC_CHEATEDITOR, "&Cheat Code Editor");
    AddMenuSeparator();
    AddMenuItem(MENU_MISC_ABOUT,      "&About");
  EndMenuGroup();

  ShowMenu();

  CheckMenuItem(MENU_SETTINGS_SHOWFPS,                 config::misc.show_fps);
  CheckMenuItem(MENU_SETTINGS_MUTE,                    config::snes.mute);
  CheckMenuItem(MENU_SETTINGS_SPEED_REGULATION_ENABLE, config::system.regulate_speed);

  event::set_video_profile(config::video.profile);
  SetFrameskip(0);
  SetRegulationSpeed(2);

  SetTimer(hwnd, 0, 50, wMainInputTimerProc);
}
