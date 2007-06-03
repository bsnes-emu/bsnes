void SettingsWindow::setup() {
  create(ui::Window::Center, 640, 365, "bsnes Configuration Settings");
  panel_list.create(*this, 0, 5, 5, 150, 355);
  panel_list.add_item("Raster Settings");
  panel_list.add_item("Input Configuration");
//panel_list.add_item("Cheat Code Editor");
  panel_list.add_item("Advanced");
  panel.create(*this, 0, 160, 5, 475, 355);

  panel_list.set_selection(0);
}

void SettingsWindow::show() {
  Window::show();
  panel_list.focus();
}

int SettingsWindow::message(uint id, void *param) {
  if(id == ui::Message::Close) {
    hide();
    return false;
  }

  if(id == ui::Message::Changed && param == &panel_list) {
    switch(panel_list.get_selection()) {
    case 0: panel.attach(window_raster_settings); break;
    case 1: panel.attach(window_input_config);    break;
  //case 2: panel.attach(window_cheat_editor);    break;
    case 2: panel.attach(window_advanced);        break;
    }
    panel_list.focus();
    return 0;
  }

  return 0;
}
