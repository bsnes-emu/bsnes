uintptr_t PathSettingsWindow::selectpath_rom(Event) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    config::path.rom = t;
    rompath.set_text(config::path.rom);
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_rom(Event) {
  config::path.rom = "";
  rompath.set_text("");
}

uintptr_t PathSettingsWindow::selectpath_save(Event) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    config::path.save = t;
    savepath.set_text(config::path.save);
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_save(Event) {
  config::path.save = "";
  savepath.set_text("");
}

uintptr_t PathSettingsWindow::selectpath_cheat(Event) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    config::path.cheat = t;
    cheatpath.set_text(config::path.cheat);
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_cheat(Event) {
  config::path.cheat = "";
  cheatpath.set_text("");
}

uintptr_t PathSettingsWindow::autodetect_tick(Event) {
  config::file.autodetect_type = autodetect.checked();
}

void PathSettingsWindow::setup() {
  create(0, 475, 355);

  lrompath.create(0, 475, 20, "Default game ROM path:");
  rompath.create(Editbox::Readonly, 265, 30);
  romselect.create(0, 100, 30, "Select");
  romdefault.create(0, 100, 30, "Default");

  lsavepath.create(0, 475, 20, "Default save RAM path:");
  savepath.create(Editbox::Readonly, 265, 30);
  saveselect.create(0, 100, 30, "Select");
  savedefault.create(0, 100, 30, "Default");

  lcheatpath.create(0, 475, 20, "Default cheat file path:");
  cheatpath.create(Editbox::Readonly, 265, 30);
  cheatselect.create(0, 100, 30, "Select");
  cheatdefault.create(0, 100, 30, "Default");

  autodetect.create(0, 475, 20, "Auto-detect file compression type (ignore file extension)");

  unsigned y = 0;
  attach(lrompath, 0, y); y += 20;
  attach(rompath, 0, y);
  attach(romselect, 270, y);
  attach(romdefault, 375, y); y += 35;
  attach(lsavepath, 0, y); y += 20;
  attach(savepath, 0, y);
  attach(saveselect, 270, y);
  attach(savedefault, 375, y); y += 35;
  attach(lcheatpath, 0, y); y += 20;
  attach(cheatpath, 0, y);
  attach(cheatselect, 270, y);
  attach(cheatdefault, 375, y); y += 35;
  attach(autodetect, 0, y); y += 20;

  romselect.on_tick = bind(&PathSettingsWindow::selectpath_rom, this);
  romdefault.on_tick = bind(&PathSettingsWindow::defaultpath_rom, this);
  saveselect.on_tick = bind(&PathSettingsWindow::selectpath_save, this);
  savedefault.on_tick = bind(&PathSettingsWindow::defaultpath_save, this);
  cheatselect.on_tick = bind(&PathSettingsWindow::selectpath_cheat, this);
  cheatdefault.on_tick = bind(&PathSettingsWindow::defaultpath_cheat, this);
  autodetect.on_tick = bind(&PathSettingsWindow::autodetect_tick, this);

  rompath.set_text(config::path.rom);
  savepath.set_text(config::path.save);
  cheatpath.set_text(config::path.cheat);
  autodetect.check(config::file.autodetect_type);
}
