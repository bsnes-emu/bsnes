bool SettingsWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CLOSE: {
    Hide();
    return true;
  } break;

  case EVENT_CHANGED: {
    if(info.control == &Panel) {
      switch(Panel.GetSelection()) {
      case PANEL_VIDEOSETTINGS:  settings.set_active_panel(&wVideoSettings);  break;
      case PANEL_COLORADJUST:    settings.set_active_panel(&wColorAdjust);    break;
      case PANEL_RASTERSETTINGS: settings.set_active_panel(&wRasterSettings); break;
      case PANEL_INPUTCONFIG:    settings.set_active_panel(&wInputConfig);    break;
      case PANEL_CHEATEDITOR:    settings.set_active_panel(&wCheatEditor);    break;
      default:                   settings.set_active_panel(0);                break;
      }
      SetFocus(Panel.hwnd);
    }
  } break;

  }

  return false;
}

void SettingsWindow::Show() {
  settings.show_active_panel();
  Window::Show();
  Panel.Focus();
}

void SettingsWindow::Hide() {
  Window::Hide();
  settings.hide_active_panel();
}

void SettingsWindow::Setup() {
  Panel.Create(this, "visible|edge", 5, 5, 150, 360,
    "Video Settings|"
    "Color Adjustment|"
    "Raster Settings|"
    "|"
    "Input Configuration|"
    "|"
    "Cheat Code Editor"
  );

  settings.set_active_panel(&wVideoSettings);
}
