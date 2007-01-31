bool MainWindow::close() {
  _term_ = true;
  hide();
  return false;
}

void MainWindow::setup() {
  create("menu|center", uint(256.0 * 4.0 / 3.0 * 3.0), uint(224.0 * 3.0), BSNES_TITLE);
  set_background_color(0, 0, 0);

  menu_begin();
    menu_group_begin(menu_file.create("File"));
      menu_add_item(menu_file_load_normal.create("Load Cartridge"));
      menu_add_item(menu_file_unload.create("Unload"));
      menu_add_separator();
      menu_add_item(menu_file_exit.create("Exit"));
    menu_group_end();

    menu_group_begin(menu_settings.create("Settings"));
      menu_add_item(menu_settings_mute.create("Mute Sound Output"));
      menu_add_separator();
      menu_add_item(menu_settings_config.create("Configuration"));
    menu_group_end();

    menu_group_begin(menu_misc.create("Help"));
      menu_add_item(menu_misc_about.create("About ..."));
    menu_group_end();
  menu_end();

  menu_settings_mute.check(config::snes.mute);

  attach(canvas.create("", uint(256.0 * 4.0 / 3.0 * 3.0), uint(224.0 * 3.0)), 0, 0);
  canvas.set_background_color(0, 0, 0);
}

void MainWindow::clicked(ui::Control *id) {
  if(id == &menu_file_load_normal) { event::load_rom_normal(); }
  if(id == &menu_file_unload) { event::unload_rom(); }
  if(id == &menu_file_exit) { close(); }

  if(id == &menu_settings_mute) { config::snes.mute = menu_settings_mute.checked(); }
  if(id == &menu_settings_config) { window_settings.focus(); }

  if(id == &menu_misc_about) { window_about.focus(); }
}
