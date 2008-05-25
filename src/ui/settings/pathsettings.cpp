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
  return true;
}

uintptr_t PathSettingsWindow::autodetect_tick(Event) {
  config::file.autodetect_type = autodetect.checked();
  return true;
}

uintptr_t PathSettingsWindow::selectpath_patch(Event) {
  char t[PATH_MAX];
  if(hiro().folder_select(&window_settings, t) == true) {
    config::path.patch = t;
    patchpath.set_text(config::path.patch);
  }
  return true;
}

uintptr_t PathSettingsWindow::defaultpath_patch(Event) {
  config::path.patch = "";
  patchpath.set_text("");
  return true;
}

uintptr_t PathSettingsWindow::bypass_crc32_tick(Event) {
  config::file.bypass_patch_crc32 = bypass_crc32.checked();
  return true;
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
  return true;
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
  return true;
}

void PathSettingsWindow::setup() {
  create(0, 475, 355);

  lrompath.create(0, 475, 18, translate["Default game ROM path:"]);
  rompath.create(Editbox::Readonly, 265, 25);
  romselect.create(0, 100, 25, translate["Select"]);
  romdefault.create(0, 100, 25, translate["Default"]);
  autodetect.create(0, 475, 18, translate["Auto-detect file compression type (ignore file extension)"]);

  lpatchpath.create(0, 475, 18, translate["Default UPS patch path:"]);
  patchpath.create(Editbox::Readonly, 265, 25);
  patchselect.create(0, 100, 25, translate["Select"]);
  patchdefault.create(0, 100, 25, translate["Default"]);
  bypass_crc32.create(0, 475, 18, translate["Bypass CRC32 patch validation (not recommended)"]);

  lsavepath.create(0, 475, 18, translate["Default save RAM path:"]);
  savepath.create(Editbox::Readonly, 265, 25);
  saveselect.create(0, 100, 25, translate["Select"]);
  savedefault.create(0, 100, 25, translate["Default"]);

  lcheatpath.create(0, 475, 18, translate["Default cheat file path:"]);
  cheatpath.create(Editbox::Readonly, 265, 25);
  cheatselect.create(0, 100, 25, translate["Select"]);
  cheatdefault.create(0, 100, 25, translate["Default"]);

  unsigned y = 0;
  attach(lrompath, 0, y); y += 18;
  attach(rompath, 0, y);
  attach(romselect, 270, y);
  attach(romdefault, 375, y); y += 25;
  attach(autodetect, 0, y); y += 18 + 5;

  attach(lpatchpath, 0, y); y += 18;
  attach(patchpath, 0, y);
  attach(patchselect, 270, y);
  attach(patchdefault, 375, y); y += 25;
  attach(bypass_crc32, 0, y); y += 18 + 5;

  attach(lsavepath, 0, y); y += 18;
  attach(savepath, 0, y);
  attach(saveselect, 270, y);
  attach(savedefault, 375, y); y += 25 + 5;

  attach(lcheatpath, 0, y); y += 18;
  attach(cheatpath, 0, y);
  attach(cheatselect, 270, y);
  attach(cheatdefault, 375, y); y += 25 + 5;

  romselect.on_tick = bind(&PathSettingsWindow::selectpath_rom, this);
  romdefault.on_tick = bind(&PathSettingsWindow::defaultpath_rom, this);
  autodetect.on_tick = bind(&PathSettingsWindow::autodetect_tick, this);

  patchselect.on_tick = bind(&PathSettingsWindow::selectpath_patch, this);
  patchdefault.on_tick = bind(&PathSettingsWindow::defaultpath_patch, this);
  bypass_crc32.on_tick = bind(&PathSettingsWindow::bypass_crc32_tick, this);

  saveselect.on_tick = bind(&PathSettingsWindow::selectpath_save, this);
  savedefault.on_tick = bind(&PathSettingsWindow::defaultpath_save, this);

  cheatselect.on_tick = bind(&PathSettingsWindow::selectpath_cheat, this);
  cheatdefault.on_tick = bind(&PathSettingsWindow::defaultpath_cheat, this);

  rompath.set_text(config::path.rom);
  autodetect.check(config::file.autodetect_type);
  patchpath.set_text(config::path.patch);
  bypass_crc32.check(config::file.bypass_patch_crc32);
  savepath.set_text(config::path.save);
  cheatpath.set_text(config::path.cheat);
}
