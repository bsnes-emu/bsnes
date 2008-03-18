void SettingsWindow::setup() {
  create(Window::AutoCenter, 640, 365, "bsnes Configuration Settings");
  set_icon(48, 48, (uint32_t*)resource::icon48);

  panel_list.create(0, 150, 355);
  panel_list.add_item("Raster Settings");
  panel_list.add_item("Input Configuration");
  panel_list.add_item("Path Settings");
  panel_list.add_item("Cheat Code Editor");
  panel_list.add_item("Advanced");

  attach(panel_list, 5, 5);
  attach(window_raster_settings, 160, 5);
  attach(window_input_config, 160, 5);
  attach(window_path_settings, 160, 5);
  attach(window_cheat_editor, 160, 5);
  attach(window_advanced, 160, 5);

  on_close = bind(&SettingsWindow::close, this);
  panel_list.on_change = bind(&SettingsWindow::list_change, this);
  panel_list.set_selection(0);
}

uintptr_t SettingsWindow::close(Event) {
  hide();
  return false;
}

uintptr_t SettingsWindow::list_change(Event) {
  window_raster_settings.hide();
  window_input_config.hide();
  window_path_settings.hide();
  window_cheat_editor.hide();
  window_advanced.hide();

  switch(panel_list.get_selection()) {
    case 0: window_raster_settings.show(); break;
    case 1: window_input_config.show(); break;
    case 2: window_path_settings.show(); break;
    case 3: window_cheat_editor.show(); break;
    case 4: window_advanced.show(); break;
  }

  panel_list.focus();
  return true;
}

void SettingsWindow::show() {
  Window::show();
  panel_list.focus();
}
