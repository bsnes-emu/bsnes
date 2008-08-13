void SettingsWindow::setup() {
  create(Window::AutoCenter, 625, 365, translate["bsnes Configuration Settings"]);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  panel_list.create(0, 135, 355);
  panel_list.add_item(translate["Video"]);
  panel_list.add_item(translate["Input"]);
  panel_list.add_item(translate["Paths"]);
  panel_list.add_item(translate["Cheat Codes"]);
  panel_list.add_item(translate["Advanced"]);

  attach(panel_list,              5, 5);
  attach(window_video_settings, 145, 5);
  attach(window_input_config,   145, 5);
  attach(window_path_settings,  145, 5);
  attach(window_cheat_editor,   145, 5);
  attach(window_advanced,       145, 5);

  on_close = bind(&SettingsWindow::close, this);
  panel_list.on_change = bind(&SettingsWindow::list_change, this);
  panel_list.set_selection(1); //default to input configuration (most frequently used panel)
}

uintptr_t SettingsWindow::close(event_t) {
  hide();
  return false;
}

uintptr_t SettingsWindow::list_change(event_t) {
  window_video_settings.hide();
  window_input_config.hide();
  window_path_settings.hide();
  window_cheat_editor.hide();
  window_advanced.hide();

  switch(panel_list.get_selection()) {
    case 0: window_video_settings.show(); break;
    case 1: window_input_config.show();   break;
    case 2: window_path_settings.show();  break;
    case 3: window_cheat_editor.show();   break;
    case 4: window_advanced.show();       break;
  }

  panel_list.focus();
  return true;
}

void SettingsWindow::show() {
  Window::show();
  panel_list.focus();
}
