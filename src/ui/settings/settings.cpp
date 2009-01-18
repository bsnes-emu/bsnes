void SettingsWindow::setup() {
  create(Window::AutoCenter, 580, 380, translate["bsnes Configuration Settings"]);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  panel_list.create(0, 114, 370);
  panel_list.add_item(translate["Drivers"]);
  panel_list.add_item(translate["Video"]);
  panel_list.add_item(translate["Audio"]);
  panel_list.add_item(translate["Input"]);
  panel_list.add_item(translate["Paths"]);
  panel_list.add_item(translate["Cheat Codes"]);
  panel_list.add_item(translate["Advanced"]);

  attach(panel_list,              5, 5);
  attach(window_driver_select,  124, 5);
  attach(window_video_settings, 124, 5);
  attach(window_audio_settings, 124, 5);
  attach(window_input_config,   124, 5);
  attach(window_path_settings,  124, 5);
  attach(window_cheat_editor,   124, 5);
  attach(window_advanced,       124, 5);

  on_close = bind(&SettingsWindow::close, this);
  panel_list.on_change = bind(&SettingsWindow::list_change, this);

  //if emulator crashed on last run, default to driver select window and disable list selection,
  //otherwise default to input configuration (most frequently used panel)
  panel_list.set_selection(config.system.invoke_crash_handler == true ? 0 : 3);
  panel_list.enable(config.system.invoke_crash_handler == false);
  list_change(event_t(event_t::Change));
}

uintptr_t SettingsWindow::close(event_t) {
  hide();
  return false;
}

uintptr_t SettingsWindow::list_change(event_t) {
  window_driver_select.hide();
  window_video_settings.hide();
  window_audio_settings.hide();
  window_input_config.hide();
  window_path_settings.hide();
  window_cheat_editor.hide();
  window_advanced.hide();

  switch(panel_list.get_selection()) {
    case 0: window_driver_select.show();   break;
    case 1: window_video_settings.show(); break;
    case 2: window_audio_settings.show(); break;
    case 3: window_input_config.show();   break;
    case 4: window_path_settings.show();  break;
    case 5: window_cheat_editor.show();   break;
    case 6: window_advanced.show();       break;
  }

  panel_list.focus();
  return true;
}

void SettingsWindow::show() {
  Window::show();
  panel_list.focus();
}
